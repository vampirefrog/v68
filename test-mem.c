#include <stdio.h>
#include "v68.h"
#include "v68mem.h"

int main(int argc, char **argv, char **envp) {

	struct v68 v;
	v68_init(16000000, 2 * 1024 * 1024, 44100);

	v68_mem_dump();

	uint32_t m1 = v68_mem_alloc(1024, 0);
	printf("Allocated 1024B @0x%08x\n", m1);
	v68_mem_dump();

	uint32_t m2 = v68_mem_alloc(4096, 0);
	printf("Allocated 4096B @0x%08x\n", m2);
	v68_mem_dump();

	uint32_t m3 = v68_mem_alloc(2048, 0);
	printf("Allocated 2048B @0x%08x\n", m3);
	v68_mem_dump();

	uint32_t f = v68_mem_free(m2, 0);
	printf("Freed 0x%06x %d\n", m2, f);
	v68_mem_dump();

	f = v68_mem_free(m1, 0);
	printf("Freed 0x%06x %d\n", m1, f);
	v68_mem_dump();

	uint32_t m4 = v68_mem_alloc(1024, 0);
	printf("Allocated 1024B @0x%08x\n", m4);
	v68_mem_dump();

	f = v68_mem_free(m1, 0);
	printf("Freed 0x%06x %d\n", m1, f);
	v68_mem_dump();

	f = v68_mem_free(m3, 0);
	printf("Freed 0x%06x %d\n", m3, f);
	v68_mem_dump();

	return 0;
}
