#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "v68.h"
#include "v68ipl.h"
#include "v68io.h"
#include "v68periph.h"
#include "v68iocscall.h"
#include "musashi/m68kcpu.h"


struct v68 v68;

int v68_init(int clock, int ram_size, int sample_rate) {
	memset(&v68, 0, sizeof(v68));

	v68.log_dasm = 0;
	v68.verbosity = 0;

	v68.ram_size = ram_size;
	v68.cpu_clock = clock;
	v68.sample_rate = sample_rate;
	v68.samples_remainder = 0;

	v68.ram = calloc(v68.ram_size, 1);
	if(!v68.ram) return errno;

	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	m68k_pulse_reset();

	v68_ipl_init();
	v68_periph_init();
	v68_io_init();
	v68_human_init();
	v68_iocs_init();

	return 0;
}

int v68_shutdown() {
	free(v68.ram);

	return 0;
}

extern int m68ki_initial_cycles;
int v68_fill_buffer(int16_t *bufL, int16_t *bufR, int samples) {
	verbose1("v68_fill_buffer samples=%d\n", samples);
	v68.prev_sound_cycles = 0;
	v68.buf_remaining = samples;
	v68.bufL = bufL;
	v68.bufR = bufR;

	int64_t x = (int64_t)samples * v68.cpu_clock + v68.cpu_cycle_remainder;
	int cpu_tstates = x / v68.sample_rate;
	v68.cpu_cycle_remainder = x - cpu_tstates * v68.sample_rate;

	verbose2("v68_fill_buffer  cpu_tstates=%d CPU_STOPPED=%d PC=%08x v68.sample_rate=%d\n", cpu_tstates, CPU_STOPPED, REG_PC, v68.sample_rate);

	int remaining_tstates = cpu_tstates;
	while(remaining_tstates > 0) {
		verbose2("v68_fill_buffer  remaining_tstates=%d opm_flags=0x%02x\n", remaining_tstates, v68.opm_flags);
		int next_int = v68_periph_next_int(remaining_tstates);

		verbose2("v68_fill_buffer  executing next_int=%d remaining_tstates=%d\n", next_int, remaining_tstates);
		v68.cpu_ended_timeslice = 0;
		v68.prev_sound_cycles = 0;
		int cycles = m68k_execute(next_int, v68.log_dasm);
		int executed_cycles = v68.cpu_ended_timeslice ? next_int - m68ki_initial_cycles : cycles;
		verbose2("v68_fill_buffer  executed_cycles=%d next_int=%d remaining_tstates = %d\n", executed_cycles, next_int, remaining_tstates);
		v68.cpu_ended_timeslice = 0;

		verbose2("v68_fill_buffer  advancing executed_cycles=%d - prev_sound_cycles=%d = %d\n", executed_cycles, v68.prev_sound_cycles, executed_cycles - v68.prev_sound_cycles);
		v68.periph_cycles = v68.prev_sound_cycles;
		v68_periph_advance(executed_cycles - v68.prev_sound_cycles);
		verbose2("v68_fill_buffer  remaining_tstates=%d  executed_cycles=%d  remaining_tstates-executed_cycles=%d\n", remaining_tstates, executed_cycles, remaining_tstates-executed_cycles);
		remaining_tstates -= executed_cycles;
	}

	verbose2("v68_fill_buffer  filled buffer buf_remaining=%d\n", v68.buf_remaining);

	return 0;
}

unsigned int m68k_read_memory_8(unsigned int addr) {
	// addr &= 0x00ffffff;

	if(addr >= 0x00fe0000 && addr <= 0x00ffffff) {
		uint8_t r = v68_ipl_read_8(addr);
		verbose3("READ8 IPL 0x%08x = 0x%02x\n", addr, r);
		return r;
	}

	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		uint8_t r = v68_read_periph_8(addr);
		verbose3("READ8 PERIPH 0x%08x = 0x%02x\n", addr, r);
		return r;
	}

	if(addr >= v68.ram_size) {
		verbose2("READ8 ERROR 0x%08x\n", addr);
		return 0;
	}

	uint8_t r = v68.ram[addr];
	verbose3("READ8 RAM 0x%08x = 0x%02x\n", addr, r);
	return r;
}

