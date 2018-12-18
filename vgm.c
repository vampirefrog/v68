#include "vgm.h"

static const uint8_t header[] = {
	0x56, 0x67, 0x6d, 0x20, 0x4a, 0x83, 0x01, 0x00, 0x51, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x9a, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x09, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int vgm_logger_begin(struct vgm_logger *l, char *filename) {
	l->f = fopen(filename, "wb");
	if(!l->f) return -1;

	fwrite(header, 1, sizeof(header), l->f);

	l->last_wait = 0;
	l->size = 0;

	return 0;
}

void vgm_logger_wait(struct vgm_logger *l, int samples) {
	l->last_wait += samples;
	l->total_samples += samples;
}

void vgm_logger_write_wait(struct vgm_logger *l, int samples) {
	if(samples <= 0) return;

	uint8_t buf[3];
	int len = 0;

	if(samples == 735) {
		buf[0] = 0x62;
		len = 1;
	} else if(samples == 882) {
		buf[0] = 0x63;
		len = 1;
	} else if(samples <= 16) {
		buf[0] = 0x70 + samples - 1;
		len = 1;
	} else {
		while(samples > 65535) {
			buf[0] = 0x61;
			buf[1] = 0xff;
			buf[2] = 0xff;
			fwrite(buf, 1, 3, l->f);
			l->size += 3;
			samples -= 65535;
		}
		buf[0] = 0x61;
		buf[1] = samples & 0xff;
		buf[2] = samples >> 8;
		len = 3;
	}

	fwrite(buf, 1, len, l->f);
	l->size += len;
}

void vgm_logger_write(struct vgm_logger *l, uint8_t *buf, int len) {
	if(l->last_wait) {
		vgm_logger_write_wait(l, l->last_wait);
		l->last_wait = 0;
	}

	fwrite(buf, 1, len, l->f);
	l->size += len;
}

void vgm_logger_write_ay(struct vgm_logger *l, uint8_t reg, uint8_t data) {
	uint8_t buf[3];
	buf[0] = 0xa0;
	buf[1] = reg;
	buf[2] = data;

	vgm_logger_write(l, buf, 3);
}

void vgm_logger_write_ym2151(struct vgm_logger *l, uint8_t reg, uint8_t data) {
	uint8_t buf[3];
	buf[0] = 0x54;
	buf[1] = reg;
	buf[2] = data;

	vgm_logger_write(l, buf, 3);
}

void vgm_logger_end(struct vgm_logger *l) {
	if(l->last_wait)
		vgm_logger_write_wait(l, l->last_wait);

	fseek(l->f, 0x04, SEEK_SET);
	l->size += 0x80 - 4;
	fwrite(&l->size, 1, 4, l->f);
	fseek(l->f, 0x18, SEEK_SET);
	fwrite(&l->total_samples, 1, 4, l->f);
	fclose(l->f);
}
