#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
	uint8_t u8 = 1;
	int8_t i8 = 2;
	uint16_t u16 = 3;
	int16_t i16 = 4;
	uint32_t u32 = 5;
	int32_t i32 = 6;
	uint64_t u64 = 7;
	int64_t i64 = 8;

	size_t s = 7;
	char c = 8;
	unsigned char u = 9;
	short sh = 10;
	unsigned short ush = 11;
	long l = 12;
	unsigned long ul = 13;

	int i = 14;
	unsigned int ui = 15;

	printf("uint8_t  %d\n", u8);
	printf(" int8_t  %d\n", i8);
	printf("uint16_t %d\n", u16);
	printf(" int16_t %d\n", i16);
	printf("uint32_t %d\n", u32);
	printf(" int32_t %d\n", i32);
	printf("uint64_t %d\n", u64);
	printf(" int64_t %d\n", i64);

	printf("size_t %d\n", s);

	printf("char   %d\n", c);
	printf("u char %d\n", u);
	printf("short  %d\n", sh);
	printf("u short %d\n", ush);
	printf("long   %d\n", l);
	printf("u long %d\n", ul);
	printf("int    %d\n", i);
	printf("unsigned int %d\n", ui);

	return 0;
}
