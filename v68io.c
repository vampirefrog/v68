#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#if __linux__
#include <mntent.h>
#elif __APPLE__
#include <sys/mount.h>
#endif
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "v68io.h"

#define MAX_DRIVES ('Z' - 'A' + 1)

struct drive {
	char vpath[PATH_MAX];
	char cwd[PATH_MAX];
};

struct drive drives[MAX_DRIVES];
static uint8_t curdrive = 0;

struct dosfile {
	int fd;
	uint16_t mode;
	char name[0x80];
};

struct dosfile dosfiles[96];

int v68_io_init() {
	memset(drives, 0, sizeof(drives));

#if __linux__ || __APPLE__
	// Init Z: as unix /
	char buf[PATH_MAX];
	buf[0] = 0;
	getcwd(buf, PATH_MAX);
	int l = strlen(buf);
	curdrive = 'Z' - 'A';
	strncpy(drives[curdrive].vpath, "/", PATH_MAX);

	if(l > 0) {
		for(int i = 1; i < l; i++)
			drives[curdrive].cwd[i-1] = buf[i] == '/' ? '\\' : buf[i];
	}
#endif

	dosfiles[0].fd = 0;
	dosfiles[0].mode = 0;
	strcpy(dosfiles[0].name, "STDIN");
	dosfiles[1].fd = 1;
	dosfiles[1].mode = 1;
	strcpy(dosfiles[0].name, "STDOUT");
	dosfiles[2].fd = 2;
	dosfiles[2].mode = 1;
	strcpy(dosfiles[0].name, "STDERR");

	// reserved fds, I think one is for printer or something
	dosfiles[3].fd = -1;
	dosfiles[4].fd = -1;

	return 0;
}

uint8_t v68_io_unused_drive() {
	// From D: to Y:
	for(int i = 3; i < 25; i++) {
		if(!drives[i].vpath[0]) return i;
	}
	return 0xff;
}

int v68_io_autodetect_drives() {
#if __linux__
	char *home = getenv("HOME");
	if(home && *home) {
		strncpy(drives['H' - 'A'].vpath, home, PATH_MAX);
		drives['H' - 'A'].cwd[0] = 0;
	}

	static char *ignore_fstypes[] = {
		"sysfs", "proc", "devtmpfs", "devpts", "tmpfs", "securityfs", "swap", 0
	};

	FILE *f = fopen("/etc/fstab", "r");
	struct mntent *m;
	while((m = getmntent(f))) {
		int valid = 1;
		for(char **i = ignore_fstypes; *i; i++) {
			if(!strcmp(*i, m->mnt_type))
				valid = 0;
		}
		if(!strcmp(m->mnt_dir, "/"))
			valid = 0;
		if(valid) {
			v68_io_add_drive(v68_io_unused_drive(), m->mnt_dir);
		}
	}
	fclose(f);
#elif __APPLE__
	char *home = getenv("HOME");
	if(home && *home) {
		strncpy(drives['H' - 'A'].vpath, home, PATH_MAX);
		drives['H' - 'A'].cwd[0] = 0;
	}

	struct statfs *mounts;
	int num_mounts = getmntinfo(&mounts, MNT_WAIT);
	for (int i = 0; i < num_mounts; i++) {
		v68_io_add_drive(v68_io_unused_drive(), mounts[i].f_mntonname);
	}
#else
	DWORD dw = GetLogicalDrives();
	for(int i = 'A', j = 1; i <= 'Z'; i++, j <<= 1) {
		if(dw & j) {
			char buf[10];
			snprintf(buf, sizeof(buf), "%c:\\", i);
			v68_io_add_drive(i - 'A', buf);
		}
	}

	char buf[PATH_MAX];
	buf[0] = 0;
	getcwd(buf, PATH_MAX);
	curdrive = toupper(buf[0]) - 'A';
	strncpy(drives[curdrive].cwd, buf + 3, sizeof(buf) / sizeof(buf[0]));

	for(int i = 'A'; i <= 'B'; i++) {
		if(!drives[i - 'A'].vpath[0]) {
			char buf2[PATH_MAX];
			snprintf(buf2, sizeof(buf2) / sizeof(buf2[0]), "%c:\\", i);
			v68_io_add_drive(i - 'A', buf2);
			strncpy(drives[i = 'A'].cwd, buf + 3, sizeof(buf) / sizeof(buf[0]));
		}
	}

#endif

	return 0;
}

