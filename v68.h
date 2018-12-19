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
#include "vgm.h"

#define XHEAD_SIZE      0x40        /* Xファイルのヘッダサイズ */
#define HUMAN_HEAD      0x6800      /* Humanのメモリ管理ブロック位置 */
#define FCB_WORK        0x20F00     /* DOSCALL GETFCB用ワーク領域 */
#define HUMAN_WORK      0x21000     /* 割り込み処理先等のワーク領域 */
#define TRAP0_WORK      0x20FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP1_WORK      0x21FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP2_WORK      0x22FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP3_WORK      0x23FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP4_WORK      0x24FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP5_WORK      0x25FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP6_WORK      0x26FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP7_WORK      0x27FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define TRAP8_WORK      0x28FF0000  /* TRAP割り込み処理先等のワーク領域 */
#define ENV_TOP         0x21C00
#define ENV_SIZE        0x2000
#define STACK_TOP       ENV_TOP + ENV_SIZE
#define STACK_SIZE      0x10000     /* 64KB */
#define MB_SIZE         16
#define PSP_SIZE        MB_SIZE + 240
#define PROG_TOP        (STACK_TOP + STACK_SIZE + PSP_SIZE)
#define NEST_MAX        20
#define FILE_MAX        20

#define RAS_INTERVAL    10000   /* ラスタ割り込みの間隔 */

#define verbose(v, fmt...) { if(v68.verbosity >= v) printf(fmt); }
#define verbose1(fmt...) verbose(1, fmt);
#define verbose2(fmt...) verbose(2, fmt);
#define verbose3(fmt...) verbose(3, fmt);
#define verbose4(fmt...) verbose(4, fmt);
#define logcall(fmt...) { if(v68.log_calls) printf(fmt); }

struct v68 {
	uint8_t *ram;
	size_t ram_size;

	/* Memory allocation */
	int heap_start, heap_top;

	struct v68_file_info file_info[FILE_MAX];

#define V68_CMD_QUEUE_LEN 8
	char cmd_queue[V68_CMD_QUEUE_LEN][1024];
	int cmd_queue_pos;
	int running;

	/* MFP */
	int mfp_vec;

	/* Sound Timing */
	int32_t *bufL, *bufR;
	int sample_rate;
	int samples_remainder, buf_remaining;
	int prev_sound_cycles, remaining_tstates;

	/* CPU timing */
	int cpu_clock, cpu_cycle_remainder, cpu_ended_timeslice;

	/* OPM */
	struct ym2151 opm;
	uint8_t opm_addr_latch;
	int opm_clka, opm_clkb, opm_flags, opm_ct;
	int opm_timera_cycles, opm_timerb_cycles;
	int opm_timera_counter, opm_timerb_counter;
	int periph_timers_altered;

	/* ADPCM */
	struct okim6258 oki;
	int oki_sample_counter, oki_sample_cycles;

	/* 8255 PPI */
	uint8_t ppi_regs[4];

	/* DMAC */
	uint8_t dmac_regs[0x100];

	/* VGM logging */
	struct vgm_logger *logger;

	uint32_t cur_prog_addr; // address of currently running program

	int log_calls, log_dasm, verbosity;
};

extern struct v68 v68;

int v68_init(int clock, int ram_size, int sample_rate);
int v68_shutdown();
int v68_fill_buffer(int32_t *bufL, int32_t *bufR, int samples);

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
int v68_render_tstates(int tstates);
