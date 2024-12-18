#include "platform.h"
#include "v68.h"
#include "v68periph.h"
#include "v68doscall.h"
#include "v68fecall.h"
#include "v68iocscall.h"
#include "musashi/m68kcpu.h"
#include "tools.h"

/* YM2151 OPM */
#define OPM_CLOCK 4000000

unsigned int v68_read_periph_32(unsigned int addr);
unsigned int v68_read_periph_16(unsigned int addr);
unsigned int v68_read_periph_8(unsigned int addr);
void v68_write_periph_32(unsigned int addr, unsigned int data);
void v68_write_periph_16(unsigned int addr, unsigned int data);
void v68_write_periph_8(unsigned int addr, unsigned int data);
void v68_opm_write_8(uint32_t addr, uint8_t data);
uint8_t v68_opm_read_8(uint32_t addr);
void v68_mfp_write_8(uint32_t addr, uint8_t value);
uint8_t v68_mfp_read_8(uint32_t addr);
void v68_adpcm_write_8(uint32_t addr, uint8_t value);
uint8_t v68_adpcm_read_8(uint32_t addr);
void v68_ppi_write_8(uint32_t addr, uint8_t value);
uint8_t v68_ppi_read_8(uint32_t addr);
void v68_emu_write_16(uint32_t addr, uint16_t data);

static uint32_t v68_opm_calc_timera(void);
static uint32_t v68_opm_calc_timerb(void);
static void v68_adpcm_update_clock_div(void);

void v68_periph_init() {
	verbose1("v68_periph_init\n");

	// for mapping $ff0000 at $000000 at startup
	v68.reset_pulsed = 0;
	ym2151_init(&v68.opm, OPM_CLOCK, v68.sample_rate);
	okim6258_init(&v68.oki, 8000000, FOSC_DIV_BY_512, TYPE_4BITS, OUTPUT_12BITS);
	dmac_init(&v68.dmac);
}

void v68_periph_deinit() {
	dmac_deinit(&v68.dmac);
	okim6258_stop(&v68.oki);
}

void v68_periph_reset() {
	// Memory controller
	v68.reset_pulsed = 1;

	// OPM reset
	ym2151_reset_chip(&v68.opm);
	v68.opm_clka = 0;
	v68.opm_clkb = 0;
	v68.opm_addr_latch = 0;
	v68.opm_timera_counter = 0;
	v68.opm_timerb_counter = 0;
	v68.opm_timera_cycles = v68_opm_calc_timera();
	v68.opm_timerb_cycles = v68_opm_calc_timerb();
	v68.opm_flags = 0;

	// OKI ADPCM reset
	okim6258_reset(&v68.oki);
	v68.oki_freq = 15625;
	v68.oki_sample_counter = 0;
	v68.oki_prev_sample[0] = v68.oki_prev_sample[1] = 0;
	v68_adpcm_update_clock_div();

	dmac_reset(&v68.dmac);
}

uint32_t v68_int_ack_handler(int int_level) {
	verbose1("v68_int_ack_handler int_level=%d int_vec=0x%02x\n", int_level, v68.int_vec);
	if(int_level == 6) { /* MFP */
		CPU_INT_LEVEL = 0;
		int vec = v68.int_vec;
		if(vec) {
			verbose2("v68_int_ack_handler  MFP int ack vec=0x%04x vector address at 0x%08x points to 0x%08x)\n", v68.int_vec, v68.int_vec * 4, m68k_read_memory_32(v68.int_vec * 4));
			v68.int_vec = 0;
			return vec;
		}
	} else if(int_level == 3) { /* DMAC */
		CPU_INT_LEVEL = 0;
		int vec = v68.int_vec;
		if(vec) {
			verbose2("v68_int_ack_handler DMAC int ack vec=0x%04x vector address at 0x%08x points to 0x%08x)\n", v68.int_vec, v68.int_vec * 4, m68k_read_memory_32(v68.int_vec * 4));
			v68.int_vec = 0;
			return vec;
		}
	}
	return M68K_INT_ACK_SPURIOUS;
}

