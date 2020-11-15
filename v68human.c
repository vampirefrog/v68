#include <string.h>

#include "platform.h"

#include "v68human.h"
#include "v68io.h"
#include "v68.h"

#include "musashi/m68kcpu.h"

void v68_human_init() {
	verbose1("v68_human_init\n");

	v68.cmd_queue_pos = 0;
	v68.running = 0;

	uint16_t fake_human[] = {
#include "fake_human.inc"
	};

	for(int i = 0; i < sizeof(fake_human) / sizeof(fake_human[0]); i++) {
		v68.ram[HUMAN_HEAD + i * 2] = fake_human[i] >> 8;
		v68.ram[HUMAN_HEAD + i * 2 + 1] = fake_human[i] & 0xff;
	}

	v68.heap_start = v68.heap_top = STACK_TOP + STACK_SIZE;

	/* Init mem chain */
	m68k_write_memory_32(HUMAN_HEAD - 0x10 + 0x00, 0);
	m68k_write_memory_32(HUMAN_HEAD - 0x10 + 0x04, 0);
	m68k_write_memory_32(HUMAN_HEAD - 0x10 + 0x08, HUMAN_WORK);
	m68k_write_memory_32(HUMAN_HEAD - 0x10 + 0x0c, 0);
}

int v68_run_command(char *cmd) {
	verbose2("v68_run_command cmd=\"%s\"\n", cmd);

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

		int rd = v68_io_read(fd, xhead, sizeof(xhead));
		if(rd != sizeof(xhead)) {
			fprintf(stderr, "%s: Could not read %"PRIuSIZET" bytes for header\n", cmd, sizeof(xhead));
			v68_io_close(fd);
			return -1;
		}

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
			fprintf(stderr, "%s: Bound module list position exceeds end of file.\n", cmd);
			v68_io_close(fd);
			return -1;
		}

		uint32_t remaining = v68_mem_remaining();
		if(remaining < text_size + data_size + bss_size + 1024) {
			fprintf(stderr, "%s: Not enough memory to load executable\n", cmd);
			v68_io_close(fd);
			return -1;
		}

		verbose2("Allocating %dB\n", remaining);

		uint32_t m = v68_mem_alloc(remaining, 0);
		if(m > 0x8000000) {
			fprintf(stderr, "%s: Failed to allocate %d remaining bytes\n", cmd, remaining);
			v68_io_close(fd);
			return -1;
		}

		v68_mem_dump();

		// Read text and data
		rd = v68_io_read(fd, &v68.ram[m + 240], text_size);
		if(rd != text_size) {
			fprintf(stderr, "%s: Could not read %d bytes of .text\n", cmd, text_size);
			v68_io_close(fd);
			return -1;
		}
		rd = v68_io_read(fd, &v68.ram[m + 240 + text_size], data_size);
		if(rd != data_size) {
			fprintf(stderr, "%s: Could not read %d bytes of .data\n", cmd, data_size);
			v68_io_close(fd);
			return -1;
		}

		/* Relocate */
		int reloc_adj = (m + 240) - base_addr;
		if(reloc_size > 0 && reloc_adj != 0) {
			uint32_t text_loc = m + 240;
			for(int i = 0; i < reloc_size; i+=2) {
				uint8_t buf[4];
				int rd = v68_io_read(fd, buf, 2);
				if(rd != 2) {
					fprintf(stderr, "%s: EOF while reading relocation table\n", cmd);
					v68_mem_free(m, 0);
					v68_io_close(fd);
					return -1;
				}
				uint32_t r = (buf[0] << 8) | buf[1];
				if(r == 1) {
					i += 2;
					int rd = v68_io_read(fd, buf, 4);
					if(rd != 4) {
						fprintf(stderr, "%s: EOF while reading relocation table\n", cmd);
						v68_mem_free(m, 0);
						v68_io_close(fd);
						return -1;
					}
					r = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
					i += 2; // 4 total, with `for' increment
				}
				text_loc += r & 0xfffffffe;
				if(text_loc >= (m + 240) + text_size + data_size) {
					fprintf(stderr, "%s: Relocation out of bounds %08x + %04x = %08x >= (%08x + 240) + %08x\n", cmd, text_loc - (r & 0xfffffffe), r, text_loc, m, text_size);
					break;
				} else {
					if(r & 1) {
						// word
						int prev = (v68.ram[text_loc] << 8) | v68.ram[text_loc + 1];
						prev += reloc_adj;
						if(prev > 0xffff || prev < 0) {
							fprintf(stderr, "%s: Relocation word offset 0x%04x cannot fit adjustment.\n", cmd, prev);
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
							fprintf(stderr, "%s: Relocation long offset 0x%08x cannot fit adjustment.\n", cmd, prev);
							v68_mem_free(m, 0);
							v68_io_close(fd);
							return -1;
						}
						verbose3("  @%04x: %04x -> %08x  %08x -> %08x / %08"PRIxSIZET"\n", i, r, text_loc, prev - reloc_adj, prev, v68.ram_size);
						m68k_write_memory_32(text_loc, prev);
					}
				}
			}
		}

		memset(&v68.ram[m], 0, 240);

		/* 0x10: Environment vars pointer */
		m68k_write_memory_32(m + 0x00, ENV_TOP);
		/* 0x14: Return PC */
		if(v68.cur_prog_addr) {
			m68k_write_memory_32(m + 0x04, m68k_read_memory_32(v68.cur_prog_addr + 0x04));
		} else {
			m68k_write_memory_32(m + 0x04, m68k_get_reg(0, M68K_REG_PC));
		}
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
		verbose2("Entry point: 0x%08x + 0x%08x = 0x%08x cmd_queue_pos=%d\n", entry_point, reloc_adj, pc, v68.cmd_queue_pos);
		m68k_set_reg(M68K_REG_PC, pc);
		m68k_set_reg(M68K_REG_A4, pc); // program start addr
		// REG_USP = STACK_TOP + STACK_SIZE;
		// REG_ISP = STACK_TOP + STACK_SIZE;
		// REG_MSP = STACK_TOP + STACK_SIZE;
