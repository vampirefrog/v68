#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include "v68human.h"
#include "okim6258.h"
#include "ym2151.h"
#include "dmac.h"

#include "vgm.h"

#define XHEAD_SIZE      0x40        /* Xファイルのヘッダサイズ */
#define HUMAN_HEAD      0x6800      /* Humanのメモリ管理ブロック位置 */
#define FCB_WORK        0x20F00     /* DOSCALL GETFCB用ワーク領域 */
#define HUMAN_WORK      0x21000     /* 割り込み処理先等のワーク領域 */
#define ENV_TOP         0x21C00
#define ENV_SIZE        0x2000
#define STACK_TOP       ENV_TOP + ENV_SIZE
#define STACK_SIZE      0x10000     /* 64KB */
#define MB_SIZE         16
#define PSP_SIZE        MB_SIZE + 240
#define PROG_TOP        (STACK_TOP + STACK_SIZE + PSP_SIZE)
#define NEST_MAX        20

#define RAS_INTERVAL    10000   /* ラスタ割り込みの間隔 */

#define verbose(v, fmt...) { if(v68.verbosity >= v) printf(fmt); }
#define verbose1(fmt...) verbose(1, "[1] " fmt);
#define verbose2(fmt...) verbose(2, "[2]  " fmt);
#define verbose3(fmt...) verbose(3, "[3]   " fmt);
#define verbose4(fmt...) verbose(4, "[4]    " fmt);
#define logcall(fmt...) { if(v68.log_calls) printf(fmt); }

struct v68 {
	int log_dasm, verbosity;

	/* VM */
	uint8_t *ram;
	size_t ram_size;
	int reset_pulsed;

	/* MFP */
	int int_vec;

	/* CPU timing */
	int cpu_clock, cpu_cycle_remainder;

	/* Sound Timing */
	int sound_touched;
	int sample_rate;

	/* OPM */
	struct ym2151 opm;
	uint8_t opm_addr_latch;
	int opm_clka, opm_clkb, opm_flags, opm_ct;
	int opm_timera_cycles, opm_timerb_cycles;
	int opm_timera_counter, opm_timerb_counter;

	/* ADPCM */
	struct okim6258 oki;
	int oki_sample_counter, oki_sample_cycles;
	int oki_resample_remainder, oki_freq;

	/* 8255 PPI */
	uint8_t ppi_regs[4];

	/* DMAC */
	struct dmac dmac;

	/* VGM logging */
	struct vgm_logger *logger;

	/* OS */
	/* Memory allocation */
	int heap_start, heap_top;

#define V68_CMD_QUEUE_LEN 8
	int running;
	char cmd_queue[V68_CMD_QUEUE_LEN][1024];
	int cmd_queue_pos;
	uint32_t cur_prog_addr; // address of currently running program
	int log_calls;
};

extern struct v68 v68;

int v68_init(int clock, int ram_size, int sample_rate);
void v68_boot(void);
void v68_run(void);
int v68_fill_buffer(int samples, int16_t *bufL, int16_t *bufR);
int v68_shutdown(void);

/* Traps */
int v68_trap(int which);
uint32_t v68_int_ack_handler(int int_level);

unsigned int m68k_read_memory_8(unsigned int addr);
unsigned int m68k_read_memory_16(unsigned int addr);
unsigned int m68k_read_memory_32(unsigned int addr);
void m68k_write_memory_8(unsigned int addr, unsigned int data);
void m68k_write_memory_16(unsigned int addr, unsigned int data);
void m68k_write_memory_32(unsigned int addr, unsigned int data);
unsigned int m68k_read_disassembler_8  (unsigned int addr);
unsigned int m68k_read_disassembler_16 (unsigned int addr);
unsigned int m68k_read_disassembler_32 (unsigned int addr);
uint32_t v68_int_ack_handler(int int_level);
int v68_trap(int which);
