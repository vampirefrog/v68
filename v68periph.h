#pragma once

#include <stdint.h>

#include "okim6258.h"
#include "ym2151.h"
#include "dmac.h"

void v68_periph_init(void);
void v68_periph_deinit(void);
void v68_periph_reset(void);
void v68_periph_render_sample(int16_t *l, int16_t *r);
void v68_periph_advance(uint32_t cycles);

unsigned int v68_read_periph_8(unsigned int addr);
unsigned int v68_read_periph_16(unsigned int addr);
unsigned int v68_read_periph_32(unsigned int addr);
void v68_write_periph_8(unsigned int addr, unsigned int data);
void v68_write_periph_16(unsigned int addr, unsigned int data);
void v68_write_periph_32(unsigned int addr, unsigned int data);