//		m68k_set_reg(M68K_REG_SR, m68k_get_reg(0, M68K_REG_SR) & ~0x2000);
		m68k_set_reg(M68K_REG_A7, STACK_TOP + STACK_SIZE);

		v68.running = 1;
		v68.cur_prog_addr = m;
	} else {
		// .R file
	}

	v68_io_close(fd);

	return 0;
}

int v68_queue_command(char *cmdline) {
	verbose1("v68_queue_command \"%s\"\n", cmdline);
	if(v68.cmd_queue_pos > V68_CMD_QUEUE_LEN) return -1;
	strncpy(v68.cmd_queue[v68.cmd_queue_pos], cmdline, sizeof(v68.cmd_queue[0]));
	v68.cmd_queue_pos++;
	verbose2("v68_queue_command cmd_queue_pos=%d\n", v68.cmd_queue_pos);

	return 0;
}

void v68_queue_next_command() {
	verbose1("v68_queue_next_command cmd_queue_pos=%d\n", v68.cmd_queue_pos);
	while(v68.cmd_queue_pos > 0) {
		if(v68_run_command(v68.cmd_queue[0])) {
			fprintf(stderr, "Could not run command \"%s\"\n", v68.cmd_queue[0]);
		}
		v68.cmd_queue_pos--;
		memcpy(&v68.cmd_queue[0], &v68.cmd_queue[1], sizeof(v68.cmd_queue[0]) * v68.cmd_queue_pos);
		return;
	}

	uint32_t pc = m68k_read_memory_32(v68.cur_prog_addr + 0x04);
	verbose2("v68_queue_next_command last command cur_prog_addr=%08x pc=%08x\n", v68.cur_prog_addr, pc);
	/* Last command, restore PC*/
	m68k_set_reg(M68K_REG_PC, pc);
	v68.running = 0;
}

int v68_env_append(char *env) {
	return 0;
	int l = strlen((char *)&v68.ram[ENV_TOP + 4]);

	if(l + strlen(env) + 1 > ENV_SIZE) return -1;

	char *ptr = (char *)&v68.ram[ENV_TOP] + (l ? l + 1 : 0);
	strncat(ptr, env, ENV_SIZE - l - 1);
	return 0;
}

int v68_env_set(char *var, char *value) {
	return 0;
	char buf[1024];
	snprintf(buf, sizeof(buf), "%s=%s\n", var, value);
	return v68_env_append(buf);
}

uint32_t v68_mem_alloc(int size, uint32_t parent_addr) {
	verbose2("v68_mem_alloc size=%d parent_addr=%08x\n", size, parent_addr);

	/* Align to 4 bytes */
	size = (size + 3) & 0xfffffc;

	uint32_t first = m68k_read_memory_32(HUMAN_HEAD - 0x04);

	uint32_t prev = first;
	if(first) {
		for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
			uint32_t cur_size = m68k_read_memory_32(cur + 0x08) - cur;
			uint32_t cur_parent = m68k_read_memory_32(cur + 0x04);
			if(cur_parent == 0xffffffff) {
				if(cur_size >= size) {
					m68k_write_memory_32(cur + 0x04, parent_addr);
					if(cur_size - size > 0x20) {
						m68k_write_memory_32(cur + 0x08, cur + 16 + size);
						m68k_write_memory_32(cur + 0x0c, cur + 16 + size);
						m68k_write_memory_32(cur + 16 + size + 0x00, cur);
						m68k_write_memory_32(cur + 16 + size + 0x04, 0xffffffff);
						m68k_write_memory_32(cur + 16 + size + 0x08, cur + cur_size);
						m68k_write_memory_32(cur + 16 + size + 0x0c, cur + cur_size);
					}
					return cur + 16;
				}
			}
			prev = cur;
		}
	}

	// Check if there is room at the top of the heap
	if(v68.heap_top + size + 16 > v68.ram_size) {
		verbose1("Could not allocate %08x bytes ram_size=%08"PRIxSIZET" heap_top=%08x\n", size, v68.ram_size, v68.heap_top);
		return -2;
	}

	if(!prev) prev = HUMAN_HEAD - 0x10;

	uint32_t next = v68.heap_top;
	m68k_write_memory_32(prev + 0x0c, next);
	m68k_write_memory_32(next, prev);
	m68k_write_memory_32(next + 0x04, parent_addr);
	m68k_write_memory_32(next + 0x08, next + 16 + size);
	m68k_write_memory_32(next + 0x0c, 0);
	v68.heap_top += 16 + size;

	v68_mem_dump();

	return next + 16;
}

