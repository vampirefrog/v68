#pragma once

#include <stdio.h>
#include <stdint.h>

struct v68_psp {
	uint32_t env_addr;        /* 0x10 (16)  Environment's address (not secured if -1) */
	uint32_t return_addr;     /* 0x14 (20)  Return address at the end */
	uint32_t ctrl_c_addr;     /* 0x18 (24)  Return address when interrupted by CTRL + C */
	uint32_t error_ret_addr;  /* 0x1c (28)  Return address when interrupt due to error */
	uint32_t cmdline_addr;    /* 0x20 (32)  Command line address */
	uint8_t  file_handle[12]; /* 0x24 (36)  File handle usage of process */
	                          /* (In use = 1 in the order of bits 0 to 7 of $24 to $2f) */
	uint32_t bss_addr;        /* 0x30 (48)  Start address of BSS */
	uint32_t heap_addr;       /* 0x34 (52)  Start address of heap (same as BSS) */
	uint32_t stack_start;     /* 0x38 (56)  Initial stack address (end of heap + 1) */
	uint32_t parent_usp;      /* 0x3c (60)  USP of the parent process */
	uint32_t parent_ssp;      /* 0x40 (64)  parent process SSP */
	uint16_t parent_sr;       /* 0x44 (68)  parent process SR */
	uint16_t abort_sr;        /* 0x46 (70)  SR at abort */
	uint32_t abort_ssp;       /* 0x48 (72)  SSP at abort */
	uint32_t trap_10;         /* 0x4c (76)  trap #10 vector */
	uint32_t trap_11;         /* 0x50 (80)  trap #11 vector */
    uint32_t trap_12;         /* 0x54 (84)  trap #12 vector */
	uint32_t trap_13;         /* 0x58 (88)  trap #13 vector */
	uint32_t trap_14;         /* 0x5c (92)  trap #14 vector */
	uint32_t shell_flag;      /* 0x60 (96)  shell activation flag (0: normal startup - 1: started as shell) */
	uint8_t  module_num;      /* 0x64 (100) Module number */
	uint8_t  unused[3];
	uint32_t child_psp;       /* 0x68 (104) Process management pointer of loaded child process */
	uint32_t unused2[5];
	uint8_t  drive[2];        /* 0x80 (128) Drive name of executable file */
	uint8_t  exec_path[66];   /* 0x82 (130) Path name of executable file */
	uint8_t  exec_name[24];   /* 0xc4 (196) File name of executable file */
	uint32_t unused3[9];
} __attribute__((packed));    /* 240B */

struct v68_x_header {
	enum {
		Normal,
		MinimumBlock,
		HighAddress
	} load_mode;

	uint8_t *base_address;
	uint8_t *starting_address;
	uint32_t text_section_size;
	uint32_t data_section_size;
	uint32_t block_section_size; /* Block storage section size, including .comm .stack */
	uint32_t relocation_information_size;
	uint32_t symbol_table_size;
	uint32_t scd_line_number_table_size;
	uint32_t scd_symbol_table_size;
	uint32_t scd_string_table_size;
	uint32_t bound_module_offset;
} __attribute__((packed));

struct v68_file_info {
	FILE     *fh;
	unsigned date;
	unsigned time;
	short    mode;
	char     nest;
	char     name[89];
};

void v68_human_init(void);

/* Initialize allocator. */
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

/* Command queue */
/* Similar to batch files but much simpler */
int v68_queue_command(char *cmdline);
/* Go to the next command or stop */
void v68_queue_next_command();

/* Env vars */
int v68_env_append(char *env);
int v68_env_set(char *var, char *value);
char *v68_find_in_path(char *cmd);
char *v68_env_get(char *var);