unsigned int  m68k_read_memory_16(unsigned int addr) {
	// addr &= 0x00ffffff;

	if(addr >= 0x00fe0000 && addr <= 0x00ffffff) {
		uint16_t r = v68_ipl_read_16(addr);
		verbose3("READ16 IPL 0x%08x = 0x%04x\n", addr, r);
		return r;
	}

	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		uint16_t r = v68_read_periph_16(addr);
		verbose3("READ16 PERIPH 0x%08x = 0x%04x\n", addr, r);
		return r;
	}

	if(addr >= v68.ram_size - 1) {
		verbose2("READ16 ERROR 0x%08x\n", addr);
		return 0;
	}

	uint16_t r = (v68.ram[addr] << 8) | v68.ram[addr + 1];
	verbose3("READ16 RAM 0x%08x = 0x%04x\n", addr, r);
	return r;
}

unsigned int  m68k_read_memory_32(unsigned int addr) {
	// addr &= 0x00ffffff;

	if(addr >= 0x00fe0000 && addr <= 0x00ffffff) {
		uint32_t r = v68_ipl_read_32(addr);
		verbose3("READ32 IPL 0x%08x = 0x%08x\n", addr, r);
		return r;
	}

	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		uint32_t r = v68_read_periph_32(addr);
		verbose3("READ32 PERIPH 0x%08x = 0x%08x\n", addr, r);
		return r;
	}

	if(addr > v68.ram_size) {
		verbose2("READ32 ERROR 0x%08x\n", addr);
		return 0;
	}

	uint32_t r = (v68.ram[addr] << 24) | (v68.ram[addr+1] << 16) | (v68.ram[addr+2] << 8) | v68.ram[addr+3];
	verbose3("READ32 @0x%08x = 0x%08x\n", addr, r);
	return r;
}

void m68k_write_memory_8(unsigned int addr, unsigned int data) {
	// addr &= 0x00ffffff;

	verbose3("WRITE8 0x%08x = %02x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_8(addr, data);
		return;
	}
	if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%02x\n", addr, data);
		return;
	}
	v68.ram[addr] = data;
}

void m68k_write_memory_16(unsigned int addr, unsigned int data) {
	// addr &= 0x00ffffff;

	verbose3("WRITE16 0x%08x = %04x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_16(addr, data);
		return;
	}
	if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%04x\n", addr, data);
		return;
	}
	v68.ram[addr++] = data >> 8;
	v68.ram[addr] = data;
}

void m68k_write_memory_32(unsigned int addr, unsigned int data) {
	// addr &= 0x00ffffff;

	verbose3("WRITE32 0x%08x = %08x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_32(addr, data);
		return;
	}
	if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%08x\n", addr, data);
		return;
	}

	v68.ram[addr++] = data >> 24;
	v68.ram[addr++] = data >> 16;
	v68.ram[addr++] = data >> 8;
	v68.ram[addr] = data;
}

unsigned int m68k_read_disassembler_8  (unsigned int addr) {
	return m68k_read_memory_8(addr);
}

unsigned int m68k_read_disassembler_16 (unsigned int addr) {
	return m68k_read_memory_16(addr);
}

unsigned int m68k_read_disassembler_32 (unsigned int addr) {
	return m68k_read_memory_32(addr);
}

int v68_trap(int which) {
	verbose1("v68_trap which=%d d0=0x%08x\n", which, m68k_get_reg(NULL, M68K_REG_D0));
	switch(which) {
		case 15:
			/* IOCS */
			return v68_iocs_call(m68k_get_reg(NULL, M68K_REG_D0));
		default:
			verbose2("v68_trap unknown #%d d0=0x%08x\n", which, m68k_get_reg(0, M68K_REG_D0));
	}
	return 1;
}
