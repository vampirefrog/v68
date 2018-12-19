#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "v68.h"
#include "v68io.h"
#include "v68mem.h"
#include "v68periph.h"
#include "musashi/m68kcpu.h"

struct v68 v68;

int v68_init(int clock, int ram_size, int sample_rate) {
	memset(&v68, 0, sizeof(v68));

	v68.log_dasm = 0;
	v68.verbosity = 0;

	v68.ram_size = ram_size;
	v68.cpu_clock = clock;
	v68.sample_rate = sample_rate;

	v68.ram = malloc(v68.ram_size);
	if(!v68.ram) return errno;

	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	m68k_pulse_reset();

	v68_mem_init();
	v68_periph_init();
	v68_io_init();
	v68_human_init();

	return 0;
}

int v68_shutdown() {
	free(v68.ram);

	return 0;
}

int v68_render_tstates(int tstates) {
	int64_t x = (int64_t)tstates * (int64_t)v68.sample_rate + v68.samples_remainder;
	int64_t samples = x / (int64_t)v68.cpu_clock;
	if(samples > v68.buf_remaining)
		samples = v68.buf_remaining;
	v68.samples_remainder = x - samples * v68.cpu_clock;
	verbose1("render tstates %d buf_remaining=%d samples=%ld\n", tstates, v68.buf_remaining, samples);

	if(samples > 0) {
		v68_periph_render(v68.bufL, v68.bufR, samples);
#ifndef __EMSCRIPTEN__
		if(v68.logger) {
			vgm_logger_wait(v68.logger, samples);
		}
#endif

		v68.bufL += samples;
		v68.bufR += samples;
		v68.buf_remaining -= samples;
	}

	return samples;
}

extern int m68ki_initial_cycles;
int v68_fill_buffer(int32_t *bufL, int32_t *bufR, int samples) {
	v68.prev_sound_cycles = 0;
	v68.buf_remaining = samples;
	v68.bufL = bufL;
	v68.bufR = bufR;

	int64_t x = (int64_t)samples * v68.cpu_clock + v68.cpu_cycle_remainder;
	int cpu_tstates = x / v68.sample_rate;
	v68.cpu_cycle_remainder = x - cpu_tstates * v68.sample_rate;

	verbose2("v68 fill buffer %d  tstates=%d  stopped=%d  pc=%08x  rate=%d\n", samples, cpu_tstates, CPU_STOPPED, REG_PC, v68.sample_rate);

	int remaining_tstates = cpu_tstates;
	while(remaining_tstates > 0) {
		verbose1("remaining tstates %d flags=%02x\n", remaining_tstates, v68.opm_flags);
		int next_int = v68_periph_next_int(remaining_tstates);

		verbose1("executing %d\n", next_int);
		v68.cpu_ended_timeslice = 0;
		v68.prev_sound_cycles = 0;
		int cycles = m68k_execute(next_int, v68.log_dasm);
		int executed_cycles = v68.cpu_ended_timeslice ? next_int - m68ki_initial_cycles : cycles;
		verbose1("ended=%d cycles=%d remaining=%d initial=%d executed=%d run=%d next_int=%d calculated=%d\n", v68.cpu_ended_timeslice, cycles, remaining_tstates, m68ki_initial_cycles, executed_cycles, m68k_cycles_run(), next_int, next_int - m68ki_initial_cycles);
		verbose1("executed cycles %d / %d\n", executed_cycles, next_int);
		v68.cpu_ended_timeslice = 0;

		v68_render_tstates(executed_cycles - v68.prev_sound_cycles);
		verbose1("timer A cycles = %d, timer B cycles = %d flags=%02x\n", v68.opm_timera_cycles, v68.opm_timerb_cycles, v68.opm_flags);
		v68_periph_advance(executed_cycles);
		remaining_tstates -= executed_cycles;
	}

	verbose1("filled buffer buf_remaining=%d\n", v68.buf_remaining);

	return 0;
}

unsigned int  m68k_read_memory_8(unsigned int addr) {
	if(addr >= 0x00e80000 && addr < 0x00eb0000)
		return v68_read_periph_8(addr);
	else if(addr > v68.ram_size) {
		verbose2("Could not read RAM at 0x%08x\n", addr);
		return 0;
	}
	return v68.ram[addr];
}

unsigned int  m68k_read_memory_16(unsigned int addr) {
	if(addr >= 0x00e80000 && addr < 0x00eb0000) return v68_read_periph_16(addr);
	else if(addr > v68.ram_size) {
		verbose2("Could not read RAM at 0x%08x\n", addr);
		return 0;
	}
	return (v68.ram[addr] << 8) | v68.ram[addr + 1];
}

unsigned int  m68k_read_memory_32(unsigned int addr) {
	if(addr >= 0x00e80000 && addr < 0x00eb0000) return v68_read_periph_32(addr);
	else if(addr > v68.ram_size) {
		verbose2("Could not read RAM at 0x%08x\n", addr);
		return 0;
	}
	return (v68.ram[addr] << 24) | (v68.ram[addr+1] << 16) | (v68.ram[addr+2] << 8) | v68.ram[addr+3];
}

void m68k_write_memory_8(unsigned int addr, unsigned int data) {
	verbose3("WRITE8  @0x%08x = %02x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_8(addr, data);
		return;
	} else if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%02x\n", addr, data);
		return;
	}
	v68.ram[addr] = data;
}

void m68k_write_memory_16(unsigned int addr, unsigned int data) {
	verbose3("WRITE16 @0x%08x = %04x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_16(addr, data);
		return;
	} else if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%04x\n", addr, data);
		return;
	}
	v68.ram[addr++] = data >> 8;
	v68.ram[addr] = data;
}

void m68k_write_memory_32(unsigned int addr, unsigned int data) {
	verbose3("WRITE32 @0x%08x = %08x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		v68_write_periph_32(addr, data);
		return;
	} else if(addr > v68.ram_size) {
		verbose2("Could not write RAM at 0x%08x = 0x%08x\n", addr, data);
		return;
	}

	v68.ram[addr++] = data >> 24;
	v68.ram[addr++] = data >> 16;
	v68.ram[addr++] = data >> 8;
	v68.ram[addr] = data;
}

unsigned int m68k_read_disassembler_8  (unsigned int addr) {
	verbose3("READ8  @0x%08x\n", addr);
	return m68k_read_memory_8(addr);
}

unsigned int m68k_read_disassembler_16 (unsigned int addr) {
	verbose3("READ16 @0x%08x\n", addr);
	return m68k_read_memory_16(addr);
}

unsigned int m68k_read_disassembler_32 (unsigned int addr) {
	verbose3("READ32 @0x%08x\n", addr);
	return m68k_read_memory_32(addr);
}

void v68_iocs_call(uint16_t instr);
int v68_trap(int which) {
	switch(which) {
		case 15: {
				/* IOCS */
				v68_iocs_call(m68k_get_reg(NULL, M68K_REG_D0));
				return 0;
			}
			break;
		default:
			printf("v68_trap %d\n", which);
	}
	return 1;
}
