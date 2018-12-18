#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#ifndef __EMSCRIPTEN__
#include <zlib.h>
#endif /* __EMSCRIPTEN */
#include "tools.h"

uint8_t *load_file(const char *filename, size_t *size_out) {
	FILE *f = fopen(filename, "rb");
	if(!f) {
		fprintf(
			stderr,
			"Could not open %s: %s (%d)\n",
			filename,
			strerror(errno),
			errno
		);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	uint8_t *data = malloc(size);
	if(!data) {
		fprintf(
			stderr,
			"Could not allocate %lu bytes for %s: %s (%d)\n",
			size, filename, strerror(errno), errno
		);
		fclose(f);
		return NULL;
	}
	rewind(f);
	fread(data, 1, size, f);
	fclose(f);

	if(size_out) *size_out = size;
	return data;
}

int gcd(int a, int b) {
	int c = a % b;

	while(c > 0) {
		a = b;
		b = c;
		c = a % b;
	}

	return b;
}

void csv_quote(char *str, size_t len) {
	if(len == 0) len = strlen(str);

	if(str == 0) {
		putchar('\\');
		putchar('N');
		return;
	}

	putchar('"');
	for(int i = 0; i < len; i++) {
		switch(str[i]) {
			case 0:
				putchar('\\');
				putchar(0);
				break;
			case '\\':
				putchar('\\');
				putchar('\\');
				break;
			case '\b':
				putchar('\\');
				putchar('b');
				break;
			case '\n':
				putchar('\\');
				putchar('n');
				break;
			case '\r':
				putchar('\\');
				putchar('r');
				break;
			case '\t':
				putchar('\\');
				putchar('t');
				break;
			case 26:
				putchar('\\');
				putchar('Z');
				break;
			case '"':
				putchar('"');
				putchar('"');
				break;
			default: putchar(str[i]);
		}
	}
	putchar('"');
}
