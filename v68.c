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
	v68.sound_touched = 0;
	v68.sample_rate = sample_rate;

	v68.ram = calloc(v68.ram_size, 1);
	if(!v68.ram) return errno;

	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);

	v68_io_init();

	return 0;
}

void v68_boot() {
	verbose1("v68_boot\n");
	v68_ipl_init();
	v68_human_init();
	v68_periph_init();
	v68_iocs_init();
	m68k_pulse_reset();
}

void v68_cpu_reset_instr_cb(void) {
	verbose2("RESET peripherals\n");
	v68.reset_pulsed = 1;
}

int v68_shutdown() {
	free(v68.ram);

	return 0;
}

extern int m68ki_initial_cycles;
void v68_run() {
	verbose1("v68_run cpu_clock=%d sound_touched=%d running=%d log_calls=%d\n", v68.cpu_clock, v68.sound_touched, v68.running, v68.log_calls);
	int remaining_tstates = v68.cpu_clock;

	v68.running = 1;

	int extra_samples = 100;

	for(int i = 0; (v68.running || !v68.sound_touched) && i < extra_samples; i += (v68.running || !v68.sound_touched ? 0 : 1)) {
		// run for one sample at a time
		int64_t x = v68.cpu_clock + v68.cpu_cycle_remainder;
		int cpu_tstates = x / v68.sample_rate;

		verbose2("v68_run  executing cpu_tstates=%d remaining_tstates=%d log_calls=%d\n", cpu_tstates, remaining_tstates, v68.log_calls);
		int executed_cycles = m68k_execute(cpu_tstates, v68.log_dasm);
		v68.cpu_cycle_remainder = x - executed_cycles * v68.sample_rate;
	}
}

int v68_fill_buffer(int samples, int16_t *bufL, int16_t *bufR) {
	verbose1("v68_fill_buffer samples=%d running=%d\n", samples, v68.running);
	memset(bufL, 0, samples * sizeof(*bufL));
	memset(bufR, 0, samples * sizeof(*bufR));

	for(int i = 0; i < samples; i++) {
		// render one sample at a time
		int64_t x = v68.cpu_clock + v68.cpu_cycle_remainder;
		int cpu_tstates = x / v68.sample_rate;

		if(v68.running) {
			int executed_cycles = m68k_execute(cpu_tstates, v68.log_dasm);
			v68.cpu_cycle_remainder = x - executed_cycles * v68.sample_rate;
		}
		v68_periph_advance(cpu_tstates);
		v68_periph_render_sample(bufL++, bufR++);
	}

	return 0;
}

unsigned int m68k_read_memory_8(unsigned int addr) {
	if(addr <= 0x0000ffff && !v68.reset_pulsed) {
		uint8_t r = v68_ipl_read_8(addr + 0xff0000);
		verbose3("READ8 BOOT 0x%08x = 0x%02x\n", addr, r);
		return r;
	}

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
		fprintf(stderr, "READ8 ERROR 0x%08x\n", addr);
		exit(1);
	}

	uint8_t r = v68.ram[addr];
	verbose3("READ8 RAM 0x%08x = 0x%02x\n", addr, r);
	return r;
}

unsigned int  m68k_read_memory_16(unsigned int addr) {
	if(addr <= 0x0000ffff && !v68.reset_pulsed) {
		uint16_t r = v68_ipl_read_16(addr + 0xff0000);
		verbose3("READ16 BOOT 0x%08x = 0x%04x\n", addr, r);
		return r;
	}

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
		fprintf(stderr, "READ16 ERROR 0x%08x\n", addr);
		exit(1);
	}

	uint16_t r = (v68.ram[addr] << 8) | v68.ram[addr + 1];
	verbose3("READ16 RAM 0x%08x = 0x%04x\n", addr, r);
	return r;
}

unsigned int  m68k_read_memory_32(unsigned int addr) {
	if(addr <= 0x0000ffff && !v68.reset_pulsed) {
		uint32_t r = v68_ipl_read_32(addr + 0xff0000);
		verbose3("READ32 BOOT 0x%08x = 0x%08x\n", addr, r);
		return r;
	}

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
		fprintf(stderr, "READ32 ERROR 0x%08x\n", addr);
		exit(1);
	}

	uint32_t r =
		(v68.ram[addr] << 24) |
		(v68.ram[addr+1] << 16) |
		(v68.ram[addr+2] << 8) |
		v68.ram[addr+3];
	verbose3("READ32 RAM 0x%08x = 0x%08x\n", addr, r);
	return r;
}

void m68k_write_memory_8(unsigned int addr, unsigned int data) {
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		verbose3("WRITE8 PERIPH 0x%08x = 0x%02x\n", addr, data);
		v68_write_periph_8(addr, data);
		return;
	}
	verbose3("WRITE8 RAM 0x%08x = 0x%02x\n", addr, data);
	if(addr > v68.ram_size) {
		fprintf(stderr, "Could not write RAM at 0x%08x = 0x%02x\n", addr, data);
		exit(1);
	}
	v68.ram[addr] = data;
}

void m68k_write_memory_16(unsigned int addr, unsigned int data) {
	verbose3("WRITE16 0x%08x = 0x%04x\n", addr, data);
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		verbose3("WRITE16 PERIPH 0x%08x = 0x%04x\n", addr, data);
		v68_write_periph_16(addr, data);
		return;
	}
	if(addr > v68.ram_size) {
		fprintf(stderr, "Could not write RAM at 0x%08x = 0x%04x\n", addr, data);
		exit(1);
	}
	v68.ram[addr++] = data >> 8;
	v68.ram[addr] = data;
}

void m68k_write_memory_32(unsigned int addr, unsigned int data) {
	if(addr >= 0x00e80000 && addr < 0x00eb0000) {
		verbose3("WRITE32 PERIPH 0x%08x = 0x%08x\n", addr, data);
		v68_write_periph_32(addr, data);
		return;
	}
	verbose3("WRITE32 RAM 0x%08x = 0x%08x\n", addr, data);
	if(addr > v68.ram_size) {
		fprintf(stderr, "Could not write RAM at 0x%08x = 0x%08x\n", addr, data);
		exit(1);
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