int v68_io_open(char *filename, int mode) {
	char xlated[PATH_MAX];
	v68_io_xlate_dos_path(filename, xlated, sizeof(xlated));
	char resolved[PATH_MAX];
	v68_io_resolve_path(xlated, resolved, sizeof(resolved));

	verbose1("v68_io_open filename=\"%s\" mode=%04x xlated=\"%s\" resolved=\"%s\"\n", filename, mode, xlated, resolved);

	switch(mode & 0x03) {
		case 0x00:
			mode = O_RDONLY;
			break;
		case 0x01:
			mode = O_WRONLY | O_TRUNC;
			break;
		case 0x02:
			mode = O_RDWR;
			break;
	}
#ifdef WIN32
	mode |= _O_BINARY;
#endif
	int fd = open(resolved, mode, 0644);
	if(fd < 0) {
		// fprintf(stderr, "Could not open %s: %s (%d)\n", filename, strerror(errno), errno);
		return -1;
	}

	for(int i = 5; i < sizeof(dosfiles) / sizeof(dosfiles[0]); i++) {
		if(dosfiles[i].fd == 0) {
			dosfiles[i].fd = fd;
			strncpy(dosfiles[i].name, filename, sizeof(dosfiles[i].name));
			dosfiles[i].mode = mode;
			return i;
		}
	}

	close(fd);

	return -1;
}

int v68_io_create(char *filename, int attribs) {
	char xlated[PATH_MAX];
	v68_io_xlate_dos_path(filename, xlated, sizeof(xlated));
	char resolved[PATH_MAX];
	v68_io_resolve_path(xlated, resolved, sizeof(resolved));

	int fd = creat(resolved, 0644);
	if(fd < 0) {
		fprintf(stderr, "Could not create %s: %s (%d)\n", resolved, strerror(errno), errno);
		return -1;
	}

	for(int i = 5; i < sizeof(dosfiles) / sizeof(dosfiles[0]); i++) {
		if(dosfiles[i].fd == 0) {
			dosfiles[i].fd = fd;
			strncpy(dosfiles[i].name, filename, sizeof(dosfiles[i].name));
			dosfiles[i].mode = 0x02;
			return i;
		}
	}

	close(fd);

	return -1;
}

#define CHECKFD(fd) { \
	if(fd >= sizeof(dosfiles) / sizeof(dosfiles[0])) \
		return -1;                                   \
                                                     \
	if(!dosfiles[fd].fd)                             \
		return -1;                                   \
}

int v68_io_close(int fd) {
	verbose2("v68_io_close fd=%d\n", fd);
	CHECKFD(fd);

	int r = close(dosfiles[fd].fd);
	if(r) return -1;

	dosfiles[fd].fd = 0;
	dosfiles[fd].mode = 0;
	dosfiles[fd].name[0] = 0;

	return 0;
}

int v68_io_read(int fd, void *buf, size_t count) {
	CHECKFD(fd);

	return read(dosfiles[fd].fd, buf, count);
}

int v68_io_write(int fd, void *buf, size_t count) {
	CHECKFD(fd);

	return write(dosfiles[fd].fd, buf, count);
}

int v68_io_seek(int fd, off_t offset, int whence) {
	CHECKFD(fd);

	switch(whence) {
		case 1: whence = SEEK_CUR; break;
		case 2: whence = SEEK_END; break;
		default: whence = SEEK_SET;
	}

	return lseek(dosfiles[fd].fd, offset, whence);
}

int v68_io_tell(int fd) {
	CHECKFD(fd);

	return lseek(dosfiles[fd].fd, 0, SEEK_CUR);
}

int v68_io_rename(char *oldpath, char *newpath) {
	return rename(oldpath, newpath);
}

int v68_io_chmod(char *pathname, mode_t mode) {
	return 0;
}

int v68_io_curdrv(void) {
	return curdrive;
}

int v68_io_chgdrv(uint8_t drv) {
	if(drv > MAX_DRIVES) return -1;
	if(drives[drv].vpath[0] == 0) return -1;

	curdrive = drv;

	return 0;
}

int v68_io_num_drives() {
	int ret = 0;

	for(int i = 0; i < MAX_DRIVES; i++) {
		if(drives[i].vpath[0])
			ret++;
	}

	return ret;
}

int v68_io_getcwd(uint8_t drv, char *buf, size_t size) {
	if(drv == 0) drv = curdrive;
	if(drv > MAX_DRIVES) return -1;
	if(drives[drv].vpath[0] == 0) return -1;
	if(size < 1) return -1;
	if(!buf) return -1;

	strncpy(buf, drives[drv].cwd, size);

	return 0;
}

int v68_io_chdir(char *buf) {
	return 0;
}

int v68_argv_to_cmdline(int argc, char **argv, char *cmdline, int len) {
	return 0;
}

int v68_io_add_drive(uint8_t drive, char *path) {
	if(drive >= MAX_DRIVES) return -1;

	strncpy(drives[drive].vpath, path, PATH_MAX);
	drives[drive].cwd[0] = 0;

	return 0;
}

char *v68_io_xlate_path(char *filename, char *out, int len) {
	int longest = -1, max_length = -1;
	for(int i = 0; i < MAX_DRIVES; i++) {
		int l = strlen(drives[i].vpath);
		if(!strncasecmp(filename, drives[i].vpath, l)) {
			if(l > max_length) {
				longest = i;
				max_length = l;
			}
		}
	}

	if(longest >= 0) {
		snprintf(out, len, "%c:%s", 'A' + longest, filename + max_length);
		for(int i = 0; i < len; i++) {
			if(out[i] == '/') out[i] = '\\';
		}
		return out;
	}

	return 0;
}

