#pragma once

/* Memory */

/* Memory management block */
struct v68_mem_block {
	uint32_t prev;            /* 0x00 (0)   Previous memory management pointer (leading with 0) */
	uint32_t parent;          /* 0x04 (4)   Parent process memory management pointer (0 with no parent) */
	uint32_t end;             /* 0x08 (8)   Last address of this memory block + 1 */
	uint32_t next;            /* 0x0C (12)  Next memory management pointer (end with 0) */
} __attribute__((packed));    /* 16B */

/* Initialize allocator and other misc stuff. */
int v68_mem_init();
/* Allocate a block of memory. */
uint32_t v68_mem_alloc(int size, uint32_t parent_addr);
/* Free a previously allocated block of memory. */
int v68_mem_free(uint32_t addr, uint32_t parent_addr);
/* Get memory remaining to RAM top */
uint32_t v68_mem_remaining();
/* Resize a block. */
int v68_mem_shrink(uint32_t addr, uint32_t new_size);
/* Get the max allocatable block size. */
int v68_mem_get_max();
/* Free all the memory that belongs to a parent process. */
/* Useful for when a process ends. */
int v68_mem_free_parent(uint32_t parent_addr);
void v68_mem_dump();
