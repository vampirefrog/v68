#pragma once

#define IPL_ROM_START 0xfe0000
#define IPL_ROM_SIZE 128 * 1024 /* 128kB */
extern uint8_t iplrom[IPL_ROM_SIZE];

void v68_ipl_init(void);
uint8_t v68_ipl_read_8(uint32_t addr);
uint16_t v68_ipl_read_16(uint32_t addr);
uint32_t v68_ipl_read_32(uint32_t addr);
