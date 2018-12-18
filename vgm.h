#pragma once

#include <stdio.h>
#include <stdint.h>

struct vgm_logger {
	FILE *f;

	int last_wait, total_samples;
	size_t size;
};

int vgm_logger_begin(struct vgm_logger *, char *);
void vgm_logger_wait(struct vgm_logger *, int samples);
void vgm_logger_write_ay(struct vgm_logger *, uint8_t reg, uint8_t data);
void vgm_logger_write_ym2151(struct vgm_logger *, uint8_t reg, uint8_t data);
void vgm_logger_end(struct vgm_logger *);

