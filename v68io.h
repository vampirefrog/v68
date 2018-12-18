#pragma once

#include"v68.h"

int v68_io_init(void);
int v68_io_autodetect_drives(void);

int v68_io_open(char *filename, int mode);
int v68_io_create(char *filename, int attr);
int v68_io_close(int fd);
int v68_io_read(int fd, void *buf, size_t count);
int v68_io_write(int fd, void *buf, size_t count);
int v68_io_seek(int fd, off_t offset, int whence);
int v68_io_rename(char *oldpath, char *newpath);
int v68_io_chmod(char *pathname, mode_t mode);

int v68_io_curdrv(void);
int v68_io_chgdrv(uint8_t drv);
int v68_io_getcwd(uint8_t drv, char *buf, size_t size);
int v68_io_chdir(char *buf);


int v68_argv_to_cmdline(int argc, char **argv, char *cmdline, int len);
// Translate from local (unix/windows) file to virtual file path
char *v68_io_xlate_path(char *filename, char *out, int len);
// Translate from virtual (DOS) file path to local (unix/windows)
char *v68_io_xlate_dos_path(char *filename, char *out, int len);
// Find a physical file on the local fs
// does case insensitive to case sensitive finding
// also parses %20 and %25 url encoded special chars (for supporting spaces)
char *v68_io_resolve_path(char *filename, char *out, int len);

// Virtual drives
int v68_io_add_drive(uint8_t drive, char *path);
uint8_t v68_io_unused_drive();
void v68_dump_drives();
