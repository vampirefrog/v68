#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "v68.h"
#include "v68io.h"
#include "v68mem.h"
#include "musashi/m68k.h"
#include "musashi/m68kcpu.h"
#include "sjis.h"

#define DOS_CALL_EXIT           0x00
#define DOS_CALL_GETCHAR        0x01
#define DOS_CALL_PUTCHAR        0x02
#define DOS_CALL_COMINP         0x03
#define DOS_CALL_COMOUT         0x04
#define DOS_CALL_PRNOUT         0x05
#define DOS_CALL_INPOUT         0x06
#define DOS_CALL_INKEY          0x07
#define DOS_CALL_GETC           0x08
#define DOS_CALL_PRINT          0x09
#define DOS_CALL_GETS           0x0a
#define DOS_CALL_KEYSNS         0x0b
#define DOS_CALL_KFLUSH         0x0c
#define DOS_CALL_FFLUSH         0x0d
#define DOS_CALL_CHGDRV         0x0e
#define DOS_CALL_DRVCTRL        0x0f
#define DOS_CALL_CONSNS         0x10
#define DOS_CALL_PRNSNS         0x11
#define DOS_CALL_CINSNS         0x12
#define DOS_CALL_COUTSNS        0x13
#define DOS_CALL_FATCHK         0x17
#define DOS_CALL_HENDSP         0x18
#define DOS_CALL_CURDRV         0x19
#define DOS_CALL_GETSS          0x1a
#define DOS_CALL_FGETC          0x1b
#define DOS_CALL_FGETS          0x1c
#define DOS_CALL_FPUTC          0x1d
#define DOS_CALL_FPUTS          0x1e
#define DOS_CALL_ALLCLOSE       0x1f
#define DOS_CALL_SUPER          0x20
#define DOS_CALL_FNCKEY         0x21
#define DOS_CALL_KNJCTRL        0x22
#define DOS_CALL_CONCTRL        0x23
#define DOS_CALL_KEYCTRL        0x24
#define DOS_CALL_INTVCS         0x25
#define DOS_CALL_PSPSET         0x26
#define DOS_CALL_GETTIM2        0x27
#define DOS_CALL_SETTIM2        0x28
#define DOS_CALL_NAMESTS        0x29
#define DOS_CALL_GETDATE        0x2a
#define DOS_CALL_SETDATE        0x2b
#define DOS_CALL_GETTIME        0x2c
#define DOS_CALL_SETTIME        0x2d
#define DOS_CALL_VERIFY         0x2e
#define DOS_CALL_DUP0           0x2f
#define DOS_CALL_VERNUM         0x30
#define DOS_CALL_KEEPPR         0x31
#define DOS_CALL_GETDPB         0x32
#define DOS_CALL_BREAKCK        0x33
#define DOS_CALL_DRVXCHG        0x34
#define DOS_CALL_INTVCG         0x35
#define DOS_CALL_DSKFRE         0x36
#define DOS_CALL_NAMECK         0x37
#define DOS_CALL_MKDIR          0x39
#define DOS_CALL_RMDIR          0x3a
#define DOS_CALL_CHDIR          0x3b
#define DOS_CALL_CREATE         0x3c
#define DOS_CALL_OPEN           0x3d
#define DOS_CALL_CLOSE          0x3e
#define DOS_CALL_READ           0x3f
#define DOS_CALL_WRITE          0x40
#define DOS_CALL_DELETE         0x41
#define DOS_CALL_SEEK           0x42
#define DOS_CALL_CHMOD          0x43
#define DOS_CALL_IOCTRL         0x44
#define DOS_CALL_DUP            0x45
#define DOS_CALL_DUP2           0x46
#define DOS_CALL_CURDIR         0x47
#define DOS_CALL_MALLOC         0x48
#define DOS_CALL_MFREE          0x49
#define DOS_CALL_SETBLOCK       0x4a
#define DOS_CALL_EXEC           0x4b
#define DOS_CALL_EXIT2          0x4c
#define DOS_CALL_WAIT           0x4d
#define DOS_CALL_FILES          0x4e
#define DOS_CALL_NFILES         0x4f
#define DOS_CALL_SETPDB         0x80
#define DOS_CALL_GETPDB         0x81
#define DOS_CALL_SETENV         0x82
#define DOS_CALL_GETENV         0x83
#define DOS_CALL_VERIFYG        0x84
#define DOS_CALL_COMMON         0x85
#define DOS_CALL_RENAME         0x86
#define DOS_CALL_FILEDATE       0x87
#define DOS_CALL_MALLOC2        0x88
#define DOS_CALL_MAKETMP        0x8a
#define DOS_CALL_NEWFILE        0x8b
#define DOS_CALL_LOCK           0x8c
#define DOS_CALL_ASSIGN         0x8f
#define DOS_CALL_FFLUSH_SET     0xaa
#define DOS_CALL_OS_PATCH       0xab
#define DOS_CALL_GETFCB         0xac
#define DOS_CALL_S_MALLOC       0xad
#define DOS_CALL_S_MFREE        0xae
#define DOS_CALL_S_PROCESS      0xaf
#define DOS_CALL_EXITVC         0xf0
#define DOS_CALL_CTRLVC         0xf1
#define DOS_CALL_ERRJVC         0xf2
#define DOS_CALL_DISKRED        0xf3
#define DOS_CALL_DISKWRT        0xf4
#define DOS_CALL_INDOSFLG       0xf5
#define DOS_CALL_SUPER_JSR      0xf6
#define DOS_CALL_BUS_ERR        0xf7
#define DOS_CALL_OPEN_PR        0xf8
#define DOS_CALL_KILL_PR        0xf9
#define DOS_CALL_GET_PR         0xfa
#define DOS_CALL_SUSPEND_PR     0xfb
#define DOS_CALL_SLEEP_PR       0xfc
#define DOS_CALL_SEND_PR        0xfd
#define DOS_CALL_TIME_PR        0xfe
#define DOS_CALL_CHANGE_PR      0xff

