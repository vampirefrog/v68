#include "v68.h"
#include "v68periph.h"
#include "musashi/m68kcpu.h"
#include "tools.h"

/* YM2151 OPM */
#define OPM_CLOCK 4000000
static uint32_t v68_opm_calc_timera(void);
static uint32_t v68_opm_calc_timerb(void);

void v68_periph_init() {
	verbose1("v68_periph_init\n");
	v68.periph_timers_altered = 0;

	ym2151_init(&v68.opm, OPM_CLOCK, v68.sample_rate);
	ym2151_reset_chip(&v68.opm);
	v68.opm_clka = 0;
	v68.opm_clkb = 0;
	v68.opm_addr_latch = 0;
	v68.opm_timera_counter = 0;
	v68.opm_timerb_counter = 0;
	v68.opm_timera_cycles = v68_opm_calc_timera();
	v68.opm_timerb_cycles = v68_opm_calc_timerb();
	v68.opm_flags = 0;

	okim6258_init(&v68.oki, 8000000, FOSC_DIV_BY_512, TYPE_4BITS, OUTPUT_12BITS);
	okim6258_reset(&v68.oki);
	v68.oki_freq = 15625;
	v68.oki_sample_counter = 0;
	v68.oki_sample_cycles = v68.cpu_clock * 2 / 15625; /* it's actually clock / (sample rate / 2), 1 byte contains 2 samples */

	dmac_init();
	dmac_write_16(0xd4, 0xe92003 >> 16, 0xffff);
	dmac_write_16(0xd6, 0xe92003 & 0xffff, 0xffff);
	dmac_write_16(0xc6, 0x0400, 0xffff);
}

void v68_periph_end() {
	okim6258_stop(&v68.oki);
}

uint32_t v68_periph_next_int(uint32_t cycles) {
	verbose1("v68_periph_next_int cycles=%d\n", cycles);
	uint32_t next_int = cycles;

	if((v68.opm_flags & 0x05) && (v68.opm_timera_counter < v68.opm_timera_cycles) && (v68.opm_timera_cycles - v68.opm_timera_counter) < next_int) {
		next_int = v68.opm_timera_cycles - v68.opm_timera_counter;
	}

	if((v68.opm_flags & 0x0a) && (v68.opm_timerb_counter < v68.opm_timerb_cycles) && (v68.opm_timerb_cycles - v68.opm_timerb_counter) < next_int) {
		next_int = v68.opm_timerb_cycles - v68.opm_timerb_counter;
	}

	verbose2("v68_periph_next_int returning %d\n", next_int);
	return next_int;
}

uint32_t v68_int_ack_handler(int int_level) {
	verbose1("v68_int_ack_handler %d\n", int_level);
	if(int_level == 6) {
		CPU_INT_LEVEL = 0;
		int vec = v68.mfp_vec;
		if(vec) { // MFP
			verbose2("v68_int_ack_handler  MFP int ack vec=0x%04x vector address at 0x%08x points to 0x%08x)\n", v68.mfp_vec, v68.mfp_vec * 4, m68k_read_memory_32(v68.mfp_vec * 4));
			v68.mfp_vec = 0;
			return vec;
		}
	}
	return M68K_INT_ACK_SPURIOUS;
}

void v68_periph_render(int16_t *bufL, int16_t *bufR, int samples) {
	verbose1("v68_periph_render samples=%d\n", samples);

#ifndef __EMSCRIPTEN__
	if(v68.logger) {
		vgm_logger_wait(v68.logger, samples);
	}
#endif

	int16_t *buf[2] = { bufL, bufR };
	ym2151_update_one(&v68.opm, buf, samples);

	int16_t tmpBufL[1024], tmpBufR[1024];
	int16_t *tmpBuf[2] = { tmpBufL, tmpBufR };

	int x = (samples * v68.oki_freq + v68.oki_resample_remainder);
	int oki_samples = x / v68.sample_rate;
	v68.oki_resample_remainder = x - oki_samples * v68.sample_rate;

	while(oki_samples > 0) {
		int oki_render_samples = MIN(1024, oki_samples);
		verbose2("v68_periph_render  oki_samples=%d oki_render_samples=%d\n", oki_samples, oki_render_samples);
		okim6258_update(&v68.oki, tmpBuf, oki_render_samples);
		for(int i = 0; i < samples; i++) {
			int j = i * v68.oki_freq / v68.sample_rate;
			*bufL++ += tmpBufL[j];
			*bufR++ += tmpBufR[j];
		}
		oki_samples -= oki_render_samples;
	}
}

