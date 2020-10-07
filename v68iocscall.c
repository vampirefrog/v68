#include <stdint.h>
#include <stdio.h>
#include <string.h>
#if __linux__
#include <sys/sysinfo.h>
#endif
#include "v68.h"
#include "v68iocscall.h"
#include "sjis.h"
#include "musashi/m68k.h"
#include "musashi/m68kcpu.h"
#ifdef WIN32
#include <windows.h>
#endif

#define IOCS_CALL_B_KEYINP  0x00
#define IOCS_CALL_B_KEYSNS  0x01
#define IOCS_CALL_B_SFTSNS  0x02
#define IOCS_CALL_KEY_INIT  0x03
#define IOCS_CALL_BITSNS    0x04
#define IOCS_CALL_SKEYSET   0x05
#define IOCS_CALL_LEDCTRL   0x06
#define IOCS_CALL_LEDSET    0x07
#define IOCS_CALL_KEYDLY    0x08
#define IOCS_CALL_KEYREP    0x09
#define IOCS_CALL_TVCTRL    0x0c
#define IOCS_CALL_LEDMOD    0x0d
#define IOCS_CALL_TGUSEMD   0x0e
#define IOCS_CALL_DEFCHR    0x0f
#define IOCS_CALL_CRTMOD    0x10
#define IOCS_CALL_CONTRAST  0x11
#define IOCS_CALL_HSVTORGB  0x12
#define IOCS_CALL_TPALET    0x13
#define IOCS_CALL_TPALET2   0x14
#define IOCS_CALL_TCOLOR    0x15
#define IOCS_CALL_FNTADR    0x16
#define IOCS_CALL_VRAMGET   0x17
#define IOCS_CALL_VRAMPUT   0x18
#define IOCS_CALL_FNTGET    0x19
#define IOCS_CALL_TEXTGET   0x1a
#define IOCS_CALL_TEXTPUT   0x1b
#define IOCS_CALL_CLIPPUT   0x1c
#define IOCS_CALL_SCROLL    0x1d
#define IOCS_CALL_B_CURON   0x1e
#define IOCS_CALL_B_CUROFF  0x1f
#define IOCS_CALL_B_PUTC    0x20
#define IOCS_CALL_B_PRINT   0x21
#define IOCS_CALL_B_COLOR   0x22
#define IOCS_CALL_B_LOCATE  0x23
#define IOCS_CALL_B_DOWN_S  0x24
#define IOCS_CALL_B_UP_S    0x25
#define IOCS_CALL_B_UP      0x26
#define IOCS_CALL_B_DOWN    0x27
#define IOCS_CALL_B_RIGHT   0x28
#define IOCS_CALL_B_LEFT    0x29
#define IOCS_CALL_B_CLR_ST  0x2a
#define IOCS_CALL_B_ERA_ST  0x2b
#define IOCS_CALL_B_INS     0x2c
#define IOCS_CALL_B_DEL     0x2d
#define IOCS_CALL_B_CONSOL  0x2e
#define IOCS_CALL_B_PUTMES  0x2f
#define IOCS_CALL_SET232C   0x30
#define IOCS_CALL_LOF232C   0x31
#define IOCS_CALL_INP232C   0x32
#define IOCS_CALL_ISNS232C  0x33
#define IOCS_CALL_OSNS232C  0x34
#define IOCS_CALL_OUT232C   0x35
#define IOCS_CALL_SETFNTADR 0x38
#define IOCS_CALL_JOYGET    0x3b
#define IOCS_CALL_INIT_PRN  0x3c
#define IOCS_CALL_SNSPRN    0x3d
#define IOCS_CALL_OUTLPT    0x3e
#define IOCS_CALL_OUTPRN    0x3f
#define IOCS_CALL_B_SEEK    0x40
#define IOCS_CALL_B_VERIFY  0x41
#define IOCS_CALL_B_READDI  0x42
#define IOCS_CALL_B_DSKINI  0x43
#define IOCS_CALL_B_DRVSNS  0x44
#define IOCS_CALL_B_WRITE   0x45
#define IOCS_CALL_B_READ    0x46
#define IOCS_CALL_B_RECALI  0x47
#define IOCS_CALL_B_ASSIGN  0x48
#define IOCS_CALL_B_WRITED  0x49
#define IOCS_CALL_B_READID  0x4a
#define IOCS_CALL_B_BADFMT  0x4b
#define IOCS_CALL_B_READDL  0x4c
#define IOCS_CALL_B_FORMAT  0x4d
#define IOCS_CALL_B_DRVCHK  0x4e
#define IOCS_CALL_B_EJECT   0x4f
#define IOCS_CALL_DATEBCD   0x50
#define IOCS_CALL_DATESET   0x51
#define IOCS_CALL_TIMEBCD   0x52
#define IOCS_CALL_TIMESET   0x53
#define IOCS_CALL_DATEGET   0x54
#define IOCS_CALL_DATEBIN   0x55
#define IOCS_CALL_TIMEGET   0x56
#define IOCS_CALL_TIMEBIN   0x57
#define IOCS_CALL_DATECNV   0x58
#define IOCS_CALL_TIMECNV   0x59
#define IOCS_CALL_DATEASC   0x5a
#define IOCS_CALL_TIMEASC   0x5b
#define IOCS_CALL_DAYASC    0x5c
#define IOCS_CALL_ALARMMOD  0x5d
#define IOCS_CALL_ALARMSET  0x5e
#define IOCS_CALL_ALARMGET  0x5f
#define IOCS_CALL_ADPCMOUT  0x60
#define IOCS_CALL_ADPCMINP  0x61
#define IOCS_CALL_ADPCMAOT  0x62
#define IOCS_CALL_ADPCMAIN  0x63
#define IOCS_CALL_ADPCMLOT  0x64
#define IOCS_CALL_ADPCMLIN  0x65
#define IOCS_CALL_ADPCMSNS  0x66
#define IOCS_CALL_ADPCMMOD  0x67
#define IOCS_CALL_OPMSET    0x68
#define IOCS_CALL_OPMSNS    0x69
#define IOCS_CALL_OPMINTST  0x6a
#define IOCS_CALL_TIMERDST  0x6b
#define IOCS_CALL_VDISPST   0x6c
#define IOCS_CALL_CRTCRAS   0x6d
#define IOCS_CALL_HSYNCST   0x6e
#define IOCS_CALL_PRNINTST  0x6f
#define IOCS_CALL_MS_INIT   0x70
#define IOCS_CALL_MS_CURON  0x71
#define IOCS_CALL_MS_CUROF  0x72
#define IOCS_CALL_MS_STAT   0x73
#define IOCS_CALL_MS_GETDT  0x74
#define IOCS_CALL_MS_CURGT  0x75
#define IOCS_CALL_MS_CURST  0x76
#define IOCS_CALL_MS_LIMIT  0x77
#define IOCS_CALL_MS_OFFTM  0x78
#define IOCS_CALL_MS_ONTM   0x79
#define IOCS_CALL_MS_PATST  0x7a
#define IOCS_CALL_MS_SEL    0x7b
#define IOCS_CALL_MS_SEL2   0x7c
#define IOCS_CALL_SKEY_MOD  0x7d
#define IOCS_CALL_DENSNS    0x7e
#define IOCS_CALL_ONTIME    0x7f
#define IOCS_CALL_B_INTVCS  0x80
#define IOCS_CALL_B_SUPER   0x81
#define IOCS_CALL_B_BPEEK   0x82
#define IOCS_CALL_B_WPEEK   0x83
#define IOCS_CALL_B_LPEEK   0x84
#define IOCS_CALL_B_MEMSTR  0x85
#define IOCS_CALL_B_BPOKE   0x86
#define IOCS_CALL_B_WPOKE   0x87
#define IOCS_CALL_B_LPOKE   0x88
#define IOCS_CALL_B_MEMSET  0x89
#define IOCS_CALL_DMAMOVE   0x8a
#define IOCS_CALL_DMAMOV_A  0x8b
#define IOCS_CALL_DMAMOV_L  0x8c
#define IOCS_CALL_DMAMODE   0x8d
#define IOCS_CALL_BOOTINF   0x8e
#define IOCS_CALL_ROMVER    0x8f
#define IOCS_CALL_G_CLR_ON  0x90
#define IOCS_CALL_GPALET    0x94
#define IOCS_CALL_SFTJIS    0xa0
#define IOCS_CALL_JISSFT    0xa1
#define IOCS_CALL_AKCONV    0xa2
#define IOCS_CALL_RMACNV    0xa3
#define IOCS_CALL_DAKJOB    0xa4
#define IOCS_CALL_HANJOB    0xa5
#define IOCS_CALL_SYS_STAT  0xac
#define IOCS_CALL_B_CONMOD  0xad
#define IOCS_CALL_OS_CURON  0xae
#define IOCS_CALL_OS_CUROF  0xaf
#define IOCS_CALL_DRAWMODE  0xb0
#define IOCS_CALL_APAGE     0xb1
#define IOCS_CALL_VPAGE     0xb2
#define IOCS_CALL_HOME      0xb3
#define IOCS_CALL_WINDOW    0xb4
#define IOCS_CALL_WIPE      0xb5
#define IOCS_CALL_PSET      0xb6
#define IOCS_CALL_POINT     0xb7
#define IOCS_CALL_LINE      0xb8
#define IOCS_CALL_BOX       0xb9
#define IOCS_CALL_FILL      0xba
#define IOCS_CALL_CIRCLE    0xbb
#define IOCS_CALL_PAINT     0xbc
#define IOCS_CALL_SYMBOL    0xbd
#define IOCS_CALL_GETGRM    0xbe
#define IOCS_CALL_PUTGRM    0xbf
#define IOCS_CALL_SP_INIT   0xc0
#define IOCS_CALL_SP_ON     0xc1
#define IOCS_CALL_SP_OFF    0xc2
#define IOCS_CALL_SP_CGCLR  0xc3
#define IOCS_CALL_SP_DEFCG  0xc4
#define IOCS_CALL_SP_GTPCG  0xc5
#define IOCS_CALL_SP_REGST  0xc6
#define IOCS_CALL_SP_REGGT  0xc7
#define IOCS_CALL_BGSCRLST  0xc8
#define IOCS_CALL_BGSCRLGT  0xc9
#define IOCS_CALL_BGCTRLST  0xca
#define IOCS_CALL_BGCTRLGT  0xcb
#define IOCS_CALL_BGTEXTCL  0xcc
#define IOCS_CALL_BGTEXTST  0xcd
#define IOCS_CALL_BGTEXTGT  0xce
#define IOCS_CALL_SPALET    0xcf
#define IOCS_CALL_TXXLINE   0xd3
#define IOCS_CALL_TXYLINE   0xd4
#define IOCS_CALL_TXLINE    0xd5
#define IOCS_CALL_TXBOX     0xd6
#define IOCS_CALL_TXFILL    0xd7
#define IOCS_CALL_TXREV     0xd8
#define IOCS_CALL_OPMDRV    0xf0
#define IOCS_CALL_RSDRV     0xf1
#define IOCS_CALL_A_JOYGET  0xf2
#define IOCS_CALL_MUSICDRV  0xf3
#define IOCS_CALL_SCSIDRV   0xf5
#define IOCS_CALL_ABORTRST  0xfd
#define IOCS_CALL_IPLERR    0xfe
#define IOCS_CALL_ABORTJOB  0xff

