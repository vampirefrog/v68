#include <stdio.h>
#include "tools.h"

int main(int argc, char **argv) {
	size_t len = 0;
	uint8_t *data = load_file(argv[1], &len);
	#define READ_LONG(x) ((data[x] << 24) | (data[x + 1] << 16) | (data[x + 2] << 8) | data[x + 3])
	uint32_t text_size = READ_LONG(0x0c);
	for(int i = 0; i < text_size; i+=2) {
		if(i % 16 == 0 && i > 0) printf("\n");
		printf("0x%02x%02x, ", data[0x40 + i], data[0x41 + i]);
	}
	printf("\n");
	free(data);
	return 0;
}
