/*
  UNMACRO common data

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#define	STACKDEPTH	32
#ifdef	mainfile
int limit;
int base=0;
int top=0;
int tot_blocks;
char *image;
char *bytetype;
int pc=0;
int opcode;
char in_file[64];
char out_file[] = "UNMACRO$OUT";
int lastliteral;
int stack[STACKDEPTH], stackp=STACKDEPTH;
#else
extern int limit;
extern int base;
extern int top;
extern int tot_blocks;
extern char *image;
extern char *bytetype;
extern int pc;
extern int opcode;
extern char in_file[64];
extern char *out_file;
extern int lastliteral;
extern int stack[], stackp;
#endif