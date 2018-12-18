#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "v68.h"
#include "v68io.h"
#include "musashi/m68k.h"
#include "musashi/m68kcpu.h"
#include "sjis.h"

#define FE_CALL_LMUL      0x00
#define FE_CALL_LDIV      0x01
#define FE_CALL_LMOD      0x02
#define FE_CALL_UMUL      0x04
#define FE_CALL_UDIV      0x05
#define FE_CALL_UMOD      0x06
#define FE_CALL_IMUL      0x08
#define FE_CALL_IDIV      0x09
#define FE_CALL_RANDOMIZE 0x0c
#define FE_CALL_SRAND     0x0d
#define FE_CALL_RAND      0x0e
#define FE_CALL_STOL      0x10
#define FE_CALL_LTOS      0x11
#define FE_CALL_STOH      0x12
#define FE_CALL_HTOS      0x13
#define FE_CALL_STOO      0x14
#define FE_CALL_OTOS      0x15
#define FE_CALL_STOB      0x16
#define FE_CALL_BTOS      0x17
#define FE_CALL_IUSING    0x18
#define FE_CALL_LTOD      0x1a
#define FE_CALL_DTOL      0x1b
#define FE_CALL_LTOF      0x1c
#define FE_CALL_FTOL      0x1d
#define FE_CALL_FTOD      0x1e
#define FE_CALL_DTOF      0x1f
#define FE_CALL_VAL       0x20
#define FE_CALL_USING     0x21
#define FE_CALL_STOD      0x22
#define FE_CALL_DTOS      0x23
#define FE_CALL_ECVT      0x24
#define FE_CALL_FCVT      0x25
#define FE_CALL_GCVT      0x26
#define FE_CALL_DTST      0x28
#define FE_CALL_DCMP      0x29
#define FE_CALL_DNEG      0x2a
#define FE_CALL_DADD      0x2b
#define FE_CALL_DSUB      0x2c
#define FE_CALL_DMUL      0x2d
#define FE_CALL_DDIV      0x2e
#define FE_CALL_DMOD      0x2f
#define FE_CALL_DABS      0x30
#define FE_CALL_DCEIL     0x31
#define FE_CALL_DFIX      0x32
#define FE_CALL_DFLOOR    0x33
#define FE_CALL_DFRAC     0x34
#define FE_CALL_DSGN      0x35
#define FE_CALL_SIN       0x36
#define FE_CALL_COS       0x37
#define FE_CALL_TAN       0x38
#define FE_CALL_ATAN      0x39
#define FE_CALL_LOG       0x3a
#define FE_CALL_EXP       0x3b
#define FE_CALL_SQR       0x3c
#define FE_CALL_PI        0x3d
#define FE_CALL_NPI       0x3e
#define FE_CALL_POWER     0x3f
#define FE_CALL_RND       0x40
#define FE_CALL_SINH      0x41
#define FE_CALL_COSH      0x42
#define FE_CALL_TANH      0x43
#define FE_CALL_ATANH     0x44
#define FE_CALL_ASIN      0x45
#define FE_CALL_ACOS      0x46
#define FE_CALL_LOG10     0x47
#define FE_CALL_LOG2      0x48
#define FE_CALL_DFREXP    0x49
#define FE_CALL_DLDEXP    0x4a
#define FE_CALL_DADDONE   0x4b
#define FE_CALL_DSUBONE   0x4c
#define FE_CALL_DDIVTWO   0x4d
#define FE_CALL_DIEECNV   0x4e
#define FE_CALL_IEEDCNV   0x4f
#define FE_CALL_FVAL      0x50
#define FE_CALL_FUSING    0x51
#define FE_CALL_STOF      0x52
#define FE_CALL_FTOS      0x53
#define FE_CALL_FECVT     0x54
#define FE_CALL_FFCVT     0x55
#define FE_CALL_FGCVT     0x56
#define FE_CALL_FTST      0x58
#define FE_CALL_FCMP      0x59
#define FE_CALL_FNEG      0x5a
#define FE_CALL_FADD      0x5b
#define FE_CALL_FSUB      0x5c
#define FE_CALL_FMUL      0x5d
#define FE_CALL_FDIV      0x5e
#define FE_CALL_FMOD      0x5f
#define FE_CALL_FABS      0x60
#define FE_CALL_FCEIL     0x61
#define FE_CALL_FFIX      0x62
#define FE_CALL_FFLOOR    0x63
#define FE_CALL_FFRAC     0x64
#define FE_CALL_FSGN      0x65
#define FE_CALL_FSIN      0x66
#define FE_CALL_FCOS      0x67
#define FE_CALL_FTAN      0x68
#define FE_CALL_FATAN     0x69
#define FE_CALL_FLOG      0x6a
#define FE_CALL_FEXP      0x6b
#define FE_CALL_FSQR      0x6c
#define FE_CALL_FPI       0x6d
#define FE_CALL_FNPI      0x6e
#define FE_CALL_FPOWER    0x6f
#define FE_CALL_FRND      0x70
#define FE_CALL_FSINH     0x71
#define FE_CALL_FCOSH     0x72
#define FE_CALL_FTANH     0x73
#define FE_CALL_FATANH    0x74
#define FE_CALL_FASIN     0x75
#define FE_CALL_FACOS     0x76
#define FE_CALL_FLOG10    0x77
#define FE_CALL_FLOG2     0x78
#define FE_CALL_FFREXP    0x79
#define FE_CALL_FLDEXP    0x7a
#define FE_CALL_FADDONE   0x7b
#define FE_CALL_FSUBONE   0x7c
#define FE_CALL_FDIVTWO   0x7d
#define FE_CALL_FIEECNV   0x7e
#define FE_CALL_IEEFCNV   0x7f
#define FE_CALL_CLMUL     0xe0
#define FE_CALL_CLDIV     0xe1
#define FE_CALL_CLMOD     0xe2
#define FE_CALL_CUMUL     0xe3
#define FE_CALL_CUDIV     0xe4
#define FE_CALL_CUMOD     0xe5
#define FE_CALL_CLTOD     0xe6
#define FE_CALL_CDTOL     0xe7
#define FE_CALL_CLTOF     0xe8
#define FE_CALL_CFTOL     0xe9
#define FE_CALL_CFTOD     0xea
#define FE_CALL_CDTOF     0xeb
#define FE_CALL_CDCMP     0xec
#define FE_CALL_CDADD     0xed
#define FE_CALL_CDSUB     0xee
#define FE_CALL_CDMUL     0xef
#define FE_CALL_CDDIV     0xf0
#define FE_CALL_CDMOD     0xf1
#define FE_CALL_CFCMP     0xf2
#define FE_CALL_CFADD     0xf3
#define FE_CALL_CFSUB     0xf4
#define FE_CALL_CFMUL     0xf5
#define FE_CALL_CFDIV     0xf6
#define FE_CALL_CFMOD     0xf7
#define FE_CALL_CDTST     0xf8
#define FE_CALL_CFTST     0xf9
#define FE_CALL_CDINC     0xfa
#define FE_CALL_CFINC     0xfb
#define FE_CALL_CDDEC     0xfc
#define FE_CALL_CFDEC     0xfd
#define FE_CALL_FEVARG    0xfe
#define FE_CALL_FEVECS    0xff