char *v68_io_xlate_dos_path(char *filename, char *out, int len) {
	int l = strlen(filename);

	if(l >= 2 && filename[1] == ':') {
		// If drive is specified
		uint8_t drv = (filename[0] & ~0x20) - 'A';
		if(drv < 0 || drv >= 26) return 0;
		if(!drives[drv].vpath[0]) return 0;
		if(l > 2) {
			if(drives[drv].vpath[0] == DIR_SEP_CHR && drives[drv].vpath[1] == 0)
				snprintf(out, len, DIR_SEP "%s", filename + 3);
			else
				snprintf(out, len, "%s" DIR_SEP "%s", drives[drv].vpath, filename + 3);
		} else {
			snprintf(out, len, "%s", drives[drv].vpath);
		}
	} else if(l >= 1 && (filename[0] == UNIX_DIR_SEP_CHR || filename[0] == DOS_DIR_SEP_CHR)) {
		// Start with root of drive
		if(drives[curdrive].vpath[0] == DIR_SEP_CHR && drives[curdrive].vpath[1] == 0)
			snprintf(out, len, DIR_SEP "%s", filename + 1);
		else
			snprintf(out, len, "%s" DIR_SEP "%s", drives[curdrive].vpath, filename + 1);
	} else {
		// Start with current dir
		if(drives[curdrive].vpath[0] == DIR_SEP_CHR && drives[curdrive].vpath[1] == 0)
			snprintf(out, len, DIR_SEP "%s" DIR_SEP "%s", drives[curdrive].cwd, filename);
		else {
			snprintf(out, len, "%s" DIR_SEP "%s" DIR_SEP "%s", drives[curdrive].vpath, drives[curdrive].cwd, filename);
		}
	}

	for(char *c = out; *c; c++) {
		if(*c == '\\') *c = DIR_SEP_CHR;
	}

	return out;
}

// translate URL entities
#define APPEND_CHAR(x) { \
	out[out_pos++] = (x); \
	out[out_pos] = 0; \
	if(out_pos == len - 1) \
		return -1; \
}
static int urldecode(char *in, char *out, int len) {
	int in_percent = 0;
	char ent[3] = { 0, 0, 0 };
	int ent_pos = 0, out_pos = 0;
	for(char *c = in; *c; c++) {
		if(in_percent) {
			if((*c >= '0' && *c <= '9') || (*c >= 'A' && *c <= 'F') || (*c >= 'a' && *c <= 'f')) {
				ent[ent_pos] = *c;
				ent_pos++;
				if(ent_pos >= 2) {
					APPEND_CHAR(strtol(ent, 0, 16));
					in_percent = 0;
				}
			} else {
				// weird char
				APPEND_CHAR('%');
				for(int i = 0; i < ent_pos; i++) {
					APPEND_CHAR(ent[i]);
				}
				if(*c == '%') {
					in_percent = 1;
					ent_pos = 0;
				} else {
					APPEND_CHAR(*c);
					in_percent = 0;
				}
			}
		} else {
			if(*c == '%') {
				in_percent = 1;
				ent_pos = 0;
			} else {
				APPEND_CHAR(*c);
			}
		}
	}

	return out_pos;
}
#undef APPEND_CHAR

static int find_ci(char *from, char *to, int to_len) {
#if WIN32
	to[0] = 0;

	char buf[PATH_MAX];

	buf[0] = '/';
	buf[1] = 0;

	for(char *c = strtok(from, "/\\"); c && *c; c = strtok(0, "/\\")) {
		DIR *d = opendir(buf);
		if(!d) {
			to[0] = 0;
			return -1;
		}
		struct dirent de, *result;
		int found = 0;
		for(result = readdir(d); result; result = readdir(d)) {
			if(!strcasecmp(c, de.d_name)) {
				found = 1;
				snprintf(buf, to_len, "%s/%s", to, de.d_name);
				break;
			}
		}
		closedir(d);
		if(!found) {
			snprintf(buf, to_len, "%s/%s", to, c);
//			return -1;
		}
		strncpy(to, buf, to_len);
	}

#else
	strncpy(to, from, to_len);
#endif

	return 0;
}

char *v68_io_resolve_path(char *filename, char *out, int len) {
	char buf[PATH_MAX];

	if(urldecode(filename, buf, sizeof(buf)) < 0) return 0;
	// only deal with absolute paths
#if WIN32
	if(buf[1] != ':') return 0;
#else
	if(buf[0] != '/') return 0;
#endif

	struct stat st;
	if(stat(buf, &st) == 0) {
		strncpy(out, buf, len);
		return out;
	}

	if(find_ci(buf, out, len)) return 0;

	return out;
}

void v68_dump_drives() {
	for(int i = 0; i < MAX_DRIVES; i++) {
		if(!drives[i].vpath[0]) continue;

		verbose1("drive=\"%c:\" cwd=\"%s\" vpath=\"%s\"\n", 'A' + i, drives[i].cwd, drives[i].vpath);
	}
}
