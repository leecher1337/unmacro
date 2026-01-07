/*
  UNMACRO - disassemble a VAX image file.  This is biased twords VMS images
  although it could be used to disassemble other images, etcetera.

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
static char
 note[]="Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.";

static char helpbuf[]="HELP/LIBRARY=UNMACRO_DIR:UNMACRO UNMACRO";
static struct {
  int length;
  char *address;
} help;

#define	mainfile
#include <stdio.h>
char *malloc();
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include "unmacro.h"
#include "commands.h"
#include "posneg.h"
#include "symtab.h"
#include "header.h"
#include "debug.h"
#ifndef SEEK_END
#define SEEK_END	2
#define SEEK_SET	0
#endif
FILE *sfp;

int ctlc(), ctlc_int;

main(argc, argv)
int argc;
char *argv[];
{
  char sstring[8], in_file_exe[256], helptmp[256];
  char block[512], arg_line[64], savefile[48];
  char tmpsym[SYMNAMALLO], tmpline[132];
  int command[8];
  int i, j, nblocks, pre_pc;
  int ssize;
  int current_arg;
/*
  init stuff
*/
  signal(SIGINT, ctlc);
  arg1[0] = 0; arg2[0] = 0; arg3[0] = 0; arg4[0] = 0;
  help.address = &helptmp[0];
  current_arg = 1;
  image_header = 'y';
/*
  Check command line for options

    -h means no header
*/
#ifdef debug
  for (i=0; i<argc; i++)
    printf("argv[%d] = '%s'\n", i, argv[i]);
#endif
  if (argc > 1) {
    if (argv[current_arg][0] == '-') {
      i = 1;
      while (argv[current_arg][i]) {
	if (argv[current_arg][i] == 'h') {
	  image_header = 'n';
	} else if (argv[current_arg][i] == '?') {
	  shorthelp();
	  exit(0);
	}
	i++;
      }
      current_arg++; --argc;
    }
  }
/*
  Open image file
*/
  if (argc == 2) {
    strcpy(in_file, argv[current_arg]);
  } else if (argc == 1) {
    fprintf(stderr, "Image file: ");
    scanf("%s", in_file);
    do {} while (getchar() != '\n');
  } else {
    usage();
  }
  j = strlen(in_file);
/*  for (i=0; i<j; i++)
    in_file[i] = toupper(in_file[i]); */
  if ((sfp = fopen(in_file, "rb")) != (FILE *) NULL) {
  } else {
    strcpy(in_file_exe, in_file); strcat(in_file_exe, ".EXE");
    if ((sfp = fopen(in_file_exe, "rb")) == (FILE *) NULL) {
      fprintf(stderr, "error opening %s for binary read\n",
        in_file);
      perror("UNMACRO reading image file");
      exit(1);
    }
  }
/*
  find the end of file
*/
  if (fseek(sfp, 0l, SEEK_END)) {
    fprintf(stderr, "Error seeking to the end of %s\n", in_file);
    perror("UNMACRO");
    fclose(sfp);
    exit(1);
  }
  tot_blocks = (ftell(sfp)+511)/512;
  if (fseek(sfp, 0l, SEEK_SET)) {
    fprintf(stderr, "error seeking to the beginning of %s\n", in_file);
    perror("UNMACRO");
    fclose(sfp);
    exit(1);
  }
  if (tot_blocks)
    fprintf(stderr, "%d blocks found in %s.\n", tot_blocks, in_file);
  else {
    fprintf(stderr, "Zero length file.\n");
    fclose(sfp);
    exit(1);
  }
  clearerr(sfp);
/*
  Get image header
*/
  header();
  fprintf(stderr, "Memory size of image is %X pages (%d.)\n",
   mem_pages, mem_pages);
/*
  Allocate image and bytetype buffers
*/
  if (!(image = malloc(mem_pages*512))) {
    fprintf(stderr, "insufficient memory for image\n");
    exit(1);
  }
  if (!(bytetype = malloc(mem_pages*512))) {
    fprintf(stderr, "insufficient memory for image mask\n");
    exit(1);
  }
