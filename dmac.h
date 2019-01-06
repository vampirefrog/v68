#pragma once

#include <stdint.h>

struct dmac_channel {
	uint8_t  csr; /* 0x00 R/W Channel Status Register    */
	uint8_t  cer; /* 0x01 R   Channel Error Register     */
	uint8_t  dcr; /* 0x04 R/W Device Control Register    */
	uint8_t  ocr; /* 0x05 R/W Operation control register */
	uint8_t  scr; /* 0x06 R/W Sequence control register  */
	uint8_t  ccr; /* 0x07 R/W Channel control register   */
	uint16_t mtc; /* 0x0a R/W Memory transfer counter    */
	uint32_t mar; /* 0x0c R/W Memory address register    */
	uint32_t dar; /* 0x14 R/W Device address register    */
	uint16_t btc; /* 0x1a R/W Base transfer counter      */
	uint32_t bar; /* 0x1c R/W Base address register      */
	uint8_t  niv; /* 0x25 R/W Normal interrupt vector    */
	uint8_t  eiv; /* 0x27 R/W Error interrupt vector     */
	uint8_t  mfc; /* 0x29 R/W Memory function code       */
	uint8_t  cpr; /* 0x2d R/W Channel priority register  */
	uint8_t  dfc; /* 0x31 R/W Device function code       */
	uint8_t  bfc; /* 0x39 R/W Base function code         */
	uint8_t  gcr; /* 0x3f R/W General control register   */
};

struct dmac {
	struct dmac_channel channels[4];
	int halted[4];
	int16_t transfer_size[4];
	int drq_state[4];
};

void dmac_init(void);
void dmac_write_16(uint32_t addr, uint16_t data, uint16_t mem_mask);
uint16_t dmac_read_16(uint32_t addr, uint16_t mem_mask);
void dmac_tick(int chan);
