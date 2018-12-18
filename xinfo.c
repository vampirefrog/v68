#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"
#include "md5.h"

int opt_csv = 0;
int opt_hexdump = 0;

uint8_t safechar(uint8_t c) {
	return c >= 0x20 && c < 0x7f ? c : '.';
}

void hexdump(uint8_t *data, uint32_t base, uint32_t len) {
	for(int i = 0; i < len; i += 16) {
		printf("0x%08x: ", base + i);
		for(int j = 0; j < 16; j++) {
			if(i + j < len)
				printf("%02x ", data[i + j]);
			else
				printf("   ");
		}
		for(int j = 0; j < 16; j++) {
			if(i + j < len)
				printf("%c", safechar(data[i + j]));
			else
				printf(" ");
		}
		printf("\n");
	}
}

static void hexdump_csv(uint8_t *data, int len) {
	for(int i = 0; i < len; i++) {
		printf("%02x", data[i]);
	}
}

const char *load_mode_name(uint8_t mode) {
	const char *load_mode_names[3] = { "Normal", "Minimum block", "High address" };
	if(mode >= 0 && mode <= 2) return load_mode_names[mode];
	return "Unknown";
}

void xinfo(char *filename) {
	FILE *f = fopen(filename, "rb");
	if(!f) {
		perror(filename);
		return;
	}

	fseek(f, 0, SEEK_END);
	long o = ftell(f);
	fseek(f, 0, SEEK_SET);

	uint8_t *buf = malloc(o);
	if(!buf) {
		fprintf(stderr, "%s: Could not allocate %ld bytes\n", filename, o);
		goto err1;
	}

	if(fread(buf, o, 1, f) < 1) {
		fprintf(stderr, "%s: Could not read %lu bytes.\n", filename, o);
		goto err1;
	}

	if(buf[0] != 'H' && buf[1] != 'U') {
		fprintf(stderr, "%s: X file signature invalid, not \"HU\"\n", filename);
		goto err1;
	}

#define READ_LONG(x) ((buf[x] << 24) | (buf[x + 1] << 16) | (buf[x + 2] << 8) | buf[x + 3])
#define READ_SHORT(x) ((buf[x] << 8) | buf[x + 1])
	uint8_t load_mode = buf[3];
	uint32_t base_addr = READ_LONG(0x04);
	uint32_t entry_point = READ_LONG(0x08);
	uint32_t text_size = READ_LONG(0x0c);
	if(text_size > 0 && 0x40 + text_size > o) {
		fprintf(stderr, "%s: .text section size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t data_size = READ_LONG(0x10);
	if(data_size > 0 && 0x40 + text_size + data_size > o) {
		fprintf(stderr, "%s: .data section size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t bss_size = READ_LONG(0x14);
	uint32_t reloc_size = READ_LONG(0x18);
	if(reloc_size > 0 && 0x40 + text_size + data_size + reloc_size > o) {
		fprintf(stderr, "%s: Relocation data size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t sym_table_size = READ_LONG(0x1c);
	if(sym_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size > o) {
		fprintf(stderr, "%s: Symbol table size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t scd_line_num_table_size = READ_LONG(0x20);
	if(scd_line_num_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size > o) {
		fprintf(stderr, "%s: SCD line number table size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t scd_sym_table_size = READ_LONG(0x24);
	if(scd_sym_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size + scd_sym_table_size > o) {
		fprintf(stderr, "%s: SCD symbol table size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t scd_string_table_size = READ_LONG(0x28);
	if(scd_string_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size + scd_sym_table_size + scd_string_table_size > o) {
		fprintf(stderr, "%s: SCD string table size exceeds end of file.\n", filename);
		goto err1;
	}
	uint32_t bound_pos = READ_LONG(0x3c);
	if(bound_pos && bound_pos >= o) {
		fprintf(stderr, "Bound module list position exceeds end of file.\n");
		goto err1;
	}

	struct md5_ctx ctx;
	md5_init_ctx(&ctx);
	md5_process_bytes(buf, o, &ctx);
	uint8_t md5buf[16];
	md5_finish_ctx(&ctx, md5buf);

	if(opt_csv) {
		printf("%s\t%lu", filename, o);

		// MD5
		printf("\t");
		for(int j = 0; j < 16; j++) {
			printf("%02x", md5buf[j]);
		}

		printf("\t%d\t%s", load_mode, load_mode_name(load_mode));
		printf("\t0x%08x\t0x%08x\t0x%08x\t0x%08x\t0x%08x\t0x%08x\t0x%08x", base_addr, entry_point, text_size, data_size, bss_size, reloc_size, sym_table_size);
		printf("\t0x%08x\t0x%08x\t0x%08x\t0x%08x", scd_line_num_table_size, scd_sym_table_size, scd_string_table_size, bound_pos);
		if(opt_hexdump) {
			printf("\t");
			hexdump_csv(buf, 0x40);
			printf("\t");
			hexdump_csv(buf + 0x40, text_size);
			printf("\t");
			hexdump_csv(buf + 0x40 + text_size, data_size);
			printf("\t");
			hexdump_csv(buf + 0x40 + text_size + data_size, reloc_size);
		}
		printf("\n");
	} else {
		printf("%s: %luB ", filename, o);
		for(int j = 0; j < 16; j++) {
			printf("%02x", md5buf[j]);
		}
		printf("\n");


		printf("Header:\n");

		if(opt_hexdump)
			hexdump(buf, 0, 0x40);

		printf("  Load mode: %s (%d)\n", load_mode_name(load_mode), load_mode);

		printf("  Base address:               0x%08x\n", base_addr);
		printf("  Entry point:                0x%08x\n", entry_point);
		printf("  Text section size:          0x%08x (%dB)\n", text_size, text_size);
		printf("  Data section size:          0x%08x (%dB)\n", data_size, data_size);
		printf("  BSS size:                   0x%08x (%dB)\n", bss_size, bss_size);
		printf("  Relocation table size:      0x%08x (%dB)\n", reloc_size, reloc_size);
		printf("  Symbol table size:          0x%08x (%dB)\n", sym_table_size, sym_table_size);
		printf("  SCD line number table size: 0x%08x (%dB)\n", scd_line_num_table_size, scd_line_num_table_size);
		printf("  SCD symbol table size:      0x%08x (%dB)\n", scd_sym_table_size, scd_sym_table_size);
		printf("  SCD string table size:      0x%08x (%dB)\n", scd_string_table_size, scd_string_table_size);
		printf("  Bound module list:          0x%08x\n", bound_pos);

		if(opt_hexdump) {
			printf(".text (%dB):\n", text_size);
			if(0x40 + text_size > o) {
				hexdump(buf + 0x40, 0x40, o - 0x40);
			} else {
				hexdump(buf + 0x40, 0x40, text_size);
			}

			printf(".data (%dB):\n", data_size);
			if(0x40 + text_size + data_size > o) {
				hexdump(buf + 0x40 + text_size, 0x40 + text_size, o - (0x40 + text_size));
			} else {
				hexdump(buf + 0x40 + text_size, 0x40 + text_size, data_size);
			}

			printf("Relocation (%dB):\n", reloc_size);
			if(0x40 + text_size + data_size + reloc_size > o) {
				hexdump(buf + 0x40 + text_size + data_size, 0x40 + text_size + data_size, o - (0x40 + text_size + data_size));
			} else {
				hexdump(buf + 0x40 + text_size + data_size, 0x40 + text_size + data_size, reloc_size);
			}
		}

		if(reloc_size > 0) {
			printf("Relocation:\n");
			uint32_t text_loc = 0x40;
			for(int i = 0; i < reloc_size; i+=2) {
				uint32_t r = READ_SHORT(0x40 + text_size + data_size + i);
				if(r == 1) {
					i += 2;
					r = READ_LONG(0x40 + text_size + data_size + i);
					i += 2; // 4 total, with `for' increment
				}
				text_loc += r & 0xfffffffe;
				if(text_loc >= 0x40 + text_size) {
					fprintf(stderr, "%s: Relocation out of bounds.\n", filename);
					break;
				} else {
					if(r & 1) {
						printf(" @%04x: %04x -> %08x (%04x)\n", i, r, text_loc, READ_SHORT(text_loc));
					} else {
						printf(" @%04x: %04x -> %08x (%08x)\n", i, r, text_loc, READ_LONG(text_loc));
					}
				}
			}
		}

		if(bound_pos > 0) {
			printf("Bound modules:\n");
			char fname[32];
			for(int i = bound_pos; i < o; i += 0x20) {
				memset(fname, 0, sizeof(fname) / sizeof(fname[0]));
				int f = 0;
				for(int j = i; j < i + 8; j++) {
					if(buf[j] > 0x20) fname[f++] = buf[j];
				}
				fname[f++] = '.';
				for(int j = i + 8; j < i + 8 + 3; j++) {
					if(buf[j] > 0x20) fname[f++] = buf[j];
				}

				uint32_t next_pos = i + 0x40 > o ? bound_pos : READ_LONG(i + 0x20 + 0x1C);
				uint32_t module_pos = READ_LONG(i + 0x1C);

				printf("%-12s %c%c%c%c @0x%08x % 6d ", fname, buf[i+11] & 0x20 ? 'A' : '-', buf[i+11] & 0x40 ? 'S' : '-', buf[i+11] & 0x02 ? 'H' : '-', buf[i+11] & 0x01 ? 'R' : '-', module_pos, next_pos - module_pos);
				printf("%04d-%02d-%02d %d:%02d:%02d\n", 1980 + ((buf[i+0x19] >> 1) & 0x7f), ((buf[i+0x19] & 0x01) << 3) | (buf[i+0x18] >> 5), buf[i+0x18] & 0x1f, buf[i+0x17] >> 3, ((buf[i+0x17] & 0x07) << 3) | (buf[i + 0x16] >> 5), (buf[i+0x16] & 0x1f) << 1);
			}
		}
	}

err1:
	free(buf);
	fclose(f);
}

int main(int argc, char **argv) {
	int optind = cmdline_parse_args(argc, argv, (struct cmdline_option[]){
		{
			'c', "csv",
			"CSV format mode",
			0,
			TYPE_SWITCH,
			TYPE_INT, &opt_csv
		},
		{
			'x', "hexdump",
			"Dump data as hex",
			0,
			TYPE_SWITCH,
			TYPE_INT, &opt_hexdump
		},
		CMDLINE_ARG_TERMINATOR
	}, 1, 0, "<file.x> [<file.x> ...]");

	if(optind < 0) exit(-optind);

	if(opt_csv) {
		printf("File\tSize\tMD5\tLoad mode\tLoad mode name\tBase addr\tEntry point\tText size\tData size\tBSS size\tReloc size\tSym table size\tSCD line num table size\tSCD sym table size\tSCD string table size\tBound module pos");
		if(opt_hexdump)
			printf("\tHeader\t.text\t.data\tReloc data");
		printf("\n");
	}

	for(int i = optind; i < argc; i++) {
		xinfo(argv[i]);
	}

	return 0;
}