static const char *iocs_call_names[256] = {
	/* 0x00 */ "_B_KEYINP",
	/* 0x01 */ "_B_KEYSNS",
	/* 0x02 */ "_B_SFTSNS",
	/* 0x03 */ "_KEY_INIT",
	/* 0x04 */ "_BITSNS",
	/* 0x05 */ "_SKEYSET",
	/* 0x06 */ "_LEDCTRL",
	/* 0x07 */ "_LEDSET",
	/* 0x08 */ "_KEYDLY",
	/* 0x09 */ "_KEYREP",
	/* 0x0a */ "?",
	/* 0x0b */ "?",
	/* 0x0c */ "_TVCTRL",
	/* 0x0d */ "_LEDMOD",
	/* 0x0e */ "_TGUSEMD",
	/* 0x0f */ "_DEFCHR",
	/* 0x10 */ "_CRTMOD",
	/* 0x11 */ "_CONTRAST",
	/* 0x12 */ "_HSVTORGB",
	/* 0x13 */ "_TPALET",
	/* 0x14 */ "_TPALET2",
	/* 0x15 */ "_TCOLOR",
	/* 0x16 */ "_FNTADR",
	/* 0x17 */ "_VRAMGET",
	/* 0x18 */ "_VRAMPUT",
	/* 0x19 */ "_FNTGET",
	/* 0x1a */ "_TEXTGET",
	/* 0x1b */ "_TEXTPUT",
	/* 0x1c */ "_CLIPPUT",
	/* 0x1d */ "_SCROLL",
	/* 0x1e */ "_B_CURON",
	/* 0x1f */ "_B_CUROFF",
	/* 0x20 */ "_B_PUTC",
	/* 0x21 */ "_B_PRINT",
	/* 0x22 */ "_B_COLOR",
	/* 0x23 */ "_B_LOCATE",
	/* 0x24 */ "_B_DOWN_S",
	/* 0x25 */ "_B_UP_S",
	/* 0x26 */ "_B_UP",
	/* 0x27 */ "_B_DOWN",
	/* 0x28 */ "_B_RIGHT",
	/* 0x29 */ "_B_LEFT",
	/* 0x2a */ "_B_CLR_ST",
	/* 0x2b */ "_B_ERA_ST",
	/* 0x2c */ "_B_INS",
	/* 0x2d */ "_B_DEL",
	/* 0x2e */ "_B_CONSOL",
	/* 0x2f */ "_B_PUTMES",
	/* 0x30 */ "_SET232C",
	/* 0x31 */ "_LOF232C",
	/* 0x32 */ "_INP232C",
	/* 0x33 */ "_ISNS232C",
	/* 0x34 */ "_OSNS232C",
	/* 0x35 */ "_OUT232C",
	/* 0x36 */ "?",
	/* 0x37 */ "?",
	/* 0x38 */ "_SETFNTADR",
	/* 0x39 */ "?",
	/* 0x3a */ "?",
	/* 0x3b */ "_JOYGET",
	/* 0x3c */ "_INIT_PRN",
	/* 0x3d */ "_SNSPRN",
	/* 0x3e */ "_OUTLPT",
	/* 0x3f */ "_OUTPRN",
	/* 0x40 */ "_B_SEEK",
	/* 0x41 */ "_B_VERIFY",
	/* 0x42 */ "_B_READDI",
	/* 0x43 */ "_B_DSKINI",
	/* 0x44 */ "_B_DRVSNS",
	/* 0x45 */ "_B_WRITE",
	/* 0x46 */ "_B_READ",
	/* 0x47 */ "_B_RECALI",
	/* 0x48 */ "_B_ASSIGN",
	/* 0x49 */ "_B_WRITED",
	/* 0x4a */ "_B_READID",
	/* 0x4b */ "_B_BADFMT",
	/* 0x4c */ "_B_READDL",
	/* 0x4d */ "_B_FORMAT",
	/* 0x4e */ "_B_DRVCHK",
	/* 0x4f */ "_B_EJECT",
	/* 0x50 */ "_DATEBCD",
	/* 0x51 */ "_DATESET",
	/* 0x52 */ "_TIMEBCD",
	/* 0x53 */ "_TIMESET",
	/* 0x54 */ "_DATEGET",
	/* 0x55 */ "_DATEBIN",
	/* 0x56 */ "_TIMEGET",
	/* 0x57 */ "_TIMEBIN",
	/* 0x58 */ "_DATECNV",
	/* 0x59 */ "_TIMECNV",
	/* 0x5a */ "_DATEASC",
	/* 0x5b */ "_TIMEASC",
	/* 0x5c */ "_DAYASC",
	/* 0x5d */ "_ALARMMOD",
	/* 0x5e */ "_ALARMSET",
	/* 0x5f */ "_ALARMGET",
	/* 0x60 */ "_ADPCMOUT",
	/* 0x61 */ "_ADPCMINP",
	/* 0x62 */ "_ADPCMAOT",
	/* 0x63 */ "_ADPCMAIN",
	/* 0x64 */ "_ADPCMLOT",
	/* 0x65 */ "_ADPCMLIN",
	/* 0x66 */ "_ADPCMSNS",
	/* 0x67 */ "_ADPCMMOD",
	/* 0x68 */ "_OPMSET",
	/* 0x69 */ "_OPMSNS",
	/* 0x6a */ "_OPMINTST",
	/* 0x6b */ "_TIMERDST",
	/* 0x6c */ "_VDISPST",
	/* 0x6d */ "_CRTCRAS",
	/* 0x6e */ "_HSYNCST",
	/* 0x6f */ "_PRNINTST",
	/* 0x70 */ "_MS_INIT",
	/* 0x71 */ "_MS_CURON",
	/* 0x72 */ "_MS_CUROF",
	/* 0x73 */ "_MS_STAT",
	/* 0x74 */ "_MS_GETDT",
	/* 0x75 */ "_MS_CURGT",
	/* 0x76 */ "_MS_CURST",
	/* 0x77 */ "_MS_LIMIT",
	/* 0x78 */ "_MS_OFFTM",
	/* 0x79 */ "_MS_ONTM",
	/* 0x7a */ "_MS_PATST",
	/* 0x7b */ "_MS_SEL",
	/* 0x7c */ "_MS_SEL2",
	/* 0x7d */ "_SKEY_MOD",
	/* 0x7e */ "_DENSNS",
	/* 0x7f */ "_ONTIME",
	/* 0x80 */ "_B_INTVCS",
	/* 0x81 */ "_B_SUPER",
	/* 0x82 */ "_B_BPEEK",
	/* 0x83 */ "_B_WPEEK",
	/* 0x84 */ "_B_LPEEK",
	/* 0x85 */ "_B_MEMSTR",
	/* 0x86 */ "_B_BPOKE",
	/* 0x87 */ "_B_WPOKE",
	/* 0x88 */ "_B_LPOKE",
	/* 0x89 */ "_B_MEMSET",
	/* 0x8a */ "_DMAMOVE",
	/* 0x8b */ "_DMAMOV_A",
	/* 0x8c */ "_DMAMOV_L",
	/* 0x8d */ "_DMAMODE",
	/* 0x8e */ "_BOOTINF",
	/* 0x8f */ "_ROMVER",
	/* 0x90 */ "_G_CLR_ON",
	/* 0x91 */ "?",
	/* 0x92 */ "?",
	/* 0x93 */ "?",
	/* 0x94 */ "_GPALET",
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
	/* 0xa0 */ "_SFTJIS",
	/* 0xa1 */ "_JISSFT",
	/* 0xa2 */ "_AKCONV",
	/* 0xa3 */ "_RMACNV",
	/* 0xa4 */ "_DAKJOB",
	/* 0xa5 */ "_HANJOB",
	/* 0xa6 */ "?",
	/* 0xa7 */ "?",
	/* 0xa8 */ "?",
	/* 0xa9 */ "?",
	/* 0xaa */ "?",
	/* 0xab */ "?",
	/* 0xac */ "_SYS_STAT",
	/* 0xad */ "_B_CONMOD",
	/* 0xae */ "_OS_CURON",
	/* 0xaf */ "_OS_CUROF",
	/* 0xb0 */ "_DRAWMODE",
	/* 0xb1 */ "_APAGE",
	/* 0xb2 */ "_VPAGE",
	/* 0xb3 */ "_HOME",
	/* 0xb4 */ "_WINDOW",
	/* 0xb5 */ "_WIPE",
	/* 0xb6 */ "_PSET",
	/* 0xb7 */ "_POINT",
	/* 0xb8 */ "_LINE",
	/* 0xb9 */ "_BOX",
	/* 0xba */ "_FILL",
	/* 0xbb */ "_CIRCLE",
	/* 0xbc */ "_PAINT",
	/* 0xbd */ "_SYMBOL",
	/* 0xbe */ "_GETGRM",
	/* 0xbf */ "_PUTGRM",
	/* 0xc0 */ "_SP_INIT",
	/* 0xc1 */ "_SP_ON",
	/* 0xc2 */ "_SP_OFF",
	/* 0xc3 */ "_SP_CGCLR",
	/* 0xc4 */ "_SP_DEFCG",
	/* 0xc5 */ "_SP_GTPCG",
	/* 0xc6 */ "_SP_REGST",
	/* 0xc7 */ "_SP_REGGT",
	/* 0xc8 */ "_BGSCRLST",
	/* 0xc9 */ "_BGSCRLGT",
	/* 0xca */ "_BGCTRLST",
	/* 0xcb */ "_BGCTRLGT",
	/* 0xcc */ "_BGTEXTCL",
	/* 0xcd */ "_BGTEXTST",
	/* 0xce */ "_BGTEXTGT",
	/* 0xcf */ "_SPALET",
	/* 0xd0 */ "?",
	/* 0xd1 */ "?",
	/* 0xd2 */ "?",
	/* 0xd3 */ "_TXXLINE",
	/* 0xd4 */ "_TXYLINE",
	/* 0xd5 */ "_TXLINE",
	/* 0xd6 */ "_TXBOX",
	/* 0xd7 */ "_TXFILL",
	/* 0xd8 */ "_TXREV",
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
	/* 0xf0 */ "_OPMDRV",
	/* 0xf1 */ "_RSDRV",
	/* 0xf2 */ "_A_JOYGET",
	/* 0xf3 */ "_MUSICDRV",
	/* 0xf4 */ "?",
	/* 0xf5 */ "_SCSIDRV",
	/* 0xf6 */ "?",
	/* 0xf7 */ "?",
	/* 0xf8 */ "?",
	/* 0xf9 */ "?",
	/* 0xfa */ "?",
	/* 0xfb */ "?",
	/* 0xfc */ "?",
	/* 0xfd */ "_ABORTRST",
	/* 0xfe */ "_IPLERR",
	/* 0xff */ "_ABORTJOB",
};

