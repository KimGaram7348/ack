/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
#define RCSID3 "$Id$"

/*
 * Signetics 2650 keywords
 */
0,	REG,		R0,		"r0",
0,	REG,		R1,		"r1",
0,	REG,		R2,		"r2",
0,	REG,		R3,		"r3",
0,	CC,		0,		"eq",
0,	CC,		0,		"z",
0,	CC,		1,		"gt",
0,	CC,		2,		"lt",
0,	CC,		3,		"un",
0,	INDE,		0x6000,		"i",
0,	ZERO,		0x0,		"lodz",
0,	ZERO,		0x80,		"addz",
0,	ZERO,		0xA0,		"subz",
0,	ZERO,		0x94,		"dar",
0,	ZEROX,		0x40,		"andz",
0,	ZERO,		0x60,		"iorz",
0,	ZERO,		0x20,		"eorz",
0,	ZERO,		0xE0,		"comz",
0,	ZERO,		0xF0,		"wrtd",
0,	ZERO,		0x70,		"redd",
0,	ZERO,		0xB0,		"wrtc",
0,	ZERO,		0x30,		"redc",
0,	ZERO,		0x50,		"rrr",
0,	ZERO,		0xD0,		"rrl",
0,	ZEROX,		0xC0,		"strz",
0,	IMMED,		0x04,		"lodi",
0,	IMMED,		0x84,		"addi",
0,	IMMED,		0xA4,		"subi",
0,	IMMED,		0x44,		"andi",
0,	IMMED,		0x64,		"iori",
0,	IMMED,		0x24,		"eori",
0,	IMMED,		0xE4,		"comi",
0,	IMMED,		0xD4,		"wrte",
0,	IMMED,		0x54,		"rede",
0,	IMMED,		0xF4,		"tmi",
0,	NOOP,		0x1E,		"halt",
0,	NOOP,		0xC0,		"nop",
0,	NOOP,		0x92,		"lpsu",
0,	NOOP,		0x93,		"lpsl",
0,	NOOP,		0x12,		"spsu",
0,	NOOP,		0x13,		"spsl",
0,	REL,		0x08,		"lodr",
0,	REL,		0xC8,		"strr",
0,	REL,		0x98,		"addr",
0,	REL,		0xA8,		"subr",
0,	REL,		0x48,		"andr",
0,	REL,		0x68,		"iorr",
0,	REL,		0x28,		"eorr",
0,	REL,		0xE8,		"comr",
0,	ABSO,		0x0C,		"loda",
0,	ABSO,		0xCC,		"stra",
0,	ABSO,		0x8C,		"adda",
0,	ABSO,		0xAC,		"suba",
0,	ABSO,		0x4C,		"anda",
0,	ABSO,		0x6C,		"iora",
0,	ABSO,		0x2C,		"eora",
0,	ABSO,		0xEC,		"coma",
0,	PSWM,		0x74,		"cpsu",
0,	PSWM,		0x75,		"cpsl",
0,	PSWM,		0x76,		"ppsu",
0,	PSWM,		0x77,		"ppsl",
0,	PSWM,		0xB4,		"tpsu",
0,	PSWM,		0xB5,		"tpsl",
0,	RET,		0x14,		"retc",
0,	RET,		0x34,		"rete",
0,	BRANCR,		0x18,		"bctr",
0,	BRANCRX,	0x98,		"bcfr",
0,	REL,		0x58,		"brnr",
0,	REL,		0xD8,		"birr",
0,	REL,		0xF8,		"bdrr",
0,	BRANCR,		0x38,		"bstr",
0,	BRANCRX,	0xB8,		"bsfr",
0,	REL,		0x78,		"bsnr",
0,	BRANCA,		0x1E,		"bcta",
0,	BRANCAX,	0x9E,		"bcfa",
0,	BRANRA,		0x5E,		"brna",
0,	BRANRA,		0xDE,		"bira",
0,	BRANRA,		0xFE,		"bdra",
0,	BRANCA,		0x3E,		"bsta",
0,	BRANCAX,	0xBE,		"bsfa",
0,	BRANRA,		0x7E,		"bsna",
0,	ZBRR,		0x9B,		"zbrr",
0,	ZBRR,		0xBB,		"zbsr",
0,	BXSA,		0x9F,		"bxa",
0,	BXSA,		0xBF,		"bsxa",