static const char *dos_call_names[256] = {
	/* 0x00 */ "_EXIT",
	/* 0x01 */ "_GETCHAR",
	/* 0x02 */ "_PUTCHAR",
	/* 0x03 */ "_COMINP",
	/* 0x04 */ "_COMOUT",
	/* 0x05 */ "_PRNOUT",
	/* 0x06 */ "_INPOUT",
	/* 0x07 */ "_INKEY",
	/* 0x08 */ "_GETC",
	/* 0x09 */ "_PRINT",
	/* 0x0a */ "_GETS",
	/* 0x0b */ "_KEYSNS",
	/* 0x0c */ "_KFLUSH",
	/* 0x0d */ "_FFLUSH",
	/* 0x0e */ "_CHGDRV",
	/* 0x0f */ "_DRVCTRL",
	/* 0x10 */ "_CONSNS",
	/* 0x11 */ "_PRNSNS",
	/* 0x12 */ "_CINSNS",
	/* 0x13 */ "_COUTSNS",
	/* 0x14 */ "?",
	/* 0x15 */ "?",
	/* 0x16 */ "?",
	/* 0x17 */ "_FATCHK",
	/* 0x18 */ "_HENDSP",
	/* 0x19 */ "_CURDRV",
	/* 0x1a */ "_GETSS",
	/* 0x1b */ "_FGETC",
	/* 0x1c */ "_FGETS",
	/* 0x1d */ "_FPUTC",
	/* 0x1e */ "_FPUTS",
	/* 0x1f */ "_ALLCLOSE",
	/* 0x20 */ "_SUPER",
	/* 0x21 */ "_FNCKEY",
	/* 0x22 */ "_KNJCTRL",
	/* 0x23 */ "_CONCTRL",
	/* 0x24 */ "_KEYCTRL",
	/* 0x25 */ "_INTVCS",
	/* 0x26 */ "_PSPSET",
	/* 0x27 */ "_GETTIM2",
	/* 0x28 */ "_SETTIM2",
	/* 0x29 */ "_NAMESTS",
	/* 0x2a */ "_GETDATE",
	/* 0x2b */ "_SETDATE",
	/* 0x2c */ "_GETTIME",
	/* 0x2d */ "_SETTIME",
	/* 0x2e */ "_VERIFY",
	/* 0x2f */ "_DUP0",
	/* 0x30 */ "_VERNUM",
	/* 0x31 */ "_KEEPPR",
	/* 0x32 */ "_GETDPB",
	/* 0x33 */ "_BREAKCK",
	/* 0x34 */ "_DRVXCHG",
	/* 0x35 */ "_INTVCG",
	/* 0x36 */ "_DSKFRE",
	/* 0x37 */ "_NAMECK",
	/* 0x38 */ "?",
	/* 0x39 */ "_MKDIR",
	/* 0x3a */ "_RMDIR",
	/* 0x3b */ "_CHDIR",
	/* 0x3c */ "_CREATE",
	/* 0x3d */ "_OPEN",
	/* 0x3e */ "_CLOSE",
	/* 0x3f */ "_READ",
	/* 0x40 */ "_WRITE",
	/* 0x41 */ "_DELETE",
	/* 0x42 */ "_SEEK",
	/* 0x43 */ "_CHMOD",
	/* 0x44 */ "_IOCTRL",
	/* 0x45 */ "_DUP",
	/* 0x46 */ "_DUP2",
	/* 0x47 */ "_CURDIR",
	/* 0x48 */ "_MALLOC",
	/* 0x49 */ "_MFREE",
	/* 0x4a */ "_SETBLOCK",
	/* 0x4b */ "_EXEC",
	/* 0x4c */ "_EXIT2",
	/* 0x4d */ "_WAIT",
	/* 0x4e */ "_FILES",
	/* 0x4f */ "_NFILES",
	/* 0x50 */ "?",
	/* 0x51 */ "?",
	/* 0x52 */ "?",
	/* 0x53 */ "?",
	/* 0x54 */ "?",
	/* 0x55 */ "?",
	/* 0x56 */ "?",
	/* 0x57 */ "?",
	/* 0x58 */ "?",
	/* 0x59 */ "?",
	/* 0x5a */ "?",
	/* 0x5b */ "?",
	/* 0x5c */ "?",
	/* 0x5d */ "?",
	/* 0x5e */ "?",
	/* 0x5f */ "?",
	/* 0x60 */ "?",
	/* 0x61 */ "?",
	/* 0x62 */ "?",
	/* 0x63 */ "?",
	/* 0x64 */ "?",
	/* 0x65 */ "?",
	/* 0x66 */ "?",
	/* 0x67 */ "?",
	/* 0x68 */ "?",
	/* 0x69 */ "?",
	/* 0x6a */ "?",
	/* 0x6b */ "?",
	/* 0x6c */ "?",
	/* 0x6d */ "?",
	/* 0x6e */ "?",
	/* 0x6f */ "?",
	/* 0x70 */ "?",
	/* 0x71 */ "?",
	/* 0x72 */ "?",
	/* 0x73 */ "?",
	/* 0x74 */ "?",
	/* 0x75 */ "?",
	/* 0x76 */ "?",
	/* 0x77 */ "?",
	/* 0x78 */ "?",
	/* 0x79 */ "?",
	/* 0x7a */ "?",
	/* 0x7b */ "?",
	/* 0x7c */ "?",
	/* 0x7d */ "?",
	/* 0x7e */ "?",
	/* 0x7f */ "?",
	/* 0x80 */ "_SETPDB",
	/* 0x81 */ "_GETPDB",
	/* 0x82 */ "_SETENV",
	/* 0x83 */ "_GETENV",
	/* 0x84 */ "_VERIFYG",
	/* 0x85 */ "_COMMON",
	/* 0x86 */ "_RENAME",
	/* 0x87 */ "_FILEDATE",
	/* 0x88 */ "_MALLOC2",
	/* 0x89 */ "?",
	/* 0x8a */ "_MAKETMP",
	/* 0x8b */ "_NEWFILE",
	/* 0x8c */ "_LOCK",
	/* 0x8d */ "?",
	/* 0x8e */ "?",
	/* 0x8f */ "_ASSIGN",
	/* 0x90 */ "?",
	/* 0x91 */ "?",
	/* 0x92 */ "?",
	/* 0x93 */ "?",
	/* 0x94 */ "?",
	/* 0x95 */ "?",
	/* 0x96 */ "?",
	/* 0x97 */ "?",
	/* 0x98 */ "?",
	/* 0x99 */ "?",
	/* 0x9a */ "?",
	/* 0x9b */ "?",
	/* 0x9c */ "?",
	/* 0x9d */ "?",
	/* 0x9e */ "?",
	/* 0x9f */ "?",
	/* 0xa0 */ "?",
	/* 0xa1 */ "?",
	/* 0xa2 */ "?",
	/* 0xa3 */ "?",
	/* 0xa4 */ "?",
	/* 0xa5 */ "?",
	/* 0xa6 */ "?",
	/* 0xa7 */ "?",
	/* 0xa8 */ "?",
	/* 0xa9 */ "?",
	/* 0xaa */ "_FFLUSH_SET",
	/* 0xab */ "_OS_PATCH",
	/* 0xac */ "_GETFCB",
	/* 0xad */ "_S_MALLOC",
	/* 0xae */ "_S_MFREE",
	/* 0xaf */ "_S_PROCESS",
	/* 0xb0 */ "?",
	/* 0xb1 */ "?",
	/* 0xb2 */ "?",
	/* 0xb3 */ "?",
	/* 0xb4 */ "?",
	/* 0xb5 */ "?",
	/* 0xb6 */ "?",
	/* 0xb7 */ "?",
	/* 0xb8 */ "?",
	/* 0xb9 */ "?",
	/* 0xba */ "?",
	/* 0xbb */ "?",
	/* 0xbc */ "?",
	/* 0xbd */ "?",
	/* 0xbe */ "?",
	/* 0xbf */ "?",
	/* 0xc0 */ "?",
	/* 0xc1 */ "?",
	/* 0xc2 */ "?",
	/* 0xc3 */ "?",
	/* 0xc4 */ "?",
	/* 0xc5 */ "?",
	/* 0xc6 */ "?",
	/* 0xc7 */ "?",
	/* 0xc8 */ "?",
	/* 0xc9 */ "?",
	/* 0xca */ "?",
	/* 0xcb */ "?",
	/* 0xcc */ "?",
	/* 0xcd */ "?",
	/* 0xce */ "?",
	/* 0xcf */ "?",
	/* 0xd0 */ "?",
	/* 0xd1 */ "?",
	/* 0xd2 */ "?",
	/* 0xd3 */ "?",
	/* 0xd4 */ "?",
	/* 0xd5 */ "?",
	/* 0xd6 */ "?",
	/* 0xd7 */ "?",
	/* 0xd8 */ "?",
	/* 0xd9 */ "?",
	/* 0xda */ "?",
	/* 0xdb */ "?",
	/* 0xdc */ "?",
	/* 0xdd */ "?",
	/* 0xde */ "?",
	/* 0xdf */ "?",
	/* 0xe0 */ "?",
	/* 0xe1 */ "?",
	/* 0xe2 */ "?",
	/* 0xe3 */ "?",
	/* 0xe4 */ "?",
	/* 0xe5 */ "?",
	/* 0xe6 */ "?",
	/* 0xe7 */ "?",
	/* 0xe8 */ "?",
	/* 0xe9 */ "?",
	/* 0xea */ "?",
	/* 0xeb */ "?",
	/* 0xec */ "?",
	/* 0xed */ "?",
	/* 0xee */ "?",
	/* 0xef */ "?",
	/* 0xf0 */ "_EXITVC",
	/* 0xf1 */ "_CTRLVC",
	/* 0xf2 */ "_ERRJVC",
	/* 0xf3 */ "_DISKRED",
	/* 0xf4 */ "_DISKWRT",
	/* 0xf5 */ "_INDOSFLG",
	/* 0xf6 */ "_SUPER_JSR",
	/* 0xf7 */ "_BUS_ERR",
	/* 0xf8 */ "_OPEN_PR",
	/* 0xf9 */ "_KILL_PR",
	/* 0xfa */ "_GET_PR",
	/* 0xfb */ "_SUSPEND_PR",
	/* 0xfc */ "_SLEEP_PR",
	/* 0xfd */ "_SEND_PR",
	/* 0xfe */ "_TIME_PR",
	/* 0xff */ "_CHANGE_PR",
};

