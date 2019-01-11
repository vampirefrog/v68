#include <stdint.h>
#include "v68ipl.h"
#include "v68.h"

uint8_t iplrom[IPL_ROM_SIZE];

static void write_ipl_16(uint32_t addr, const uint16_t *data, int data_len) {
	uint8_t *p = iplrom + addr - IPL_ROM_START;
	const uint16_t *d = data;
	for(int i = 0; i < data_len; i++) {
		*p++ = *d >> 8;
		*p++ = *d & 0xff;
		d++;
	}
}

void v68_ipl_init() {
#define DMA3_VEC_END 0x6a
#define DMA3_VEC_ERR 0x6b
#define DMA3_INT_ADDR 0xff1940
	const uint16_t dma3_int[] = {
		/* DMA 3 interrupt handler */
		0x2f08,                         /* move.l a0, -(a7)           */
		0x41f9, 0x00e8, 0x40c0,         /* lea.l ($00e840c0), a0      */
		0x11e8, 0x0001, 0x0c33,         /* move.b ($0001,A0), ($0c33) */
		0x4a38, 0x0c32,                 /* tst.b ($0c32)              */
		0x6b18,                         /* bmi.b ($00FF196C)          */
		0x13fc, 0x0001, 0x00e9, 0xa007, /* move.b #$01, ($00e9a007)   */
		0x13fc, 0x0003, 0x00e9, 0xa007, /* move.b #$03, ($00e9a007)   */
		0x13fc, 0x0001, 0x00e9, 0x2001, /* move.b #$01, ($00e92001)   */
		0x4a10,                         /* tst.b (a0)                 */
		0x50d0,                         /* st (a0)                    */
		0x4238, 0x0c32,                 /* clr.b ($0c32)              */
		0x205f,                         /* movea.l (a7)+, a0          */
		0x4e73,                         /* rte                        */
	};

	write_ipl_16(DMA3_INT_ADDR, dma3_int, sizeof(dma3_int) / sizeof(dma3_int[0]));
	m68k_write_memory_32(DMA3_VEC_END * 4, DMA3_INT_ADDR);
	m68k_write_memory_32(DMA3_VEC_ERR * 4, DMA3_INT_ADDR);
}

uint8_t v68_ipl_read_8(uint32_t addr) {
	addr -= 0xfe0000;
	return iplrom[addr];
}

uint16_t v68_ipl_read_16(uint32_t addr) {
	addr -= 0xfe0000;
	return (iplrom[addr] << 8) | iplrom[addr + 1];
}

uint32_t v68_ipl_read_32(uint32_t addr) {
	addr -= 0xfe0000;
	return (iplrom[addr] << 24) | (iplrom[addr + 1] << 16) | (iplrom[addr + 2] << 8) | iplrom[addr + 3];
}
