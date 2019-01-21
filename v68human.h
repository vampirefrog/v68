#pragma once

#include <stdio.h>
#include <stdint.h>

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
