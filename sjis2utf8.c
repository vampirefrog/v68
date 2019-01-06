#include "sjis.h"
#include "utf8.h"

void put_utf8(FILE *f, int c) {
	char buf[4];
	int chars = utf8_encode(c, buf);
	if(chars) {
		fwrite(buf, chars, 1, f);
	}
}

int main(int argc, char **argv) {
	FILE *in = stdin, *out = stdout;
	if(argc > 1) in = fopen(argv[1], "rb");
	if(argc > 2) out = fopen(argv[2], "rb");

	int last_byte = 0;
	int b = 0;
	while((b = fgetc(in)) != EOF) {
		if(SJIS_FIRST_CHAR(b)) {
			last_byte = b;
		} else {
			put_utf8(out, sjis_char_to_unicode((last_byte << 8) | b));
			last_byte = 0;
		}
	}

	return 0;
}