int v68_render_tstates(int tstates) {
	verbose1("v68_render_tstates tstates=%d\n", tstates);
	int64_t x = (int64_t)tstates * (int64_t)v68.sample_rate + v68.samples_remainder;
	int64_t samples = x / (int64_t)v68.cpu_clock;
	if(samples > v68.buf_remaining)
		samples = v68.buf_remaining;
	v68.samples_remainder = x - samples * v68.cpu_clock;
	verbose2("v68_render_tstates tstates=%d samples_remainder=%d buf_remaining=%d samples=%ld\n", tstates, v68.samples_remainder, v68.buf_remaining, samples);

	if(samples > 0) {
		v68_periph_render(v68.bufL, v68.bufR, samples);
		v68.bufL += samples;
		v68.bufR += samples;
		v68.buf_remaining -= samples;
	}

	return samples;
}

void v68_periph_advance(uint32_t cycles) {
	v68.in_periph_timing = 1;

	verbose1("v68_periph_advance cycles=%d\n", cycles);
	verbose2("v68_periph_advance  opm_timera_counter=%d opm_timera_cycles=%d\n", v68.opm_timera_counter, v68.opm_timera_cycles);
	verbose2("v68_periph_advance  opm_timerb_counter=%d opm_timerb_cycles=%d\n", v68.opm_timerb_counter, v68.opm_timerb_cycles);
	verbose2("v68_periph_advance  oki_sample_counter=%d oki_sample_cycles=%d\n", v68.oki_sample_counter, v68.oki_sample_cycles);

	/* If timer A enabled */
	if((v68.opm_flags & 0x01) && v68.opm_timera_counter < v68.opm_timera_cycles) {
		v68.opm_timera_counter += cycles;
		if(v68.opm_timera_counter >= v68.opm_timera_cycles && (v68.opm_flags & 0x04)) {
			v68.mfp_vec = 0x43;
			verbose2("v68_periph_advance  generating IRQ 6 Timer A\n");
			m68k_set_irq(6);
		}
	}

	/* If timer B enabled */
	if((v68.opm_flags & 0x02) && v68.opm_timerb_counter < v68.opm_timerb_cycles) {
		v68.opm_timerb_counter += cycles;
		if(v68.opm_timerb_counter >= v68.opm_timerb_cycles && (v68.opm_flags & 0x08)) {
			v68.mfp_vec = 0x43;
			verbose2("v68_periph_advance generating IRQ 6 Timer B\n");
			m68k_set_irq(6);
		}
	}

	/* If OKI is playing, trigger a DMA transfer */
	if(v68.oki.status & 0x02) {
		v68.oki_sample_counter += cycles;
		while(v68.oki_sample_counter >= v68.oki_sample_cycles) {
			verbose2("v68_periph_advance  DMA Tick oki_sample_counter=%d oki_sample_cycles=%d\n", v68.oki_sample_counter, v68.oki_sample_cycles);
			v68.oki_sample_counter -= v68.oki_sample_cycles;
			// Trigger transfer
			v68.periph_cycles += v68.oki_sample_cycles;
			dmac_tick(3);
		}
	}

	v68_render_tstates(v68.periph_cycles - v68.prev_sound_cycles);
	v68.prev_sound_cycles = v68.periph_cycles;

	verbose2("v68_periph_advance done periph_cycles=%d\n", v68.periph_cycles);

	v68.in_periph_timing = 0;
}

/* ＦＭ音源 ◆ FM sound source (OPM) */
static uint32_t v68_opm_calc_timera() {
	uint64_t x = 1024 * v68.opm_clka;
	x *= v68.cpu_clock;
	x /= 62500;
	return x;
}

