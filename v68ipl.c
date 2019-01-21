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
	const uint16_t fake_ipl[] = {
#include "fake_ipl.inc"
	};

	write_ipl_16(0xff0000, fake_ipl, sizeof(fake_ipl) / sizeof(fake_ipl[0]));
	/* IOCS work setting */
	m68k_write_memory_16(0x970, 79);       /* Number of columns on screen -1 */
	m68k_write_memory_16(0x972, 24);       /* Number of lines on screen -1 */
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
