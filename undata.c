/*
  disassemble a data line

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
char *malloc();
#include <string.h>
#include "unmacro.h"
#include "commands.h"
#include "posneg.h"
#include "getdata.h"
#include "debug.h"
undata()
{
  char arg_line[64];
  int aleng=0, count;

  debmsg("<undata>");
  switch (pos(bytetype[pc-base])) {
   case C0_BYTE:
    printf("\t.byte\t");
    printval(1,1);
    break;
   case C0_WORD:
    printf("\t.word\t");
    printval(2,1);
    break;
   case C0_LONG:
    printf("\t.long\t");
    printval(4,1);
    break;
   case C0_QUAD:
    printf("\t.quad\t");
    printval(8,1);
    break;
   case C0_OCTA:
    printf("\t.octa\t");
    printval(16,1);
    break;
   case C0_F_FLOATING:
    printf("\t.f_float\t%f\n", (float) getlong());
    break;
   case C0_D_FLOATING:
    break;
   case C0_G_FLOATING:
    break;
   case C0_H_FLOATING:
    break;
   case C0_ASCII:
    printf("\t.ascii\t/");
    while (pos(bytetype[pc-base]) == C0_ASCII) {
      if (aleng > 60) {
	aleng = 0;
	printf("/\n\t.ascii\t/");
      }
      aleng++;
      putchar(getbyte());
    }
    if (aleng)
      printf("/\n");
    break;
   case C0_ASCIC:
    printf("\t.ascic\t/");
    aleng = getbyte() & 0xff;
    for (count = 0; count < aleng; count++)
      putchar(getbyte());
    printf("/\n");
    break;
   case C0_ASCID:
    aleng = getlong() & 0xffff;
    pc = getlong();
    if ((pc >= base) && (pc <= top)) {
      printf("\t.ascid\t/");
      for (count = 0; count < aleng; count++)
	putchar(getbyte());
      printf("/\n");
    } else {
      printf(";Address in .ASCID descriptor is out of range.\n");
    }
    break;
   case C0_ASCIZ:
    printf("\t.ascii\t/");
    while (image[pc-base]) {
      if (aleng > 60) {
	aleng = 0;
	printf("/\n\t.ascii\t/");
      }
      aleng++;
      putchar(getbyte());
    }
    if (aleng)
      printf("/\n");
    getbyte();
    printf("\t.byte\t0\n");
    break;
   case C0_BLKB:
    printf("\t.blkb\t");
    aleng = 0;
    while ((pos(bytetype[pc-base]) == C0_BLKB) && !image[pc-base]) {
      pc++;
      aleng++;
    }
    if (pos(bytetype[pc-base]) == C0_BLKB) {
      bytetype[pc-base] == C0_BYTE;
    }
    printf("%d\n", aleng);
    break;
   case C0_BLKW:
    printf("\t.blkw\t");
    aleng = 0;
    while ((pos(bytetype[pc-base]) == C0_BLKW) && !image[pc-base]) {
      pc++;
      aleng++;
    } 
    if (pos(bytetype[pc-base]) == C0_BLKW) {
      bytetype[pc-base] == C0_BYTE;
    }
    printf("%d\n", aleng/2);
    break;
   case C0_BLKL:
    printf("\t.blkl\t");
    aleng = 0;
    while ((pos(bytetype[pc-base]) == C0_BLKL) && !image[pc-base]) {
      pc++;
      aleng++;
    } 
    if (pos(bytetype[pc-base]) == C0_BLKL) {
      bytetype[pc-base] == C0_BYTE;
    }
    printf("%d\n", aleng/4);
    break;
   case C0_BLKQ:
    printf("\t.blkq\t");
    aleng = 0;
    while ((pos(bytetype[pc-base]) == C0_BLKQ) && !image[pc-base]) {
      pc++;
      aleng++;
    } 
    if (pos(bytetype[pc-base]) == C0_BLKQ) {
      bytetype[pc-base] == C0_BYTE;
    }
    printf("%d\n", aleng/8);
    break;
   case C0_BLKO:
    printf("\t.blko\t");
    aleng = 0;
    while ((pos(bytetype[pc-base]) == C0_BLKO) && !image[pc-base]) {
      pc++;
      aleng++;
    } 
    if (pos(bytetype[pc-base]) == C0_BLKO) {
      bytetype[pc-base] == C0_BYTE;
    }
    printf("%d\n", aleng/16);
    break;
   case C0_ADDRESS:
    printf("\t.address\tx^%08X\n", getlong());
    break;
   case C0_MASK:
    masks(getbyte(), arg_line);
    printf("\t.mask\t%s\n", arg_line);
    break;
   case C0_ENTRY:
    masks(getbyte(), arg_line);
    printf("\t.entry\tL%X,%s\n", pc-2, arg_line);
    break;
  }
}
force_undata(btype)
char btype;
{
  char arg_line[64];
  int aleng=0, count;

  debmsg("<force_undata>");
  switch (btype) {
   case C0_BYTE:
    printf("\t.byte\t");
    printval(1,1);
    break;
   case C0_WORD:
    printf("\t.word\t");
    printval(2,1);
    break;
   case C0_LONG:
    printf("\t.long\t");
    printval(4,1);
    break;
   case C0_QUAD:
    printf("\t.quad\t");
    printval(8,1);
    break;
   case C0_OCTA:
    printf("\t.octa\t");
    printval(16,1);
    break;
   case C0_F_FLOATING:
    printf("\t.f_float\t%f\n", (float) getlong());
    break;
   case C0_D_FLOATING:
    break;
   case C0_G_FLOATING:
    break;
   case C0_H_FLOATING:
    break;
   case C0_ASCII:
    printf("\t.ascii\t/");
    while (aleng < 16) {
      aleng++;
      putchar(getbyte());
    }
    printf("/\n");
    break;
   case C0_ASCIC:
    printf("\t.ascic\t/");
    aleng = getbyte() & 0xff;
    for (count = 0; count < aleng; count++)
      putchar(getbyte());
    printf("/\n");
    break;
   case C0_ASCID:
    aleng = getlong() & 0xffff;
    pc = getlong();
    if ((pc >= base) && (pc <= top)) {
      printf("\t.ascid\t/");
      for (count = 0; count < aleng; count++)
	putchar(getbyte());
      printf("/\n");
    } else {
      printf(";Address in .ASCID descriptor is out of range.\n");
    }
    break;
   case C0_ASCIZ:
    printf("\t.ascii\t/");
    while (image[pc-base]) {
      if (aleng > 60) {
	aleng = 0;
	printf("/\n\t.ascii\t/");
      }
      aleng++;
      putchar(getbyte());
    }
    if (aleng)
      printf("/\n");
    getbyte();
    printf("\t.byte\t0\n");
    break;
   case C0_BLKB:
    printf("\t.blkb\t");
    aleng = 0;
    while (!getbyte()) {
      aleng++;
    } 
    --pc;
    printf("%d\n", aleng);
    break;
   case C0_BLKW:
    printf("\t.blkw\t");
    aleng = 0;
    while (!getbyte()) {
      aleng++;
    } 
    --pc;
    printf("%d\n", aleng/2);
    break;
   case C0_BLKL:
    printf("\t.blkl\t");
    aleng = 0;
    while (!getbyte()) {
      aleng++;
    } 
    --pc;
    printf("%d\n", aleng/4);
    break;
   case C0_BLKQ:
    printf("\t.blkq\t");
    aleng = 0;
    while (!getbyte()) {
      aleng++;
    } 
    --pc;
    printf("%d\n", aleng/8);
    break;
   case C0_BLKO:
    printf("\t.blko\t");
    aleng = 0;
    while (!getbyte()) {
      aleng++;
    } 
    --pc;
    printf("%d\n", aleng/16);
    break;
   case C0_ADDRESS:
    printf("\t.address\tx^%08X\n", getlong());
    break;
   case C0_MASK:
    masks(getbyte(), arg_line);
    printf("\t.mask\t%s\n", arg_line);
    break;
   case C0_ENTRY:
    masks(getbyte(), arg_line);
    printf("\t.entry\tL%X,%s\n", pc-2, arg_line);
    break;
  }
}
/*
  Display a value in the current mode
*/
printval(size, eoln)
int size, eoln;
{         /* do everything in hex for now */
  int l2, l1, l0;

  switch (size) {
   case 1:
    printf("^x%02X", getbyte() & 0xff);
    break;
   case 2:
    printf("^x%04X", getword() & 0xffff);
    break;
   case 4:
    printf("^x%08X", getlong());
    break;
   case 8:
    l0 = getlong();
    printf("^x%08X", getlong());
    printf("%08X", l0);
    break;
   case 16:
    l0 = getlong();
    l1 = getlong();
    l2 = getlong();
    printf("^x%08X", getlong());
    printf("%08X", l2);
    printf("%08X", l1);
    printf("%08X", l0);
    break;
  }
  if (eoln)
    printf("\n");
}
