/*
  command definitions

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
/*
  main commands
*/
#define	C_DISASSEMBLE	0
#define	C_SET		1
#define	C_SHOW		2
#define	C_QUIT		3
#define	C_GOTO		4
#define	C_SAVE		5
#define	C_UNSAVE	6
#define	C_DUMP		7
#define	C_STATS		8
#define	C_SEARCH	9
#define	C_HEADER	10
#define	C_HELP		11
#define	C_WRITE		12
#define C_SYMBOL	13
#define C_EXIT		14
#define C_PUSH		15
#define C_POP		16
/*
  DISASSEMBLE and SET/SHOW BYTE/WORD/LONG/QUAD/OCTA commands
*/
#define C0_UNDEFINED	0
#define	C0_INSTRUCTION	1
#define	C0_BYTE		2
#define	C0_WORD		3
#define	C0_LONG		4
#define	C0_QUAD		5
#define	C0_OCTA		6
#define	C0_F_FLOATING	7
#define	C0_D_FLOATING	8
#define	C0_G_FLOATING	9
#define	C0_H_FLOATING	10
#define	C0_ASCII	11
#define	C0_ASCIC	12
#define	C0_ASCID	13
#define	C0_ASCIZ	14
#define	C0_BLKB		15
#define	C0_BLKW		16
#define	C0_BLKL		17
#define	C0_BLKQ		18
#define	C0_BLKO		19
#define	C0_ADDRESS	20
#define	C0_MASK		21
#define	C0_ENTRY	22
#define	C0_UNTIL	23
#define	C0_ALL		24
/*
  DISASSEMBLE UNTIL commands
*/
#define	C0x22_END	0
#define	C0x22_PC	1
#define	C0x22_DATA	2
/*
  SET SHOW commands
*/
#define	C1_BYTE		0
#define	C1_MODE		1
#define	C1_CASE		2
#define	C1_WORD		3
#define	C1_LONG		4
#define	C1_QUAD		5
#define	C1_OCTA		6
#define	C1_RANGE	7
#define	C1_LABEL	8
#define	C1_UNLABEL	9
#define	C1_STACK	10
/*
  SET CASE commands
*/
#define	C1x2_LOWER	0
#define	C1x2_UPPER	1
/*
  SET MODE commands
*/
#define	C1x1_BINARY	0
#define	C1x1_OCTAL	0
#define	C1x1_DECIMAL	0
#define	C1x1_HEXADECIMAL 0
/*
  SYMBOL commands
*/
#define C13_DEFINE	0
#define C13_LIST	1
#define C13_REMOVE	2
/*
  command strings
*/
#ifdef	commandsfile
char prompt[] = "%08X> ";
char *unmacrocommands[] = {
  "DISASSEMBLE",
  "SET",
  "SHOW",
  "QUIT",
  "GOTO",
  "SAVE",
  "UNSAVE",
  "DUMP",
  "STATS",
  "SEARCH",
  "HEADER",
  "HELP",
  "WRITE",
  "SYMBOL",
  "EXIT",
  "PUSH",
  "POP"};
int sizeof_unmacrocommands=sizeof(unmacrocommands)/4;
char *disassembl[] = {
  "UNDEFINED",
  "INSTRUCTION",
  "BYTE",
  "WORD",
  "LONG",
  "QUAD",
  "OCTA",
  "F_FLOATING",
  "D_FLOATING",
  "G_FLOATING",
  "H_FLOATING",
  "ASCII",
  "ASCIC",
  "ASCID",
  "ASCIZ",
  "BLKB",
  "BLKW",
  "BLKL",
  "BLKQ",
  "BLKO",
  "ADDRESS",
  "MASK",
  "ENTRY",
  "UNTIL",
  "ALL"};
char *disassembleuntil[] = {
  "END",
  "PC",		/* DISASSEMBLE UNTIL PC x */
  "DATA"};	/* Disassembles source until bytetype = data */
char *setshow[] = {
  "BYTE",
  "MODE",
  "CASE",
  "WORD",
  "LONG",
  "QUAD",
  "OCTA",
  "RANGE",
  "LABEL",
  "UNLABEL",
  "STACK"};
char *setshowbyte[] = {
  "UNDEFINED",
  "INSTRUCTION",
  "BYTE",
  "WORD",
  "LONG",
  "QUAD",
  "OCTA",
  "F_FLOATING",
  "D_FLOATING",
  "G_FLOATING",
  "H_FLOATING",
  "ASCII",
  "ASCIC",
  "ASCID",
  "ASCIZ",
  "BLKB",
  "BLKW",
  "BLKL",
  "BLKQ",
  "BLKO",
  "ADDRESS",
  "MASK",
  "ENTRY"};
char *setcase[] = {
  "LOWER",
  "UPPER"};
char *setmode[] = {
  "BINARY",
  "OCTAL",
  "DECIMAL",
  "HEXADECIMAL"};
char *symbol[] = {
  "DEFINE",
  "LIST",
  "REMOVE"};
char arg1[128], arg2[128], arg3[128], arg4[128];
int lastcommand = -1;
#else
extern char prompt[];
extern char *unmacrocommands[];
extern int sizeof_unmacrocommands;
extern char *disassembl[];
extern char *disassembleuntil[];
extern char *setshow[];
extern char *setshowbyte[];
extern char *setcase[];
extern char *setmode[];
extern char *symbol[];
extern char arg1[], arg2[], arg3[], arg4[];
extern int lastcommand;
#endif