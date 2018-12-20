#include "v68.h"
#include "v68mem.h"
#include "musashi/m68kcpu.h"

int v68_mem_init() {
	if(!v68.ram) return -1;

	/* Init trap vectors */
	FLAG_S = 1;

	/* Interrupts */
	m68k_write_memory_32(0x28, HUMAN_WORK);        /* A traps */
	m68k_write_memory_32(0x2C, HUMAN_WORK);        /* F traps */
	m68k_write_memory_32(0x80, TRAP0_WORK);        /* trap0 */
	m68k_write_memory_32(0x84, TRAP1_WORK);        /* trap1 */
	m68k_write_memory_32(0x88, TRAP2_WORK);        /* trap2 */
	m68k_write_memory_32(0x8C, TRAP3_WORK);        /* trap3 */
	m68k_write_memory_32(0x90, TRAP4_WORK);        /* trap4 */
	m68k_write_memory_32(0x94, TRAP5_WORK);        /* trap5 */
	m68k_write_memory_32(0x98, TRAP6_WORK);        /* trap6 */
	m68k_write_memory_32(0x9C, TRAP7_WORK);        /* trap7 */
	m68k_write_memory_32(0xA0, TRAP8_WORK);        /* trap8 */
	m68k_write_memory_32(0x118, 0);                /* vdisp */
	m68k_write_memory_32(0x138, 0);                /* crtc */
	m68k_write_memory_16(HUMAN_WORK    , 0x4e73);  /* 0x4e73 = rte */
	m68k_write_memory_16(HUMAN_WORK + 2, 0x4e75);  /* 0x4e75 = rts */
	m68k_write_memory_16(HUMAN_WORK + 4, 0x4e72);  /* 0x4e75 = stop */
	m68k_write_memory_16(HUMAN_WORK + 6, 0x2000);  /* 0x2000 = SR S=1 */
	m68k_write_memory_16(HUMAN_WORK + 8, 0x60fa);  /* 0x4e75 = bra -4. */

	/* IOCSコールベクタの設定 */
	for(int i = 0; i < 256; i ++ ) {
		m68k_write_memory_32(0x400 + i * 4, HUMAN_WORK + 2);
	}

	/* IOCSワークの設定 */
	m68k_write_memory_16(0x970, 79);       /* 画面の桁数-1 */
	m68k_write_memory_16(0x972, 24);       /* 画面の行数-1 */

	/* DOSコールベクタの設定 */
	for(int i = 0; i < 256; i ++ ) {
		m68k_write_memory_32(0x1800 + i * 4, HUMAN_WORK + 2);
	}

	FLAG_S = 0;

	/* Humanのメモリ管理ブロック設定 */
	struct v68_mem_block *human = (struct v68_mem_block *)&v68.ram[HUMAN_HEAD];
	human->prev = 0;
	human->parent = 0;
	human->end = HUMAN_WORK;
	human->next = 0;

	v68.heap_start = v68.heap_top = STACK_TOP + STACK_SIZE;

	return 0;
}

uint32_t v68_mem_alloc(int size, uint32_t parent_addr) {
	/* Align to 4 bytes */
	size = (size + 3) & 0xfffffc;

	uint32_t first = m68k_read_memory_32(HUMAN_HEAD + 0x0c);

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
		verbose1("Could not allocate %08x bytes ram_size=%08lx heap_top=%08x\n", size, v68.ram_size, v68.heap_top);
		return -2;
	}

	if(!prev) prev = HUMAN_HEAD;

	uint32_t next = v68.heap_top;
	m68k_write_memory_32(prev + 0x0c, next);
	m68k_write_memory_32(next, prev);
	m68k_write_memory_32(next + 0x04, parent_addr);
	m68k_write_memory_32(next + 0x08, next + 16 + size);
	m68k_write_memory_32(next + 0x0c, 0);
	v68.heap_top += 16 + size;
	return next + 16;
}

int v68_mem_shrink(uint32_t addr, uint32_t new_size) {
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD + 0x0c);

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
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD + 0x0c);

	addr -= 16;

	if(first) {
		for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
			if(addr == cur) {
				// Last in line
				if(m68k_read_memory_32(cur + 0x0c) == 0) {
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
	uint32_t first = m68k_read_memory_32(HUMAN_HEAD + 0x0c);
	for(uint32_t cur = first; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
		uint32_t end = m68k_read_memory_32(cur + 0x08);
		if(end > min) min = end;
		if(cur > min) min = cur;
	}

	return v68.ram_size - min - 0x200;
}

void v68_mem_dump() {
	for(uint32_t cur = HUMAN_HEAD; cur; cur = m68k_read_memory_32(cur + 0x0c)) {
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