/*
  read in file
*/
  if (image_header == 'y') {
    base = lowest_vpn*512;
    fprintf(stderr,"isd ");
    for (i=0; i<h_nisd; i++) {
      fprintf(stderr,"%d ",i);
      if (!(h_isd[i]->flags & ISD_GLOBAL) && !(h_isd[i]->flags & 0xff000000)) {
        if (h_isd[i]->flags & ISD_DZERO) {
          memset(&image[top], 0, h_isd[i]->pages*512);
          memset(&bytetype[top], C0_BLKB, h_isd[i]->pages*512);
        } else {
          if (feof(sfp))
            fprintf(stderr, "end of image file detected\n");
          fread(&image[top], 512, h_isd[i]->pages, sfp);
/* if this section is writeable then assume data */
	  if (h_isd[i]->flags & ISD_WRITE)
	    memset(&bytetype[top], C0_BYTE, h_isd[i]->pages*512);
	  else
	    memset(&bytetype[top], C0_INSTRUCTION, h_isd[i]->pages*512);
        }
        top += 512*h_isd[i]->pages;
      }
    }
    fprintf(stderr, "\n");
    top += base-1;
  } else {
/* "no-header" stuff */
    fread(image, 512, tot_blocks, sfp);
    memset(bytetype, C0_INSTRUCTION, tot_blocks*512);
    top = mem_pages*512 - 1;
  }
  fprintf(stderr, "base, top = %08X, %08X\n", base, top);
  fclose(sfp);
/*
  put .ENTRY's on transfer addresses in memory
*/
  if ((h_transfer1 >= base) && (h_transfer1 <= top))
    bytetype[h_transfer1-base] = C0_ENTRY;
  if ((h_transfer2 >= base) && (h_transfer2 <= top))
    bytetype[h_transfer2-base] = C0_ENTRY;
  if ((h_transfer3 >= base) && (h_transfer3 <= top))
    bytetype[h_transfer3-base] = C0_ENTRY;