int v68_mem_shrink(uint32_t addr, uint32_t new_size) {
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD - 0x04);

	addr -= 16;

	if(first) {
		for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
			// Yup. Found it.
			// I found your sliz.
			if(addr == cur) {
				uint32_t old_size = m68k_read_memory_32(addr + 0x08) - addr;
				if(new_size >= old_size)
					return -1;

				new_size = (new_size + 3) & 0xfffffc;

				uint32_t next_addr = m68k_read_memory_32(addr + 0x0c);
				m68k_write_memory_32(addr + 0x08, addr + 0x10 + new_size);
				if(next_addr) {
					if(old_size - new_size > 0x20) {
						uint32_t next = addr + new_size;
						m68k_write_memory_32(addr + 0x0c, next);
						m68k_write_memory_32(next, addr);
						m68k_write_memory_32(next + 0x04, 0xffffffff);
						m68k_write_memory_32(next + 0x08, next_addr);
						m68k_write_memory_32(next + 0x0c, next_addr);
						m68k_write_memory_32(next_addr, next);
					}
				} else {
					v68.heap_top = addr + 0x10 + new_size;
				}

				return 0;
			}
		}
	}

	return -1;
}

int v68_mem_free(uint32_t addr, uint32_t parent_addr) {
	verbose2("v68_mem_free addr=%08x parent_addr=%08x\n", addr, parent_addr);
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD - 0x04);

	v68_mem_dump();

	addr -= 16;

	if(first) {
		for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
			if(addr == cur) {
				// Last in line
				if(m68k_read_memory_32(cur + 0x0c) == 0) {
					verbose3("v68_mem_free  last in line\n");
					uint32_t prev = m68k_read_memory_32(cur);
					if(prev != HUMAN_HEAD) {
						// Previous block is also freed
						if(m68k_read_memory_32(prev + 0x04) == 0xffffffff) {
							cur = prev;
						}
					}
					m68k_write_memory_32(m68k_read_memory_32(cur) + 0x0c, 0);
					v68.heap_top = cur;
				} else {
					uint32_t end = m68k_read_memory_32(cur + 0x08);
					uint32_t prev = m68k_read_memory_32(cur);
					uint32_t next = m68k_read_memory_32(cur + 0x0c);
					verbose3("v68_mem_free  end=%08x prev=%08x next=%08x  %08x %08x %08x %08x\n", end, prev, next, m68k_read_memory_32(cur + 0x00), m68k_read_memory_32(cur + 0x04), m68k_read_memory_32(cur + 0x08), m68k_read_memory_32(cur + 0x0c));

					if(prev != HUMAN_HEAD) {
						// Previous block is also freed
						if(m68k_read_memory_32(prev + 0x04) == 0xffffffff) {
							cur = prev;
						}
					}

					if(next) {
						if(m68k_read_memory_32(next + 0x04) == 0xffffffff) {
							end = m68k_read_memory_32(next + 0x08);
							m68k_write_memory_32(end, cur);
						}
					}
					m68k_write_memory_32(cur + 0x04, 0xffffffff);
					m68k_write_memory_32(cur + 0x08, end);
					m68k_write_memory_32(cur + 0x0c, end);
				}
				return 0;
			}
		}
	}

	return -1;
}

uint32_t v68_mem_remaining() {
	uint32_t min = v68.heap_start;
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD - 0x04);
	for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
		uint32_t end = m68k_read_memory_32(cur + 0x08);
		if(end > min) min = end;
		if(cur > min) min = cur;
	}

	return v68.ram_size - min - 0x200;
}

void v68_mem_dump() {
	for(uint32_t cur = HUMAN_HEAD - 0x10; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
		verbose1("block @%06x addr=%06x prev=%06x parent=%06x end=%06x len=%d next=%06x\n",
			cur, cur + 16,
			m68k_read_memory_32(cur),
			m68k_read_memory_32(cur + 0x04),
			m68k_read_memory_32(cur + 0x08),
			m68k_read_memory_32(cur + 0x08) - cur - 16,
			m68k_read_memory_32(cur + 0x0c)
		);
	}
}