void v68_periph_render_sample(int16_t *l, int16_t *r) {
	verbose1("v68_periph_render_sample\n");

#ifndef __EMSCRIPTEN__
	if(v68.logger) {
		vgm_logger_wait(v68.logger, 1);
	}
#endif

	int16_t *buf[2] = { l, r };
	ym2151_update_one(&v68.opm, buf, 1);

	int prev_oki_remainder = v68.oki_resample_remainder;
	int x = v68.oki_freq + v68.oki_resample_remainder;
	int oki_samples = x / v68.sample_rate;
	v68.oki_resample_remainder = x - oki_samples * v68.sample_rate;

	verbose2("v68_periph_render_samples  oki_samples=%d\n", oki_samples);
	if(oki_samples) {
		int16_t tl, tr;
		buf[0] = &tl;
		buf[1] = &tr;
		okim6258_update(&v68.oki, buf, oki_samples);
		v68.oki_prev_sample[0] = tl;
		v68.oki_prev_sample[1] = tr;

		int j = 0;
		int x = v68.oki_freq + prev_oki_remainder;
		int s = x / v68.sample_rate;
		prev_oki_remainder = x - s * v68.sample_rate;
		if(s > 0 && j < oki_samples - 1)
			j++;
		*l += tl;
		*r += tr;
	} else {
		*l += v68.oki_prev_sample[0];
		*r += v68.oki_prev_sample[1];
	}
}

void v68_periph_advance(uint32_t cycles) {
	verbose1("v68_periph_advance cycles=%d\n", cycles);
	verbose2("v68_periph_advance  opm_timera_counter=%d opm_timera_cycles=%d\n", v68.opm_timera_counter, v68.opm_timera_cycles);
	verbose2("v68_periph_advance  opm_timerb_counter=%d opm_timerb_cycles=%d\n", v68.opm_timerb_counter, v68.opm_timerb_cycles);
	verbose2("v68_periph_advance  oki_sample_counter=%d oki_sample_cycles=%d\n", v68.oki_sample_counter, v68.oki_sample_cycles);

	/* If timer A enabled */
	if((v68.opm_flags & 0x01) && v68.opm_timera_counter < v68.opm_timera_cycles) {
		v68.opm_timera_counter += cycles;
		if(v68.opm_timera_counter >= v68.opm_timera_cycles && (v68.opm_flags & 0x04)) {
			v68.int_vec = 0x43;
			verbose2("v68_periph_advance  generating IRQ 6 Timer A\n");
			m68k_set_irq(6);
		}
	}

	/* If timer B enabled */
	if((v68.opm_flags & 0x02) && v68.opm_timerb_counter < v68.opm_timerb_cycles) {
		v68.opm_timerb_counter += cycles;
		if(v68.opm_timerb_counter >= v68.opm_timerb_cycles && (v68.opm_flags & 0x08)) {
			v68.int_vec = 0x43;
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
			dmac_channel_request(&v68.dmac, 3);
		}
	}

	verbose2("v68_periph_advance done\n");
}

static void v68_periph_before_write(uint32_t addr) {
	/* Check if sound is touched */
	if(!v68.sound_touched) {
		if(addr == 0xe92001 || addr == 0xe92003 || addr == 0xe90003) {
			verbose1("sound touched\n");
			v68.sound_touched = 1;
		}
	}
}

