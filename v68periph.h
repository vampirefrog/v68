#pragma once

#include <stdint.h>

void v68_periph_init(void);
void v68_periph_render(int32_t *bufL, int32_t *bufR, int nsamples);
uint32_t v68_periph_next_int(uint32_t tstates);
void v68_periph_advance_timers(uint32_t cycles);

unsigned int v68_read_periph_8(unsigned int addr);
unsigned int v68_read_periph_16(unsigned int addr);
unsigned int v68_read_periph_32(unsigned int addr);
void v68_write_periph_8(unsigned int addr, unsigned int data);
void v68_write_periph_16(unsigned int addr, unsigned int data);
void v68_write_periph_32(unsigned int addr, unsigned int data);