void v68_iocs_init() {

}

int v68_iocs_call(uint16_t instr) {
	uint8_t call = instr & 0xff;
	uint32_t call_addr = m68k_read_memory_32(0x400 + call * 4);

	if(v68.log_calls)
		printf("v68_iocs_call call=0x%02x %s call_addr=0x%08x\n", instr, iocs_call_names[call], call_addr);

	if(call_addr < 0xff0000) {/* Only implement calls that aren't overridden */
		uint16_t sr = m68k_get_reg(0, M68K_REG_SR);
		uint32_t sp = m68k_get_reg(0, M68K_REG_SP);
		verbose2("v68_iocs_call call_addr=0x%08x implemented sp=0x%08x\n", call_addr, sp);
		sp -= 4;
		m68k_set_reg(M68K_REG_SR, sr | 0x2000);
		uint32_t pc = m68k_get_reg(0, M68K_REG_PC);
		m68k_write_memory_32(sp, pc);
		m68k_set_reg(M68K_REG_SP, sp);
		m68k_set_reg(M68K_REG_PC, call_addr);
		return 0;
	}

	switch(call) {
		case IOCS_CALL_B_PUTC: {
				uint16_t chr = m68k_get_reg(0, M68K_REG_D0);
				uint8_t chrbuf[2] = { chr >> 8, chr };
				sjis_print_utf8(chrbuf[0] ? chrbuf : chrbuf+1, chrbuf[0] ? 2 : 1);
				m68k_set_reg(M68K_REG_D0, 0x00100010);
			}
			break;
		case IOCS_CALL_ONTIME: {
#if __linux__
				struct sysinfo si;
				sysinfo(&si);
				m68k_set_reg(M68K_REG_D0, (si.uptime % (24 * 60 * 60)) * 100);
				m68k_set_reg(M68K_REG_D1, si.uptime / (24 * 60 * 60));
#endif
#ifdef WIN32
				DWORD u = GetTickCount();
				u /= 1000;
				printf("GetTickCount=%lu\n", u);
				m68k_set_reg(M68K_REG_D0, (u % (24 * 60 * 60)) * 100);
				m68k_set_reg(M68K_REG_D1, u / (24 * 60 * 60));
#endif
			}
			break;
		case IOCS_CALL_B_MEMSTR: {
				uint32_t num_bytes = m68k_get_reg(0, M68K_REG_D1);
				uint32_t src = m68k_get_reg(0, M68K_REG_A1);
				uint32_t dst = m68k_get_reg(0, M68K_REG_A2);
				if(src + num_bytes >= v68.ram_size || dst + num_bytes >= v68.ram_size) {
					// Out of bounds, fool
					m68k_set_reg(M68K_REG_A1, 0);
					m68k_set_reg(M68K_REG_A2, 0);
				} else {
					memcpy(&v68.ram[dst], &v68.ram[src], num_bytes);
					m68k_set_reg(M68K_REG_A1, src + num_bytes);
					m68k_set_reg(M68K_REG_A2, 0);
				}
			}
			break;
		case IOCS_CALL_ADPCMOUT: {
				verbose1("_ADPCMOUT\n");
				m68k_write_memory_8(0x00E840C5, 0x32); // Channel 3, OCR
				m68k_write_memory_8(0x00E840C0, 0xff); // Channel 3, CSR
				m68k_write_memory_32(0x00E840CC, m68k_get_reg(0, M68K_REG_A1)); // Channel 3, MAR
				m68k_write_memory_16(0x00E840CA, m68k_get_reg(0, M68K_REG_D2)); // Channel 3, MTC
				m68k_write_memory_8(0x00E840C7, 0x88); // Channel 3, CCR
				m68k_write_memory_8(0xe9a005, 0x08); // Write to UPD8255
				m68k_write_memory_8(0xE92001, 0x02); // OKI ADPCM: play start
			}
			break;
		case IOCS_CALL_ADPCMINP: {
				verbose1("_ADPCMINP\n");
			}
			break;
		case IOCS_CALL_ADPCMAOT: {
				verbose1("_ADPCMAOT\n");
			}
			break;
		case IOCS_CALL_ADPCMAIN: {
				verbose1("_ADPCMAIN\n");
			}
			break;
		case IOCS_CALL_ADPCMLOT: {
				verbose1("_ADPCMLOT\n");
			}
			break;
		case IOCS_CALL_ADPCMLIN: {
				verbose1("_ADPCMLIN\n");
			}
			break;
		case IOCS_CALL_ADPCMSNS: {
				verbose1("_ADPCMSNS\n");
			}
			break;
		case IOCS_CALL_ADPCMMOD: {
				verbose1("_ADPCMMOD\n");
			}
			break;

		default:
			logcall("Unimplemented V68 IOCS CALL %04x %s\n", instr, iocs_call_names[call]);
	}

	return 0;
}