static const char *fe_call_names[256] = {
	/* fe00 */ "LMUL",
	/* fe01 */ "LDIV",
	/* fe02 */ "LMOD",
	/* fe03 */ "?",
	/* fe04 */ "UMUL",
	/* fe05 */ "UDIV",
	/* fe06 */ "UMOD",
	/* fe07 */ "?",
	/* fe08 */ "IMUL",
	/* fe09 */ "IDIV",
	/* fe0a */ "?",
	/* fe0b */ "?",
	/* fe0c */ "RANDOMIZE",
	/* fe0d */ "SRAND",
	/* fe0e */ "RAND",
	/* fe0f */ "?",
	/* fe10 */ "STOL",
	/* fe11 */ "LTOS",
	/* fe12 */ "STOH",
	/* fe13 */ "HTOS",
	/* fe14 */ "STOO",
	/* fe15 */ "OTOS",
	/* fe16 */ "STOB",
	/* fe17 */ "BTOS",
	/* fe18 */ "IUSING",
	/* fe19 */ "?",
	/* fe1a */ "LTOD",
	/* fe1b */ "DTOL",
	/* fe1c */ "LTOF",
	/* fe1d */ "FTOL",
	/* fe1e */ "FTOD",
	/* fe1f */ "DTOF",
	/* fe20 */ "VAL",
	/* fe21 */ "USING",
	/* fe22 */ "STOD",
	/* fe23 */ "DTOS",
	/* fe24 */ "ECVT",
	/* fe25 */ "FCVT",
	/* fe26 */ "GCVT",
	/* fe27 */ "?",
	/* fe28 */ "DTST",
	/* fe29 */ "DCMP",
	/* fe2a */ "DNEG",
	/* fe2b */ "DADD",
	/* fe2c */ "DSUB",
	/* fe2d */ "DMUL",
	/* fe2e */ "DDIV",
	/* fe2f */ "DMOD",
	/* fe30 */ "DABS",
	/* fe31 */ "DCEIL",
	/* fe32 */ "DFIX",
	/* fe33 */ "DFLOOR",
	/* fe34 */ "DFRAC",
	/* fe35 */ "DSGN",
	/* fe36 */ "SIN",
	/* fe37 */ "COS",
	/* fe38 */ "TAN",
	/* fe39 */ "ATAN",
	/* fe3a */ "LOG",
	/* fe3b */ "EXP",
	/* fe3c */ "SQR",
	/* fe3d */ "PI",
	/* fe3e */ "NPI",
	/* fe3f */ "POWER",
	/* fe40 */ "RND",
	/* fe41 */ "SINH",
	/* fe42 */ "COSH",
	/* fe43 */ "TANH",
	/* fe44 */ "ATANH",
	/* fe45 */ "ASIN",
	/* fe46 */ "ACOS",
	/* fe47 */ "LOG10",
	/* fe48 */ "LOG2",
	/* fe49 */ "DFREXP",
	/* fe4a */ "DLDEXP",
	/* fe4b */ "DADDONE",
	/* fe4c */ "DSUBONE",
	/* fe4d */ "DDIVTWO",
	/* fe4e */ "DIEECNV",
	/* fe4f */ "IEEDCNV",
	/* fe50 */ "FVAL",
	/* fe51 */ "FUSING",
	/* fe52 */ "STOF",
	/* fe53 */ "FTOS",
	/* fe54 */ "FECVT",
	/* fe55 */ "FFCVT",
	/* fe56 */ "FGCVT",
	/* fe57 */ "?",
	/* fe58 */ "FTST",
	/* fe59 */ "FCMP",
	/* fe5a */ "FNEG",
	/* fe5b */ "FADD",
	/* fe5c */ "FSUB",
	/* fe5d */ "FMUL",
	/* fe5e */ "FDIV",
	/* fe5f */ "FMOD",
	/* fe60 */ "FABS",
	/* fe61 */ "FCEIL",
	/* fe62 */ "FFIX",
	/* fe63 */ "FFLOOR",
	/* fe64 */ "FFRAC",
	/* fe65 */ "FSGN",
	/* fe66 */ "FSIN",
	/* fe67 */ "FCOS",
	/* fe68 */ "FTAN",
	/* fe69 */ "FATAN",
	/* fe6a */ "FLOG",
	/* fe6b */ "FEXP",
	/* fe6c */ "FSQR",
	/* fe6d */ "FPI",
	/* fe6e */ "FNPI",
	/* fe6f */ "FPOWER",
	/* fe70 */ "FRND",
	/* fe71 */ "FSINH",
	/* fe72 */ "FCOSH",
	/* fe73 */ "FTANH",
	/* fe74 */ "FATANH",
	/* fe75 */ "FASIN",
	/* fe76 */ "FACOS",
	/* fe77 */ "FLOG10",
	/* fe78 */ "FLOG2",
	/* fe79 */ "FFREXP",
	/* fe7a */ "FLDEXP",
	/* fe7b */ "FADDONE",
	/* fe7c */ "FSUBONE",
	/* fe7d */ "FDIVTWO",
	/* fe7e */ "FIEECNV",
	/* fe7f */ "IEEFCNV",
	/* fe80 */ "?",
	/* fe81 */ "?",
	/* fe82 */ "?",
	/* fe83 */ "?",
	/* fe84 */ "?",
	/* fe85 */ "?",
	/* fe86 */ "?",
	/* fe87 */ "?",
	/* fe88 */ "?",
	/* fe89 */ "?",
	/* fe8a */ "?",
	/* fe8b */ "?",
	/* fe8c */ "?",
	/* fe8d */ "?",
	/* fe8e */ "?",
	/* fe8f */ "?",
	/* fe90 */ "?",
	/* fe91 */ "?",
	/* fe92 */ "?",
	/* fe93 */ "?",
	/* fe94 */ "?",
	/* fe95 */ "?",
	/* fe96 */ "?",
	/* fe97 */ "?",
	/* fe98 */ "?",
	/* fe99 */ "?",
	/* fe9a */ "?",
	/* fe9b */ "?",
	/* fe9c */ "?",
	/* fe9d */ "?",
	/* fe9e */ "?",
	/* fe9f */ "?",
	/* fea0 */ "?",
	/* fea1 */ "?",
	/* fea2 */ "?",
	/* fea3 */ "?",
	/* fea4 */ "?",
	/* fea5 */ "?",
	/* fea6 */ "?",
	/* fea7 */ "?",
	/* fea8 */ "?",
	/* fea9 */ "?",
	/* feaa */ "?",
	/* feab */ "?",
	/* feac */ "?",
	/* fead */ "?",
	/* feae */ "?",
	/* feaf */ "?",
	/* feb0 */ "?",
	/* feb1 */ "?",
	/* feb2 */ "?",
	/* feb3 */ "?",
	/* feb4 */ "?",
	/* feb5 */ "?",
	/* feb6 */ "?",
	/* feb7 */ "?",
	/* feb8 */ "?",
	/* feb9 */ "?",
	/* feba */ "?",
	/* febb */ "?",
	/* febc */ "?",
	/* febd */ "?",
	/* febe */ "?",
	/* febf */ "?",
	/* fec0 */ "?",
	/* fec1 */ "?",
	/* fec2 */ "?",
	/* fec3 */ "?",
	/* fec4 */ "?",
	/* fec5 */ "?",
	/* fec6 */ "?",
	/* fec7 */ "?",
	/* fec8 */ "?",
	/* fec9 */ "?",
	/* feca */ "?",
	/* fecb */ "?",
	/* fecc */ "?",
	/* fecd */ "?",
	/* fece */ "?",
	/* fecf */ "?",
	/* fed0 */ "?",
	/* fed1 */ "?",
	/* fed2 */ "?",
	/* fed3 */ "?",
	/* fed4 */ "?",
	/* fed5 */ "?",
	/* fed6 */ "?",
	/* fed7 */ "?",
	/* fed8 */ "?",
	/* fed9 */ "?",
	/* feda */ "?",
	/* fedb */ "?",
	/* fedc */ "?",
	/* fedd */ "?",
	/* fede */ "?",
	/* fedf */ "?",
	/* fee0 */ "CLMUL",
	/* fee1 */ "CLDIV",
	/* fee2 */ "CLMOD",
	/* fee3 */ "CUMUL",
	/* fee4 */ "CUDIV",
	/* fee5 */ "CUMOD",
	/* fee6 */ "CLTOD",
	/* fee7 */ "CDTOL",
	/* fee8 */ "CLTOF",
	/* fee9 */ "CFTOL",
	/* feea */ "CFTOD",
	/* feeb */ "CDTOF",
	/* feec */ "CDCMP",
	/* feed */ "CDADD",
	/* feee */ "CDSUB",
	/* feef */ "CDMUL",
	/* fef0 */ "CDDIV",
	/* fef1 */ "CDMOD",
	/* fef2 */ "CFCMP",
	/* fef3 */ "CFADD",
	/* fef4 */ "CFSUB",
	/* fef5 */ "CFMUL",
	/* fef6 */ "CFDIV",
	/* fef7 */ "CFMOD",
	/* fef8 */ "CDTST",
	/* fef9 */ "CFTST",
	/* fefa */ "CDINC",
	/* fefb */ "CFINC",
	/* fefc */ "CDDEC",
	/* fefd */ "CFDEC",
	/* fefe */ "FEVARG",
	/* feff */ "FEVECS",
};

