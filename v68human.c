#include <string.h>

#include "v68human.h"
#include "v68io.h"
#include "v68mem.h"
#include "v68.h"

#include "musashi/m68kcpu.h"


void v68_human_init() {
	v68.log_calls = 0;

	/* Init file info */
	memset(v68.file_info, 0, sizeof(v68.file_info));
	v68.file_info[0].fh   = stdin;
	v68.file_info[0].mode = 0;
	v68.file_info[1].fh   = stdout;
	v68.file_info[1].mode = 1;
	v68.file_info[2].fh   = stderr;
	v68.file_info[2].mode = 1;

	v68.cmd_queue_pos = 0;
}

int v68_run_command(char *cmd) {
	verbose2("run command %s\n", cmd);

	v68.running = 1;

	// Find first space, or \0
	char *c = cmd;
	while(*c && !isspace(*c)) {
		c++;
	}

	if(*c)
		*c++ = 0;

	int cmd_len = strlen(cmd);

	// move past any space, on to the args
	if(c) while(*c && isspace(*c)) c++;

	int l = strlen(c);
	if(l > 255) l = 255;

	// cmdline args are copied here
	v68.ram[STACK_TOP] = l;
	strncpy((char *)&v68.ram[STACK_TOP + 1], c, 255);
	v68.ram[STACK_TOP + 1 + l] = 0;

	int fd = v68_io_open(cmd, O_RDONLY);
	if(fd < 0) {
		fprintf(stderr, "Could not open %s\n", cmd);
		return -1;
	}

	off_t o = v68_io_seek(fd, 0, SEEK_END);
	v68_io_seek(fd, 0, SEEK_SET);

	if(cmd_len > 2 && cmd[cmd_len - 2] == '.' && (cmd[cmd_len - 1] == 'x' || cmd[cmd_len - 1] == 'X')) {
		// .X files
		uint8_t xhead[0x40];

		v68_io_read(fd, xhead, sizeof(xhead));

		if(xhead[0] != 'H' && xhead[1] != 'U') {
			fprintf(stderr, "%s: X file signature invalid, not \"HU\"\n", cmd);
			v68_io_close(fd);
			return -1;
		}

#define READ_LONG(x) ((xhead[x] << 24) | (xhead[x + 1] << 16) | (xhead[x + 2] << 8) | xhead[x + 3])
#define READ_SHORT(x) ((xhead[x] << 8) | xhead[x + 1])
		uint8_t load_mode = xhead[3];
		(void)load_mode;
		uint32_t base_addr = READ_LONG(0x04);
		uint32_t entry_point = READ_LONG(0x08);
		verbose3("base_addr=%08x entry_point=%08x\n", base_addr, entry_point);
		uint32_t text_size = READ_LONG(0x0c);
		if(text_size > 0 && 0x40 + text_size > o) {
			fprintf(stderr, "%s: .text section size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t data_size = READ_LONG(0x10);
		if(data_size > 0 && 0x40 + text_size + data_size > o) {
			fprintf(stderr, "%s: .data section size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t bss_size = READ_LONG(0x14);
		uint32_t reloc_size = READ_LONG(0x18);
		if(reloc_size > 0 && 0x40 + text_size + data_size + reloc_size > o) {
			fprintf(stderr, "%s: Relocation data size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t sym_table_size = READ_LONG(0x1c);
		if(sym_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size > o) {
			fprintf(stderr, "%s: Symbol table size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t scd_line_num_table_size = READ_LONG(0x20);
		if(scd_line_num_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size > o) {
			fprintf(stderr, "%s: SCD line number table size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t scd_sym_table_size = READ_LONG(0x24);
		if(scd_sym_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size + scd_sym_table_size > o) {
			fprintf(stderr, "%s: SCD symbol table size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t scd_string_table_size = READ_LONG(0x28);
		if(scd_string_table_size > 0 && 0x40 + text_size + data_size + reloc_size + sym_table_size + scd_line_num_table_size + scd_sym_table_size + scd_string_table_size > o) {
			fprintf(stderr, "%s: SCD string table size exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}
		uint32_t bound_pos = READ_LONG(0x3c);
		if(bound_pos && bound_pos >= o) {
			fprintf(stderr, "Bound module list position exceeds end of file.\n");
			v68_io_close(fd);
			return -1;
		}

		uint32_t remaining = v68_mem_remaining();
		if(remaining < text_size + data_size + bss_size + 1024) {
			fprintf(stderr, "Not enough memory\n");
			v68_io_close(fd);
			return -1;
		}

		verbose2("Allocating %dB\n", remaining);

		uint32_t m = v68_mem_alloc(remaining, 0);
		if(m > 0x8000000) {
			verbose2("Failed to allocate %d remaining bytes\n", remaining);
			v68_io_close(fd);
			return -1;
		}

		v68_mem_dump();

		// Read text and data
		v68_io_read(fd, &v68.ram[m + 240], text_size + data_size);

		int reloc_adj = (m + 240) - base_addr;
		if(reloc_size > 0 && reloc_adj != 0) {
			verbose2("Relocating %dB\n", reloc_size);
			uint32_t text_loc = m + 240;
			for(int i = 0; i < reloc_size; i+=2) {
				uint8_t buf[4];
				v68_io_read(fd, buf, 2);
				uint32_t r = (buf[0] << 8) | buf[1];
				if(r == 1) {
					i += 2;
					v68_io_read(fd, buf, 4);
					r = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
					i += 2; // 4 total, with `for' increment
				}
				text_loc += r & 0xfffffffe;
				if(text_loc >= (m + 240) + text_size + data_size) {
					fprintf(stderr, "Relocation out of bounds %08x + %04x = %08x >= (%08x + 240) + %08x\n", text_loc - (r & 0xfffffffe), r, text_loc, m, text_size);
					break;
				} else {
					if(r & 1) {
						// word
						int prev = (v68.ram[text_loc] << 8) | v68.ram[text_loc + 1];
						prev += reloc_adj;
						if(prev > 0xffff || prev < 0) {
							fprintf(stderr, "Relocation word offset cannot fit adjustment.\n");
							v68_mem_free(m, 0);
							v68_io_close(fd);
							return -1;
						}
						m68k_write_memory_16(text_loc, prev);
					} else {
						// long
						int prev = (v68.ram[text_loc] << 24) | (v68.ram[text_loc + 1] << 16) | (v68.ram[text_loc + 2] << 8) | v68.ram[text_loc + 3];
						prev += reloc_adj;
						if(prev > 0x00ffffff || prev < 0) {
							fprintf(stderr, "Relocation word offset cannot fit adjustment.\n");
							v68_mem_free(m, 0);
							v68_io_close(fd);
							return -1;
						}

						verbose3("  @%04x: %04x -> %08x  %08x -> %08x / %08lx\n", i, r, text_loc, prev - reloc_adj, prev, v68.ram_size);
						m68k_write_memory_32(text_loc, prev);
					}
				}
			}
		}

		// mem_ptr = prog_ptr + ra[0];
		memset(&v68.ram[m], 0, 240);

		/* 0x10: Environment vars pointer */
		m68k_write_memory_32(m + 0x00, ENV_TOP);
		/* 0x20: Command line pointer */
		m68k_write_memory_32(m + 0x10, STACK_TOP);
		/* 0x30: Start address of BSS */
		m68k_write_memory_32(m + 0x20, m + o + 240);
		/* 0x34: Start address of heap (same as BSS) */
		m68k_write_memory_32(m + 0x24, m + o + 240);
		/* 0x38: Initial stack address */
		m68k_write_memory_32(m + 0x28, m + remaining); /* Stack top is at end of heap */
		/* 0x44: Parent process SR */
		m68k_write_memory_32(m + 0x34, m68ki_get_sr());
		/* 0x60: Shell activtation flag */
		m68k_write_memory_32(m + 0x50, 0);
		/* Write drive, path and filename at 0x80, 0x82 and 0xc4 */
		// if ( set_fname( fname, ra [ 0 ] ) == FALSE )
			// return( FALSE );

		// set registers
		m68k_set_reg(M68K_REG_A0, m - 16); // memory management ptr
		m68k_set_reg(M68K_REG_A1, m + 240 + o + bss_size); // end of loaded program + 1
		m68k_set_reg(M68K_REG_A2, STACK_TOP); // cmdline
		m68k_set_reg(M68K_REG_A3, ENV_TOP); // env vars
		uint32_t pc = entry_point + reloc_adj;
		verbose2("Entry point: 0x%08x + 0x%08x = 0x%08x\n", entry_point, reloc_adj, pc);
		m68k_set_reg(M68K_REG_PC, pc);
		m68k_set_reg(M68K_REG_A4, pc); // program start addr
		m68k_set_reg(M68K_REG_A7, STACK_TOP + STACK_SIZE);
		REG_USP = STACK_TOP + STACK_SIZE;
		REG_ISP = STACK_TOP + STACK_SIZE;
		REG_MSP = STACK_TOP + STACK_SIZE;
		FLAG_S = 0;

		v68.cur_prog_addr = m;
	} else {
		// .R file
	}

	v68_io_close(fd);

	return 0;
}

int v68_queue_command(char *cmdline) {
	if(v68.cmd_queue_pos > V68_CMD_QUEUE_LEN) return -1;

	strncpy(v68.cmd_queue[v68.cmd_queue_pos], cmdline, sizeof(v68.cmd_queue[0]));
	v68.cmd_queue_pos++;

	if(!v68.running)
		v68_queue_next_command();

	return 0;
}

void v68_queue_next_command() {
	if(v68.cmd_queue_pos > 0) {
		v68_run_command(v68.cmd_queue[0]);
		v68.cmd_queue_pos--;
		memcpy(&v68.cmd_queue[0], &v68.cmd_queue[1], sizeof(v68.cmd_queue[0]) * v68.cmd_queue_pos);
	} else {
		/* Last command, HALT */
		m68k_set_reg(M68K_REG_SR, 0x2000);
		m68k_set_reg(M68K_REG_PC, HUMAN_WORK + 4);
	}
}

int v68_env_append(char *env) {
	int l = strlen((char *)&v68.ram[ENV_TOP]);

	if(l + strlen(env) + 1 > ENV_SIZE) return -1;

	char *ptr = (char *)&v68.ram[ENV_TOP] + (l ? l + 1 : 0);
	strncat(ptr, env, ENV_SIZE - l - 1);
	return 0;
}

int v68_env_set(char *var, char *value) {
	char buf[1024];
	snprintf(buf, sizeof(buf), "%s=%s\n", var, value);
	return v68_env_append(buf);
}