static uint32_t v68_opm_calc_timerb() {
	uint64_t x = 1024 * (256 - v68.opm_clkb);
	x *= v68.cpu_clock;
	x /= 4000000;
	return x;
}

void v68_opm_write_8(uint32_t addr, uint8_t data) {
	verbose1("v68_opm_write_8 0x%08x 0x%02x\n", addr, data);
	switch(addr) {
		case 0x01:
			v68.opm_addr_latch = data;
			break;
		case 0x03: {
				verbose2("v68_opm_write_8  DATA opm_addr_latch=0x%02x data=0x%02x\n", v68.opm_addr_latch, data);
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
							v68.opm_clka = clka;
							v68.opm_timera_cycles = v68_opm_calc_timera();
							v68.periph_timers_altered = 1;
						}
						break;
					case 0x11:
						clka &= 0x03fc;
						clka |= data & 0x03;
						if(v68.opm_clka != clka) {
							v68.opm_clka = clka;
							v68.opm_timera_cycles = v68_opm_calc_timera();
							v68.periph_timers_altered = 1;
						}
						break;
					case 0x12:
						clkb = data;
						if(v68.opm_clkb != clkb) {
							v68.opm_clkb = clkb;
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
			break;
	}
}

uint8_t v68_opm_read_8(uint32_t addr) {
	verbose1("v68_opm_read_8 addr=0x%08x\n", addr);
	switch(addr) {
		case 0x03: {
			uint8_t ret = 0;
			if(v68.opm_timera_counter >= v68.opm_timera_cycles) ret |= 0x01;
			if(v68.opm_timerb_counter >= v68.opm_timerb_cycles) ret |= 0x02;
			return ret;
		}
	}
	return 0x00;
}

/* ＭＦＰ ◆ Multi-function peripheral */
void v68_mfp_write_8(uint32_t addr, uint8_t value) {
	verbose2("v68_mfp_write_8 0x%08x = 0x%02x\n", addr, value);
}

uint8_t v68_mfp_read_8(uint32_t addr) {
	verbose2("v68_mfp_read_8 %08x\n", addr);
	return 0x00;
}

/* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
void v68_adpcm_write_8(uint32_t addr, uint8_t value) {
	verbose1("v68_adpcm_write_8 addr=0x%08x value=0x%02x\n", addr, value);
	switch(addr) {
		case 0x01: /* 0xe92001 ADPCM command */
			okim6258_write(&v68.oki, 0x00, value);
#ifndef __EMSCRIPTEN__
			if(v68.logger)
				vgm_logger_write_okim6258(v68.logger, 0x00, value);
#endif
			if(value & 0x02) {
				v68.periph_timers_altered = 1;
				v68.oki_freq = 15625;
				v68.oki_sample_cycles = v68.cpu_clock * 2 / v68.oki_freq;
			}
			break;
		case 0x03: /* 0xe92003 Data input / output */
			okim6258_write(&v68.oki, 0x01, value);
#ifndef __EMSCRIPTEN__
			if(v68.logger)
				vgm_logger_write_okim6258(v68.logger, 0x01, value);
#endif
			break;
	}
}

uint8_t v68_adpcm_read_8(uint32_t addr) {
	verbose1("v68_adpcm_read_8 0x%08x\n", addr);
	return 0x00;
}

/* ｉ８２５５ ◆ Programmable peripheral interface */
void v68_ppi_write_8(uint32_t addr, uint8_t value) {
	verbose2("v68_ppi_write_8 0x%08x = 0x%02x\n", addr, value);
}

uint8_t v68_ppi_read_8(uint32_t addr) {
	verbose2("v68_ppi_read_8 0x%08x\n", addr);
	return 0x00;
}

static void v68_periph_before_read(uint32_t addr) {
	(void)addr;
	verbose1("v68_periph_before_read addr=0x%08x prev_sound_cycles=%d in_periph_timing=%d\n", addr, v68.prev_sound_cycles, v68.in_periph_timing);

	if(v68.in_periph_timing) {
		v68_render_tstates(v68.periph_cycles - v68.prev_sound_cycles);
		v68.prev_sound_cycles = v68.periph_cycles;
	} else {
		v68.periph_cycles = v68.prev_sound_cycles;
		v68_periph_advance(m68k_cycles_run() - v68.prev_sound_cycles);
		v68.prev_sound_cycles = m68k_cycles_run();
	}
}