int v68_dos_call(uint16_t instr) {
	uint8_t call = instr & 0xff;

	if(v68.log_calls)
		printf("DOS %02x %s\n", call, dos_call_names[call]);

	switch(call) {
		case DOS_CALL_EXIT:
			v68.running = 0;
			v68_mem_free(v68.cur_prog_addr, m68k_read_memory_32(v68.cur_prog_addr - 0x0c));
			v68_queue_next_command();
			break;
		case DOS_CALL_EXIT2:
			v68.running = 0;
			v68_mem_free(v68.cur_prog_addr, m68k_read_memory_32(v68.cur_prog_addr - 0x0c));
			v68_queue_next_command();
			break;
		case DOS_CALL_PRINT: {
				uint32_t str_addr = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				logcall("str=%08x\n", str_addr);
				if(str_addr < v68.ram_size) {
					uint8_t *str_ptr = &v68.ram[str_addr];
					size_t l = strnlen((char *)str_ptr, v68.ram_size - str_addr);
					sjis_print_utf8(str_ptr, l);
					fflush(stdout);
				}
			}
			break;
		case DOS_CALL_PUTCHAR: {
				uint16_t chr = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint8_t chrbuf[2] = { chr >> 8, chr };
				sjis_print_utf8(chrbuf[0] ? chrbuf : chrbuf+1, chrbuf[0] ? 2 : 1);
			}
			break;
		case DOS_CALL_KEEPPR: {
				uint32_t prglen = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint16_t code = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 4);
				logcall("KEEPPR prglen=%08x (%dkB) code=%04x\n", prglen, prglen/1024, code);
				v68_mem_shrink(v68.cur_prog_addr, prglen);
				v68_mem_dump();
				v68_queue_next_command();
			}
			break;
		case DOS_CALL_SUPER: {
				uint32_t sp = m68k_get_reg(0, M68K_REG_A7);
				uint32_t stack = m68k_read_memory_32(sp);
				logcall("SUPER %08x\n", stack);
				FLAG_S = 1;
				m68k_set_reg(M68K_REG_SR, m68k_get_reg(0, M68K_REG_SR) | 0x2000);
				m68k_set_reg(M68K_REG_A7, sp);
			}
			break;
		case DOS_CALL_IOCTRL: {
				uint16_t md = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				switch(md) {
					case 0: {
							uint16_t fileno = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 2);
							if(fileno == 0) m68k_set_reg(M68K_REG_D0, 0x80c1);
							else if(fileno == 1 || fileno == 2) m68k_set_reg(M68K_REG_D0, 0x80c2);
							else m68k_set_reg(M68K_REG_D0, 0);
						}
						break;
					default:
						logcall("IOCTRL %04x\n", md);
				}
			}
			break;
		case DOS_CALL_SETBLOCK: {
				v68_mem_dump();
				uint32_t memptr = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t len = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				// printf("SETBLOCK memptr=%08x len=%d\n", memptr, len);
				v68_mem_shrink(memptr, len);
				v68_mem_dump();
			}
			break;
		case DOS_CALL_MALLOC: {
				uint32_t len = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t remaining = v68_mem_remaining();
				if(len > remaining) len = remaining;
				uint32_t m = v68_mem_alloc(len, v68.cur_prog_addr);
				// printf("MALLOC len=%08x -> %08x\n", len, m);
				m68k_set_reg(M68K_REG_D0, m);
				v68_mem_dump();
			}
			break;
		case DOS_CALL_FPUTS: {
				uint32_t mesptr = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint16_t fileno = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 4);
				uint8_t *str_ptr = &v68.ram[mesptr];
				size_t l = strnlen((char *)str_ptr, v68.ram_size - mesptr);
				if(fileno == 2) {
					sjis_print_utf8(str_ptr, l);
				} else {
					printf("FPUTS mesptr=%08x \"%s\" fileno=%d\n", mesptr, v68.ram + mesptr, fileno);
				}
			}
			break;
		case DOS_CALL_VERNUM: {
				m68k_set_reg(M68K_REG_D0, 0x36380302);
			}
			break;
		case DOS_CALL_GETDATE: {
				time_t t = time(NULL);
				struct tm result;
				localtime_r(&t, &result);
				uint32_t d = ((result.tm_wday & 0x07) << 16) | (((result.tm_year - 80) & 0x7f) << 9) | ((result.tm_mon & 0x0f) << 5) | (result.tm_mday & 0x1f);
				m68k_set_reg(M68K_REG_D0, d);
			}
			break;
		case DOS_CALL_GETTIME: {
				time_t t = time(NULL);
				struct tm result;
				localtime_r(&t, &result);
				uint16_t d = ((result.tm_hour & 0x1f) << 11) | ((result.tm_min & 0x3f) << 5) | ((result.tm_sec >> 1) & 0x1f);
				m68k_set_reg(M68K_REG_D0, d);
			}
			break;
		case DOS_CALL_GETTIM2: {
				time_t t = time(NULL);
				struct tm result;
				localtime_r(&t, &result);
				uint16_t d = ((result.tm_hour & 0x1f) << 16) | ((result.tm_min & 0x3f) << 8) | (result.tm_sec & 0x3f);
				m68k_set_reg(M68K_REG_D0, d);
			}
			break;
		case DOS_CALL_CREATE: {
				uint32_t file = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint16_t attr = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 4);
				uint8_t *str_ptr = &v68.ram[file];
				size_t l = strnlen((char *)str_ptr, v68.ram_size - file - 1);
				str_ptr[l] = 0;
				// printf("Create %s %04x\n", str_ptr, attr);
				logcall("Create %s attr=%04x\n", str_ptr, attr);
				int fd = v68_io_create((char *)str_ptr, attr);
				m68k_set_reg(M68K_REG_D0, fd);

				/* Fill fd bitmap */
				if(fd > 0) {
					fd -= 4;
					uint32_t fdmapaddr = v68.cur_prog_addr + 0x24 + (fd >> 3);
					m68k_write_memory_8(fdmapaddr, m68k_read_memory_8(fdmapaddr) | (1 << (fd & 0x07)));
				}
			}
			break;
		case DOS_CALL_OPEN: {
				uint32_t file = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint16_t mode = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 4);
				uint8_t *str_ptr = &v68.ram[file];
				size_t l = strnlen((char *)str_ptr, v68.ram_size - file - 1);
				str_ptr[l] = 0;
				logcall("Open %s %04x\n", str_ptr, mode);
				int fd = v68_io_open((char *)str_ptr, mode);
				m68k_set_reg(M68K_REG_D0, fd);

				/* Fill fd bitmap */
				if(fd > 0) {
					fd -= 4;
					uint32_t fdmapaddr = v68.cur_prog_addr + 0x24 + (fd >> 3);
					m68k_write_memory_8(fdmapaddr, m68k_read_memory_8(fdmapaddr) | (1 << (fd & 0x07)));
				}
			}
			break;
		case DOS_CALL_CLOSE: {
				uint16_t fd = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				int r = v68_io_close(fd);
				m68k_set_reg(M68K_REG_D0, r);

				logcall("%d\n", fd);

				/* Unset bit from fd bitmap */
				if(!r) {
					fd -= 4;
					uint32_t fdmapaddr = v68.cur_prog_addr + 0x24 + (fd >> 3);
					m68k_write_memory_8(fdmapaddr, m68k_read_memory_8(fdmapaddr) & ~(1 << (fd & 0x07)));
				}
			}
			break;
		case DOS_CALL_WRITE: {
				uint16_t fd = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint32_t buffer = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 2);
				uint32_t len = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 6);

				int r = v68_io_write(fd, &v68.ram[buffer], len);
				logcall("fd=%d len=%d r=%d\n", fd, len, r);

				m68k_set_reg(M68K_REG_D0, r);
			}
			break;
		case DOS_CALL_READ: {
				uint16_t fd = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint32_t buffer = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 2);
				uint32_t len = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 6);

				int r = v68_io_read(fd, &v68.ram[buffer], len);
				logcall("fd=%d len=%d r=%d\n", fd, len, r);

				m68k_set_reg(M68K_REG_D0, r);
			}
			break;
		case DOS_CALL_FGETC: {
				uint16_t fd = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint8_t b = 0;
				int r = v68_io_read(fd, &b, 1);
				logcall("fd=%d r=%d\n", fd, r);
				m68k_set_reg(M68K_REG_D0, b);
			}
			break;
		case DOS_CALL_SEEK: {
				uint16_t fd = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint32_t ofs = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 2);
				uint32_t whence = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7) + 6);

				int r = v68_io_seek(fd, ofs, whence);

				m68k_set_reg(M68K_REG_D0, r);
			}
			break;
		case DOS_CALL_INTVCS: {
				uint16_t intno = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				uint32_t jobadr = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 2);
				if(intno <= 0xff) {
					m68k_write_memory_32(intno * 4, jobadr);
				}
				logcall("intno=%04x jobaddr=%08x\n", intno, jobadr);
				m68k_set_reg(M68K_REG_D0, 0);
			}
			break;
		case DOS_CALL_INTVCG: {
				uint16_t intno = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				m68k_set_reg(M68K_REG_D0, m68k_read_memory_32(intno * 4));
			}
			break;
		case DOS_CALL_PRNSNS: {
				m68k_set_reg(M68K_REG_D0, 0);
			}
			break;
		case DOS_CALL_CONCTRL: {
				uint16_t md = m68k_read_memory_16(m68k_get_reg(0, M68K_REG_A7));
				switch(md) {
					case 0x01: {
							uint32_t mesptr = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 2);
							uint8_t *str_ptr = &v68.ram[mesptr];
							size_t l = strnlen((char *)str_ptr, v68.ram_size - mesptr);
							sjis_print_utf8(str_ptr, l);
						}
						break;
					default:
						printf("conctrl md=%d\n", md);
				}
			}
			break;
		case DOS_CALL_NAMECK: {
				uint32_t filename = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t buffer = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				strncpy((char *)&v68.ram[buffer], (char *)&v68.ram[filename], 91);
			}
			break;
		default:
			printf("V68 DOS CALL %04x %s\n", instr, dos_call_names[call]);
			break;
	}

	return 0;
}