unsigned int v68_read_periph_32(unsigned int addr) {
	if(addr < 0xe82000) { /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return (dmac_read_16(&v68.dmac, addr & 0xfe) << 16) | dmac_read_16(&v68.dmac, (addr & 0xfe) + 2);
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
	if(addr < 0xe82000) { /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return dmac_read_16(&v68.dmac, addr & 0xfe);
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
	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		return dmac_read_8(&v68.dmac, addr);
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

void v68_write_periph_32(unsigned int addr, unsigned int data) {
	verbose2("write_periph_32 addr=0x%08x data=0x%08x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_16(&v68.dmac, (addr & 0x0fff) + 0, data >> 16);
		dmac_write_16(&v68.dmac, (addr & 0x0fff) + 2, data);
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
	} else if(addr < 0xe9c000) { /* ｉ８２５５ ◆ Programmable peripheral interface */
		v68_ppi_write_8((addr & 0xff) + 0, (data >> 24) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 1, (data >> 16) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 2, (data >>  8) & 0xff);
		v68_ppi_write_8((addr & 0xff) + 3, (data      ) & 0xff);
	} else if(addr < 0xe9e000) { /* Ｉ／Ｏ コントローラ ◆ I/O controller */
	}
}

void v68_write_periph_16(unsigned int addr, unsigned int data) {
	verbose2("write_periph_16 addr=0x%08x data=0x%04x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_16(&v68.dmac, addr & 0xff, data);
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
	} else if(addr < 0xeb0000) { /* Emulator port */
		v68_emu_write_16(addr, data);
	}
}

void v68_write_periph_8(unsigned int addr, unsigned int data) {
	verbose1("v68_write_periph_8 addr=0x%08x data=0x%02x cycles=%d remaining=%d\n", addr, data, m68k_cycles_run(), m68k_cycles_remaining());

	v68_periph_before_write(addr);

	if(addr < 0xe82000) {        /* ＣＲＴコントローラ ◆ CRT controller */
	} else if(addr < 0xe84000) { /* ビデオコントローラ ◆ Video controller */
	} else if(addr < 0xe86000) { /* ＤＭＡＣ ◆ DMA controller */
		dmac_write_8(&v68.dmac, addr, data);
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
						}
						break;
					case 0x11:
						clka &= 0x03fc;
						clka |= data & 0x03;
						if(v68.opm_clka != clka) {
							v68.opm_clka = clka;
							v68.opm_timera_cycles = v68_opm_calc_timera();
						}
						break;
					case 0x12:
						clkb = data;
						if(v68.opm_clkb != clkb) {
							v68.opm_clkb = clkb;
							v68.opm_timerb_cycles = v68_opm_calc_timerb();
						}
						break;
					case 0x14: {
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
							v68.opm_ct = data & 0xc0;
							int clock = v68.opm_ct & 0x80 ? 4000000 : 8000000;
							verbose2("v68_opm_write_8 setting ADPCM Clock CT1=%d CT2=%d clock=%d\n", v68.opm_ct >> 7, v68.opm_ct >> 6 & 0x01, clock)
							okim6258_set_clock(&v68.oki, clock);
							v68_adpcm_update_clock_div();
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
	verbose2("v68_mfp_write_8 *STUB* 0x%08x = 0x%02x\n", addr, value);
}

uint8_t v68_mfp_read_8(uint32_t addr) {
	verbose2("v68_mfp_read_8 *STUB* %08x\n", addr);
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
				v68_adpcm_update_clock_div();
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

static void v68_adpcm_update_clock_div() {
	/* it's actually cpu_clock / (sample rate / 2), 1 byte contains 2 samples */
	//v68.oki_sample_cycles = v68.cpu_clock * 2 / 15625;
	uint64_t cycles = v68.cpu_clock;
	cycles *= v68.oki.divider;
	cycles *= 2;
	cycles /= v68.oki.master_clock;
	v68.oki_sample_cycles = cycles;
	v68.oki_freq = v68.oki.master_clock / v68.oki.divider;
	verbose2("v68_adpcm_update_clock_div cpu_clock=%d divider=%d oki_clock=%d oki_sample_cycles=%d oki_freq=%d\n", v68.cpu_clock, v68.oki.divider, v68.oki.master_clock, v68.oki_sample_cycles, v68.oki_freq);
}

/* ｉ８２５５ ◆ Programmable peripheral interface */
void v68_ppi_write_8(uint32_t addr, uint8_t value) {
	if(addr == 0x05) {
		uint8_t new_oki_div = value >> 2 & 0x03;
		okim6258_set_divider(&v68.oki, new_oki_div);
		v68_adpcm_update_clock_div();
		verbose2("v68_ppi_write_8 Port C %02x -> %02x\n", v68.ppi_port_c, value);
		v68.ppi_port_c = value;
	} else if(addr == 0x07) {
		if(value & 0x80) {
			verbose2("v68_ppi_ctr %02x -> %02x\n", v68.ppi_ctrl, value & 0x7f);
			v68.ppi_ctrl = value & 0x7f;
		} else {
			uint8_t port_c = v68.ppi_port_c;
			uint8_t bitsel = (value >> 1) & 0x07;
			uint8_t bit = 1 << bitsel;
			if(value & 0x01) {
				port_c = port_c | bit;
			} else {
				port_c = port_c & ~bit;
			}
			verbose2("v68_ppi_write_8 Port C %02x -> %02x\n", v68.ppi_port_c, port_c);

			if(bitsel >= 2 && bitsel <= 3) {
				uint8_t new_oki_div = (port_c >> 2) & 0x03;
				verbose2("v68_ppi_write_8 setting ADPCM divider %d\n", new_oki_div)
				okim6258_set_divider(&v68.oki, new_oki_div);
				v68_adpcm_update_clock_div();
			}

			v68.ppi_port_c = port_c;
		}

	}
	verbose2("v68_ppi_write_8 0x%08x = 0x%02x\n", addr, value);
}

uint8_t v68_ppi_read_8(uint32_t addr) {
	verbose2("v68_ppi_read_8 0x%08x\n", addr);
	if(addr == 0x05) {
		return v68.ppi_port_c;
	}

	return 0x00;
}

/* Emulator specific ports */
void v68_emu_write_16(uint32_t addr, uint16_t data) {
	verbose1("v68_emu_write_16 addr=0x%08x data=0x%04x\n", addr, data);
	if(addr == 0xea0000) {
		if((data & 0xff00) == 0xff00)
			v68_dos_call(data);
		else if((data & 0xff00) == 0xfe00)
			v68_fe_call(data);
	} else if(addr == 0xea0004) {
		v68_iocs_call(data);
	} else if(addr == 0xea0008) {
		v68_queue_next_command();
	}
}
