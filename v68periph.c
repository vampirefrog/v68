#include "v68.h"
#include "v68periph.h"
#include "musashi/m68kcpu.h"

/* YM2151 OPM */
#define OPM_CLOCK 4000000
static uint32_t v68_opm_calc_timera(void);
static uint32_t v68_opm_calc_timerb(void);

void v68_periph_init() {
	ym2151_init(&v68.opm, OPM_CLOCK, v68.sample_rate);
	ym2151_reset_chip(&v68.opm);
	v68.opm_clka = 0;
	v68.opm_clkb = 0;
	v68.opm_addr_latch = 0;
	v68.opm_timera_counter = 0;
	v68.opm_timerb_counter = 0;
	v68.opm_timera_cycles = v68_opm_calc_timera();
	v68.opm_timerb_cycles = v68_opm_calc_timerb();
	v68.periph_timers_altered = 0;
	v68.opm_flags = 0;

	okim6258_init(&v68.oki, 8000000, FOSC_DIV_BY_512, TYPE_4BITS, OUTPUT_12BITS);
	okim6258_reset(&v68.oki);
}

uint32_t v68_periph_next_int(uint32_t cycles) {
	uint32_t next_int = cycles;

	if((v68.opm_flags & 0x05) && (v68.opm_timera_counter < v68.opm_timera_cycles) && (v68.opm_timera_cycles - v68.opm_timera_counter) < next_int) {
		next_int = (v68.opm_timera_cycles - v68.opm_timera_counter);
	}

	if((v68.opm_flags & 0x0a) && (v68.opm_timerb_counter < v68.opm_timerb_cycles) && (v68.opm_timerb_cycles - v68.opm_timerb_counter) < next_int) {
		next_int = (v68.opm_timerb_cycles - v68.opm_timerb_counter);
	}

	verbose1("next int cycles=%d ret=%d\n", cycles, next_int);
	return next_int;
}

uint32_t v68_int_ack_handler(int int_level) {
	verbose1("v68_int_ack_handler %d\n", int_level);
	if(int_level == 6) {
		CPU_INT_LEVEL = 0;
		int vec = v68.mfp_vec;
		if(vec) { // MFP
			verbose1("MFP int ack vec=%04x (%08x = %08x)\n", v68.mfp_vec, v68.mfp_vec * 4, m68k_read_memory_32(v68.mfp_vec * 4));
			v68.mfp_vec = 0;
			return vec;
		}
	}
	return M68K_INT_ACK_SPURIOUS;
}

void v68_periph_advance(uint32_t cycles) {
	verbose1("periph advance %d opm_flags=%02x \n", cycles, v68.opm_flags);
	verbose1("timer A counter = %d  timer A cycles = %d\n", v68.opm_timera_counter, v68.opm_timera_cycles);
	verbose1("timer B counter = %d  timer B cycles = %d\n", v68.opm_timerb_counter, v68.opm_timerb_cycles);
	/* If timer A enabled */
	if((v68.opm_flags & 0x01) && v68.opm_timera_counter < v68.opm_timera_cycles) {
		v68.opm_timera_counter += cycles;
		if(v68.opm_timera_counter >= v68.opm_timera_cycles && (v68.opm_flags & 0x04)) {
			v68.mfp_vec = 0x43;
			verbose1("Generating IRQ 6 Timer A\n");
			m68k_set_irq(6);
		}
	} else if((v68.opm_flags & 0x02) && v68.opm_timerb_counter < v68.opm_timerb_cycles) {
		v68.opm_timerb_counter += cycles;
		if(v68.opm_timerb_counter >= v68.opm_timerb_cycles && (v68.opm_flags & 0x08)) {
			v68.mfp_vec = 0x43;
			verbose1("Generating IRQ 6 Timer B\n");
			m68k_set_irq(6);
		}
	}
}

void v68_opm_write_addr(uint8_t addr) {
	v68.opm_addr_latch = addr;
}

static uint32_t v68_opm_calc_timera() {
	return (uint64_t)(1024 - v68.opm_clka) * v68.cpu_clock / 62500;
}

static uint32_t v68_opm_calc_timerb() {
	return 4 * (uint64_t)(256 - v68.opm_clka) * v68.cpu_clock / 15625;
}

void v68_opm_write_data(uint8_t data) {
	verbose1("OPM %02x %02x\n", v68.opm_addr_latch, data);
	ym2151_write_reg(&v68.opm, v68.opm_addr_latch, data);
#ifndef __EMSCRIPTEN__
	if(v68.logger)
		vgm_logger_write_ym2151(v68.logger, v68.opm_addr_latch, data);
#endif

	int clka = v68.opm_clka, clkb = v68.opm_clkb;
	switch(v68.opm_addr_latch) {
		case 0x10:
			clka &= 0x0003;
			clka |= data << 2;
			if(v68.opm_clka != clka) {
				v68.opm_timera_cycles = v68_opm_calc_timera();
				v68.periph_timers_altered = 1;
			}
			break;
		case 0x11:
			clka &= 0x03fc;
			clka |= data & 0x03;
			if(v68.opm_clka != clka) {
				v68.opm_timera_cycles = v68_opm_calc_timera();
				v68.periph_timers_altered = 1;
			}
			break;
		case 0x12:
			clkb = data;
			if(v68.opm_clkb != clkb) {
				v68.opm_timerb_cycles = v68_opm_calc_timerb();
				v68.periph_timers_altered = 1;
			}
			break;
		case 0x14: {
				if((data & 0x3f) != (v68.opm_flags & 0x3f)) {
					v68.periph_timers_altered = 1;
				}
				v68.opm_flags = data & 0x0f;
				/* Check for reset */
				if(data & 0x10) {
					v68.opm_timera_counter = 0;
				}
				if(data & 0x20) {
					v68.opm_timerb_counter = 0;
				}
			}
			break;
		case 0x1b: {
				if((data & 0x80) != (v68.opm_ct & 0x80)) {
					v68.periph_timers_altered = 1;
				}
				v68.opm_ct = data & 0xc0;
			}
			break;
	}
}