int v68_fe_call(uint16_t instr) {
	uint8_t call = instr & 0xff;

	if(v68.log_calls)
		printf("V68 FE CALL %04x %s\n", instr, fe_call_names[call]);

	switch(call) {
		case FE_CALL_LTOS: {
				char *buf = (char *)&v68.ram[m68k_get_reg(0, M68K_REG_A0)];
				sprintf(buf, "%d", m68k_get_reg(0, M68K_REG_D0));
				m68k_set_reg(M68K_REG_A0, m68k_get_reg(0, M68K_REG_A0) + strlen(buf));
			}
			break;
		case FE_CALL_CLMOD: {
				uint32_t n = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t d = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), d ? n % d : 0);
				if(d == 0) FLAG_C = 1;
			}
			break;
		case FE_CALL_CLDIV: {
				uint32_t n = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t d = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), d ? n / d : 0);
				if(d == 0) FLAG_C = 1;
			}
			break;
		case FE_CALL_CLMUL: {
				uint32_t n = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t d = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), d ? n * d : 0);
				// if(d == 0) FLAG_C = 1;
			}
			break;
		case FE_CALL_CLTOD: {
				double d = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				uint32_t *p = (uint32_t *)&d;
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), *p++);
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), *p);
			}
			break;
		case FE_CALL_DTST: {
				double d = 0;
				uint32_t *p = (uint32_t *)&d;
				*p++ = m68k_get_reg(0, M68K_REG_D0);
				*p   = m68k_get_reg(0, M68K_REG_D1);
				FLAG_Z = d == 0;
				FLAG_N = d < 0;
			}
			break;
		case FE_CALL_POWER: {
				double d1 = 0, d2 = 0;
				uint32_t *p1 = (uint32_t *)&d1;
				uint32_t *p2 = (uint32_t *)&d2;
				*p1++ = m68k_get_reg(0, M68K_REG_D0);
				*p1   = m68k_get_reg(0, M68K_REG_D1);
				*p2++ = m68k_get_reg(0, M68K_REG_D2);
				*p2   = m68k_get_reg(0, M68K_REG_D3);
				double r = pow(d1, d2);
				uint32_t *p = (uint32_t *)&r;
				m68k_set_reg(M68K_REG_D0, *p++);
				m68k_set_reg(M68K_REG_D1, *p);
			}
			break;
		case FE_CALL_CDTOL: {
				double d = 0;
				uint32_t *p = (uint32_t *)&d;
				*p++ = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7));
				*p   = m68k_read_memory_32(m68k_get_reg(0, M68K_REG_A7) + 4);
				m68k_write_memory_32(m68k_get_reg(0, M68K_REG_A7), (int32_t)d);
			}
			break;
		case FE_CALL_IDIV: {
				uint32_t d0 = m68k_get_reg(0, M68K_REG_D0);
				uint32_t d1 = m68k_get_reg(0, M68K_REG_D1);
				if(d1 == 0) {
					FLAG_C = 1;
				} else {
					m68k_set_reg(M68K_REG_D0, d0 / d1);
					m68k_set_reg(M68K_REG_D1, d0 % d1);
				}
			}
			break;
		case FE_CALL_UMOD: {
				uint32_t d0 = m68k_get_reg(0, M68K_REG_D0);
				uint32_t d1 = m68k_get_reg(0, M68K_REG_D1);
				if(d1 == 0) {
					FLAG_C = 1;
				} else {
					m68k_set_reg(M68K_REG_D0, d0 % d1);
				}
			}
			break;
		default:
			printf("V68 FE CALL %04x %s\n", instr, fe_call_names[call]);
			break;
	}

	return 0;
}