/*
  display disassembled source
*/
  pc = base;
  do {
    ctlc_int = 0;
/*
  get command
*/
    if (commands(command)) {
      comderr("UNMACRO>", 0, unmacrocommands, sizeof_unmacrocommands);
    } else {
      switch (command[0]) {
/*
  DISASSEMBLE
*/
       case C_DISASSEMBLE:
	switch (command[1]) {
	 case C0_UNTIL:
	  switch (command[2]) {
	   case C0x22_END:
	    while ((pc <= top) && !ctlc_int) {
	      if (bytetype[pc-base] < 0)
		printf("L%X:", pc);
	      if (pos(bytetype[pc-base]) == C0_INSTRUCTION) {
		disassemble();
	      } else {
		undata();
	      }
	    }
	    printf("\n");
	    break;
	   case C0x22_PC:
	    fprintf(stderr, "pc, command[3] = %08X, %08X\n", pc, command[3]);
	    while ((pc <= command[3]) && (pc <= top) && !ctlc_int) {
	      if (bytetype[pc-base] < 0)
		printf("L%X:", pc);
	      if (pos(bytetype[pc-base]) == C0_INSTRUCTION) {
		disassemble();
	      } else {
		undata();
	      }
	    }
	    printf("\n");
	    break;
	   case C0x22_DATA:
	    while ((pc <= top) && (pos(bytetype[pc-base]) ==
	     C0_INSTRUCTION) && !ctlc_int) {
	      if (bytetype[pc-base] < 0)
		printf("L%X:", pc);
	      disassemble();
	    }
	    printf("\n");
	    break;
	  }
	  break;
	 case C0_ALL:
	  pc = base;
	  while ((pc <= top) && !ctlc_int) {
	    if (bytetype[pc-base] < 0)
	      printf("L%X:", pc);
	    if (pos(bytetype[pc-base]) == C0_INSTRUCTION) {
	      disassemble();
	    } else {
	      undata();
	    }
	  }
	  break;
	 case C0_INSTRUCTION:
	  if (bytetype[pc-base] < 0)
	    printf("L%X:", pc);
	  disassemble();
	  printf("\n");
	  break;
	 default:
	  if ((command[1] >= C0_BYTE) && (command[1] < C0_UNTIL)) {
	    pre_pc = pc;
	    force_undata(command[1]);
	    pc = pre_pc;
	  } else if (pos(bytetype[pc-base]) == C0_INSTRUCTION) {
	    if (bytetype[pc-base] < 0)
	      printf("L%X:", pc);
	    disassemble();
	  } else {
	    if (bytetype[pc-base] < 0)
	      printf("L%X:", pc);
	    undata();
	  }
	  printf("\n");
	}
	break;
/*
  SET
*/
       case C_SET:
	switch(command[1]) {
	 case C1_BYTE:
	  bytetype[pc-base] = command[2];
	  break;
	 case C1_WORD:
	  bytetype[pc-base] = command[2];
	  bytetype[pc-base+1] = command[2];
	  break;
	 case C1_LONG:
	  bytetype[pc-base] = command[2];
	  bytetype[pc-base+1] = command[2];
	  bytetype[pc-base+2] = command[2];
	  bytetype[pc-base+3] = command[2];
	  break;
	 case C1_QUAD:
	  bytetype[pc-base] = command[2];
	  bytetype[pc-base+1] = command[2];
	  bytetype[pc-base+2] = command[2];
	  bytetype[pc-base+3] = command[2];
	  bytetype[pc-base+4] = command[2];
	  bytetype[pc-base+5] = command[2];
	  bytetype[pc-base+6] = command[2];
	  bytetype[pc-base+7] = command[2];
	  break;
	 case C1_OCTA:
	  bytetype[pc-base] = command[2];
	  bytetype[pc-base+1] = command[2];
	  bytetype[pc-base+2] = command[2];
	  bytetype[pc-base+3] = command[2];
	  bytetype[pc-base+4] = command[2];
	  bytetype[pc-base+5] = command[2];
	  bytetype[pc-base+6] = command[2];
	  bytetype[pc-base+7] = command[2];
	  bytetype[pc-base+8] = command[2];
	  bytetype[pc-base+9] = command[2];
	  bytetype[pc-base+10] = command[2];
	  bytetype[pc-base+11] = command[2];
	  bytetype[pc-base+12] = command[2];
	  bytetype[pc-base+13] = command[2];
	  bytetype[pc-base+14] = command[2];
	  bytetype[pc-base+15] = command[2];
	  break;
	 case C1_RANGE:
	  if (pc-base+command[3] > top)
	    fprintf(stderr, "?Top of program is %08X - command invalid\n",
	     top);
	  else
	    for (i=0; i<command[3]; i++)
	      bytetype[pc-base+i] = command[2];
	  break;
	 case C1_LABEL:
	  bytetype[pc-base] = neg(bytetype[pc-base]);
	  break;
	 case C1_UNLABEL:
	  bytetype[pc-base] = pos(bytetype[pc-base]);
	  break;
	}
	break;
/*
  SHOW
*/
       case C_SHOW:
	switch(command[1]) {
	 case C1_BYTE:
	 case C1_WORD:
	 case C1_LONG:
	 case C1_QUAD:
	 case C1_OCTA:
	  fprintf(stderr, "%s", setshowbyte[pos(bytetype[pc-base])]);
	  if (bytetype[pc-base] < 0) {
	    fprintf(stderr, " (labeled)\n");
	  } else {
	    fprintf(stderr, "\n");
	  }
	  break;
	 case C1_STACK:
	  if (stackp < STACKDEPTH) {
	    fprintf(stderr, "(sp) -> %08X\n", stack[stackp]);
	    for (i = stackp+1; i < STACKDEPTH; i++)
	      fprintf(stderr, "        %08X\n", stack[i]);
	  } else
	    fprintf(stderr, "The stack is empty\n");
	  break;
	}
	printf("\n");
	break;
/*
  GOTO
*/
       case C_GOTO:
	if (command[1] < base) {
	  fprintf(stderr, "Base of program is %08X\n", base);
	} else if (command[1] > top) {
	  fprintf(stderr, "Top of program is %08X\n", top);
	} else {
	  pc = command[1];
	}
	break;
/*
  SAVE
*/
       case C_SAVE:
	if (strlen(arg2)) {
	  strcpy(savefile, arg2);
	} else {
	  fprintf(stderr, "File to save image mask to: ");
	  scanf("%s", savefile);
	  do {} while (getchar() != '\n');
	}
	if ((sfp = fopen(savefile, "wb")) == (FILE *) NULL) {
	  fprintf(stderr, "error opening %s for binary write\n",
	   savefile);
	  perror("SAVE");
	} else {
	  fwrite(bytetype, 512, mem_pages, sfp);
	  fclose(sfp);
	}
	break;
/*
  UNSAVE
*/
       case C_UNSAVE:
	if (strlen(arg2)) {
	  strcpy(savefile, arg2);
	} else {
	  fprintf(stderr, "File to read image mask from: ");
	  scanf("%s", savefile);
	  do {} while (getchar() != '\n');
	}
	if ((sfp = fopen(savefile, "rb")) == (FILE *) NULL) {
	  fprintf(stderr, "error opening %s for binary read\n",
	   savefile);
	  perror("UNSAVE");
	} else {
	  fread(bytetype, 512, mem_pages, sfp);
	  fclose(sfp);
	}
	break;
/*
  DUMP
*/
       case C_DUMP:
	dump(image, pc-base, top-base, pc);
	break;
/*
  PUSH
*/
       case C_PUSH:
	if (stackp)
	  stack[--stackp] = pc;
	else
	  fprintf(stderr, "not enough room on stack\n");
	break;
/*
  POP
*/
       case C_POP:
	if (stackp < STACKDEPTH)
	  pc = stack[stackp++];
	else
	  fprintf(stderr, "stack empty\n");
	break;
/*
  STATS
*/
       case C_STATS:
	fprintf(stderr, "Stats for image %s\n", in_file);
        disph(stderr);
	break;
/*
  SEARCH
*/
       case C_SEARCH:
	fprintf(stderr, "size of string (bytes): ");
	scanf("%d", &ssize);
	do {} while (getchar() != '\n');
	if (ssize <= 0) {
	  break;
	} else if (ssize >= sizeof(sstring)) {
	  fprintf(stderr, "string size must be less than %d bytes\n",
	   sizeof(sstring));
	  break;
	}
	for (i = 0; i < ssize; i++) {
	  fprintf(stderr, "byte %d (decimal): ", i);
	  scanf("%d", &sstring[i]);
	  do {} while (getchar() != '\n');
	}
	for (; pc <= top; pc++) {
	  for (i = 0; i < ssize; i++) {
	    if (sstring[i] != image[pc-base+i])
	      break;
	  }
	  if (i == ssize)
	    break;
	}
	if (i == ssize) {
	  fprintf(stderr, "string found at pc = %08X\n", pc);
	}
	break;
/*
  HEADER dump
*/
       case C_HEADER:
	if (h_blocks) {
	  for (i = 0; i < h_blocks; i++) {
	    fprintf(stderr, "\nheader block %d\n\n", i);
	    dump(hb[i], 0, 512, 512*i);
	    dump(hb[i], 256, 512, 512*i+256);
	  }
	} else {
	  fprintf(stderr, "\nThere is no image header.\n");
	}
	break;
/*
  HELP
*/
       case C_HELP:
	strcpy(helptmp, helpbuf);
	if (strlen(arg2)) {
	  strcat(helptmp, " ");
	  strcat(helptmp, arg2);
	}
	if (strlen(arg3)) {
	  strcat(helptmp, " ");
	  strcat(helptmp, arg3);
	}
	if (strlen(arg4)) {
	  strcat(helptmp, " ");
	  strcat(helptmp, arg4);
	}
	help.length = strlen(helptmp);
#ifdef VMS
	Lib$Spawn(&help);
#else
	fprintf(stderr, "Sorry, this was not copiled under VMS.\n");
#endif
	break;
/*
  WRITE results to a file
*/
       case C_WRITE:
	if (strlen(arg2)) {
	  strcpy(out_file, arg2);
	} else {
	  fprintf(stderr, "File to save disassembled code to: ");
	  scanf("%s", out_file);
	  do {} while (getchar() != '\n');
	}
	if (freopen(out_file, "w", stdout) == (FILE *) NULL) {
	  fprintf(stderr, "error reopening '%s' for write\n", out_file);
	  perror("WRITE");
	  exit(1);
	}
	pre_pc = pc;
	pc = base;
	printf(";++\n");
	printf(";Stats for image %s\n", in_file);
	disph(stdout);
	printf(";--\n;++\n");
	if (h_blocks) {
	  for (i = 0; i < h_blocks; i++) {
	    printf(";header block %d\n;\n", i);
	    fdump(hb[i], 0, 512, 512*i);
	    fdump(hb[i], 256, 512, 512*i+256);
	  }
	} else {
	  printf(";There is no image header.\n");
	}
	printf(";--\n");
	while ((pc <= top) && !ctlc_int) {
	  if (bytetype[pc-base] < 0)
	    printf("L%X:", pc);
	  if (pos(bytetype[pc-base]) == C0_INSTRUCTION) {
	    disassemble();
	  } else {
	    undata();
	  }
	}
	printf("\t.end L%X\n", h_transfer1);
	pc = pre_pc;
	if (freopen("SYS$OUTPUT", "w", stdout) == (FILE *) NULL) {
	  fprintf(stderr, "error reopening SYS$OUTPUT for write\n");
	  perror("WRITE");
	  exit(1);
	}
	break;
/*
  SYMBOL
*/
       case C_SYMBOL:
	switch(command[1]) {
	 case C13_DEFINE:
          fprintf(stderr, "Symbol name (max %d chars): ", SYMNAMALLO);
	  sprintf(tmpline, "%%%ds", SYMNAMALLO);
          scanf(tmpline, tmpsym); do {} while (getchar() != '\n');
          fprintf(stderr, "Symbol value: ");
          scanf("%08X", &i); do {} while (getchar() != '\n');
	  sym_add(tmpsym, i);
          break;
	 case C13_LIST:
	  fprintf(stderr, "%d symbols defined\n", sym_count);
          if (sym_count) {
            fprintf(stderr, "Symbol table:\n");
            for (i=0; i<sym_count; i++)
              fprintf(stderr, "%08X = '%s'\n", sym_value[i], sym_name[i]);
          }
          break;
	 case C13_REMOVE:
	  fprintf(stderr, "SYMBOL REMOVE not working yet.  Sorry.\n");
          break;
        }
	break;
      }
    }
  } while ((command[0] != C_QUIT) && (command[0] != C_EXIT));
  exit(1);
}
/*
  get a long word
*/
lw(b)
int *b;
{
  return(*b);
}
/*
  Dump 256 bytes of data in a VMS DUMP format.

	"address" is the address of the buffer
	"start" is the starting index
	"limit" is the maximum index (the upper limit)
	"virtual" is the base of the address to be displayed at the end
	  of each dump line
*/
dump(address, start, limit, virtual)
char *address;
int start;
int limit;
int virtual;
{
  char c;
  int i, j, k;

  for (i=0; i<256; i+=16) {
    if (start+i+15 > limit) {
      k = limit - (start+i);
      if (k) {
        for (j=k; j>=0; --j)
          fprintf(stderr, "%02X ", address[start+i+j] & 0xff);
        for (j=15-k; j>0; --j)
          fprintf(stderr, "?? ");
        fprintf(stderr, " ");
        for (j=0; j<=k; j++) {
          c = address[start+i+j] & 0x7f;
          if ((c > 0x1f) && (c < 0x7f))
            fprintf(stderr, "%c", c);
          else
            fprintf(stderr, ".");
        }
        for (j=15-k; j>0; --j)
          fprintf(stderr, "?");
        fprintf(stderr, " %08X\n", virtual+i);
      }
      break;
    }
    for (j=15; j>=0; --j)
      fprintf(stderr, "%02X ", address[start+i+j] & 0xff);
    fprintf(stderr, " ");
    for (j=0; j<16; j++) {
      c = address[start+i+j] & 0x7f;
      if ((c > 0x1f) && (c < 0x7f))
        fprintf(stderr, "%c", c);
      else
        fprintf(stderr, ".");
    }
    fprintf(stderr, " %08X\n", virtual+i);
  }
}
/*
  Dump a block to the disassembly listing file
*/
fdump(address, start, limit, virtual)
char *address;
int start;
int limit;
int virtual;
{
  int i, j;

  for (i=0; i<256; i+=16) {
    if (start+16 > limit)
      break;
    printf(";");
    for (j=15; j>=0; --j)
      printf("%02X ", address[start+i+j] & 0xff);
    printf(" ");
    for (j=0; j<16; j++)
      if (!iscntrl(address[start+i+j] & 0x7f))
        printf("%c", address[start+i+j] & 0x7f);
      else
        printf(".");
    printf(" %08X\n", virtual+i);
  }
}

/*
  "usage" error message
*/
usage()
{
  fprintf(stderr, "usage: unmacro [-h] imagefile[.exe]\n");
  fprintf(stderr, "       unmacro -? for short help\n");
  exit(1);
}

/*
  control-c interrupt handler
*/
ctlc()
{
  ctlc_int = 1;
  signal(SIGINT, ctlc);
}

/*
  Short help
*/
shorthelp()
{
  fprintf(stderr, "unmacro [-options] image-file[.exe]\n");
  fprintf(stderr, "unmacro -- disassemble VMS executeable files\n");
  fprintf(stderr, "options:\n");
  fprintf(stderr, "        h - ignore image header\n");
  fprintf(stderr, "        ? - display this short message\n");
}