uint8_t v68_opm_read_data() {
	uint8_t ret = 0;
	if(v68.opm_timera_counter >= v68.opm_timera_cycles) ret |= 0x01;
	if(v68.opm_timerb_counter >= v68.opm_timerb_cycles) ret |= 0x02;
	return ret;
}

void v68_periph_render(int32_t *bufL, int32_t *bufR, int nsamples) {
	verbose1("\033[32mperiph render %d\033[0m\n", nsamples);
	int32_t *buf[2] = { bufL, bufR };
	ym2151_update_one(&v68.opm, buf, nsamples);
}

uint32_t v68_dmac_read(uint32_t addr) {
	verbose2("DMAC READ %08x\n", addr);
	return v68.dmac_regs[addr & 0xff];
}

uint32_t v68_mfp_read(uint32_t addr) {
	verbose2("MFP READ %08x\n", addr);
	return 0xffffffff;
}

uint32_t v68_adpcm_read(uint32_t addr) {
	verbose2("ADPCM READ %08x\n", addr);
	return 0xffffffff;
}

uint32_t v68_ppi_read(uint32_t addr) {
	verbose2("PPI READ %08x\n", addr);
	return 0xffffffff;
}

void v68_dmac_write(uint32_t addr, uint32_t value) {
	verbose2("DMAC WRITE %08x = %04x\n", addr, value);
	v68.dmac_regs[addr & 0xff] = value;
}

void v68_mfp_write(uint32_t addr, uint32_t value) {
	verbose2("MFP WRITE %08x = %04x\n", addr, value);
}

void v68_adpcm_write(uint32_t addr, uint32_t value) {
	verbose2("ADPCM WRITE %08x = %04x\n", addr, value);
}

void v68_ppi_write(uint32_t addr, uint32_t value) {
	verbose2("PPI WRITE %08x = %04x\n", addr, value);
}

unsigned int v68_read_periph_32(unsigned int addr) {
	if(addr < 0xe82000) {
		/* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) {
		/* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) {
		/* ＤＭＡＣ ◆ DMA controller */
		return v68_dmac_read(addr & 0x0fff);
	} else if(addr < 0xe88000) {
		/* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) {
		/* ＭＦＰ ◆ Multi-function peripheral */
		return v68_mfp_read(addr & 0x0fff);
	} else if(addr < 0xe8c000) {
		/* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) {
		/* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) {
		/* システムポート ◆ System port */
	} else if(addr < 0xe92000) {
		/* ＦＭ音源 ◆ FM sound source (OPM) */
		switch(addr) {
			case 0xe90003:
				return v68.opm_flags & 0x03;
		}
	} else if(addr < 0xe94000) {
		/* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		return v68_adpcm_read(addr & 0x0fff);
	} else if(addr < 0xe96000) {
		/* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) {
		/* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) {
		/* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) {
		/* ｉ８２５５ ◆ Programmable peripheral interface */
		return v68_ppi_read(addr & 0x0fff);
	} else if(addr < 0xe9e000) {
		/* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	return 0xffffffff;
}

unsigned int v68_read_periph_16(unsigned int addr) {
	return v68_read_periph_32(addr);
}

unsigned int v68_read_periph_8(unsigned int addr) {
	return v68_read_periph_32(addr);
}

void v68_write_periph_32(unsigned int addr, unsigned int data) {
	printf("periph %x cycles=%d remaining=%d\n", addr, m68k_cycles_run(), m68k_cycles_remaining());
	int render_chippies = 0;
	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		v68_dmac_write(addr & 0x0fff, data);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		v68_mfp_write(addr & 0x0fff, data);
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		switch(addr) {
			case 0xe90001:
				v68_opm_write_addr(data);
				break;
			case 0xe90003:
				v68_opm_write_data(data);
				render_chippies = 1;
				break;
		}
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		v68_adpcm_write(addr & 0x0fff, data);
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		v68_ppi_write(addr & 0x0fff, data);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	if(render_chippies) {
		printf("render chippies %d - %d = %d\n", m68k_cycles_run(), v68.prev_sound_cycles, m68k_cycles_run() - v68.prev_sound_cycles);
		v68_render_tstates(m68k_cycles_run() - v68.prev_sound_cycles);
		v68.prev_sound_cycles = m68k_cycles_run();
	}

	if(v68.periph_timers_altered) {
		printf("\033[32mtimers altered at %d cycles, %d remaining\033[0m\n", m68k_cycles_run(), m68k_cycles_remaining());
		v68.periph_timers_altered = 0;
		v68.cpu_ended_timeslice = 1;
		m68k_end_timeslice();
	}
}

void v68_write_periph_16(unsigned int addr, unsigned int data) {
	v68_write_periph_32(addr, data & 0xffff);
}

void v68_write_periph_8(unsigned int addr, unsigned int data) {
	v68_write_periph_32(addr, data & 0xff);
}