unsigned int v68_read_periph_32(unsigned int addr) {
	v68_periph_before_read(addr);
	if(addr < 0xe82000) { /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return (dmac_read_16(addr & 0xfe, 0xffff) << 16) | dmac_read_16((addr & 0xfe) + 2, 0xffff);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		return
			(v68_mfp_read_8((addr & 0xff) + 0) << 24) |
			(v68_mfp_read_8((addr & 0xff) + 1) << 16) |
			(v68_mfp_read_8((addr & 0xff) + 2) <<  8) |
			(v68_mfp_read_8((addr & 0xff) + 3)      );
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		return
			(v68_opm_read_8((addr & 0xff) + 0) << 24) |
			(v68_opm_read_8((addr & 0xff) + 1) << 16) |
			(v68_opm_read_8((addr & 0xff) + 2) <<  8) |
			(v68_opm_read_8((addr & 0xff) + 3)      );
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		return
			(v68_adpcm_read_8((addr & 0xff) + 0) << 24) |
			(v68_adpcm_read_8((addr & 0xff) + 1) << 16) |
			(v68_adpcm_read_8((addr & 0xff) + 2) <<  8) |
			(v68_adpcm_read_8((addr & 0xff) + 3)      );
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		return
			(v68_ppi_read_8((addr & 0xff) + 0) << 24) |
			(v68_ppi_read_8((addr & 0xff) + 1) << 16) |
			(v68_ppi_read_8((addr & 0xff) + 2) <<  8) |
			(v68_ppi_read_8((addr & 0xff) + 3)      );
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	return 0xffffffff;
}

unsigned int v68_read_periph_16(unsigned int addr) {
	v68_periph_before_read(addr);
	if(addr < 0xe82000) { /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return dmac_read_16(addr & 0xfe, 0xffff);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		return
			(v68_mfp_read_8((addr & 0xff) + 0) <<  8) |
			(v68_mfp_read_8((addr & 0xff) + 1)      );
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		return
			(v68_opm_read_8((addr & 0xff) + 0) <<  8) |
			(v68_opm_read_8((addr & 0xff) + 1)      );
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		return
			(v68_adpcm_read_8((addr & 0xff) + 0) <<  8) |
			(v68_adpcm_read_8((addr & 0xff) + 1)      );
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		return
			(v68_ppi_read_8((addr & 0xff) + 0) <<  8) |
			(v68_ppi_read_8((addr & 0xff) + 1)      );
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	return 0xffff;
}

unsigned int v68_read_periph_8(unsigned int addr) {
	v68_periph_before_read(addr);
	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return dmac_read_16(addr & 0xfe, (addr & 1) ? 0x00ff : 0xff00);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		return v68_mfp_read_8(addr & 0xff);
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		return v68_opm_read_8(addr & 0xff);
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		return v68_adpcm_read_8(addr & 0xff);
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		return v68_ppi_read_8(addr & 0xff);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	return 0xff;
}

static void v68_periph_before_write(uint32_t addr) {
	/* Check for OKIM6258 or Y2151 writes */
	// if(addr == 0xe92001 || addr == 0xe92003 || addr == 0xe90003) {
		verbose1("v68_periph_before_write addr=0x%08x in_periph_timing=%d prev_sound_cycles=%d periph_cycles=%d\n", addr, v68.in_periph_timing, v68.prev_sound_cycles, v68.periph_cycles);
		v68_periph_before_read(addr);
	// }
}

static void v68_periph_after_write(void) {
	if(v68.periph_timers_altered) {
		verbose1("timers altered at %d cycles, %d remaining\n", m68k_cycles_run(), m68k_cycles_remaining());
		v68.periph_timers_altered = 0;
		v68.cpu_ended_timeslice = 1;
		m68k_end_timeslice();
	}
}

void v68_write_periph_32(unsigned int addr, unsigned int data) {
	verbose2("write_periph_32 addr=0x%08x data=0x%08x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_16((addr & 0x0fff) + 0, data >> 16, 0xffff);
		dmac_write_16((addr & 0x0fff) + 2, data, 0xffff);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		v68_mfp_write_8((addr & 0xff) + 0, (data >> 24) & 0xff);
		v68_mfp_write_8((addr & 0xff) + 1, (data >> 16) & 0xff);
		v68_mfp_write_8((addr & 0xff) + 2, (data >>  8) & 0xff);
		v68_mfp_write_8((addr & 0xff) + 3, (data      ) & 0xff);
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		v68_opm_write_8((addr & 0xff) + 0, (data >> 24) & 0xff);
		v68_opm_write_8((addr & 0xff) + 1, (data >> 16) & 0xff);
		v68_opm_write_8((addr & 0xff) + 2, (data >>  8) & 0xff);
		v68_opm_write_8((addr & 0xff) + 3, (data      ) & 0xff);
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		v68_adpcm_write_8((addr & 0xff) + 0, (data >> 24) & 0xff);
		v68_adpcm_write_8((addr & 0xff) + 1, (data >> 16) & 0xff);
		v68_adpcm_write_8((addr & 0xff) + 2, (data >>  8) & 0xff);
		v68_adpcm_write_8((addr & 0xff) + 3, (data      ) & 0xff);
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		v68_ppi_write_8((addr & 0xff) + 0, (data >> 24) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 1, (data >> 16) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 2, (data >>  8) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 3, (data      ) & 0xff);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	v68_periph_after_write();
}

void v68_write_periph_16(unsigned int addr, unsigned int data) {
	verbose2("write_periph_16 addr=0x%08x data=0x%04x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_16(addr & 0xff, data, 0xffff);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		v68_mfp_write_8((addr & 0xff) + 0, (data >> 8) & 0xff);
		v68_mfp_write_8((addr & 0xff) + 1, (data     ) & 0xff);
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		v68_opm_write_8((addr & 0xff) + 0, (data >> 8) & 0xff);
		v68_opm_write_8((addr & 0xff) + 1, (data     ) & 0xff);
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		v68_adpcm_write_8((addr & 0xff) + 0, (data >> 8) & 0xff);
		v68_adpcm_write_8((addr & 0xff) + 1, (data     ) & 0xff);
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		v68_ppi_write_8((addr & 0xff) + 0, (data >> 8) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 1, (data     ) & 0xff);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	v68_periph_after_write();
}

void v68_write_periph_8(unsigned int addr, unsigned int data) {
	verbose2("v68_write_periph_8 addr=0x%08x data=0x%02x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_16(addr & 0xfe, (addr & 1) ? data : (data << 8), (addr & 1) ? 0x00ff : 0xff00);
	} else if(addr < 0xe88000) { /* エリアセット ◆ Area set */
	} else if(addr < 0xe8a000) { /* ＭＦＰ ◆ Multi-function peripheral */
		v68_mfp_write_8(addr & 0xff, data);
	} else if(addr < 0xe8c000) { /* ＲＴＣ ◆ Realtime clock */
	} else if(addr < 0xe8e000) { /* プリンタ ◆ Printer */
	} else if(addr < 0xe90000) { /* システムポート ◆ System port */
	} else if(addr < 0xe92000) { /* ＦＭ音源 ◆ FM sound source (OPM) */
		v68_opm_write_8(addr & 0xff, data);
	} else if(addr < 0xe94000) { /* ＡＤＰＣＭ ◆ OKI M6258 ADPCM */
		v68_adpcm_write_8(addr & 0xff, data);
	} else if(addr < 0xe96000) { /* ＦＤＣ ◆ Floppy drive controller */
	} else if(addr < 0xe98000) { /* ＨＤＣ ◆ Hard disk controller */
	} else if(addr < 0xe9a000) { /* ＳＣＣ ◆ Serial communications */
	} else if(addr < 0xe9C000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		v68_ppi_write_8(addr & 0xff, data);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}

	v68_periph_after_write();
}
