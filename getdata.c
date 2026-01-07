/*
  Copyright (c) 1988 by Robert Thomas Chojnacki, All rights reserved.
*/
/*
  GetByte returns the next byte from the file
*/
#include "unmacro.h"
#include "getdata.h"
#include "debug.h"
#include <stdio.h>
static int open=0;
char getbyte()
{
  debmsg("<getbyte>");
/* get a byte */
#ifdef	debug
  fprintf(stderr, " val, pc = %02X, %08X\n", image[pc-base] & 0xff, pc);
#endif
  if (pc <= top) {
    pc++;
    return(image[pc-base-1]);
  } else {
    fprintf(stderr, "past end of image\n");
    return(-1);
  }
}
/*
  read in a word
*/
short int getword()
{
  union {
    char b[4];
    int word;
  } b;

  debmsg("<getword>");
  b.word = 0;
  b.b[0] = getbyte();
  b.b[1] = getbyte();
  return(b.word);
}
/*
  read in a long word
*/
getlong()
{
  union {
    char b[4];
    int longword;
  } b;

  debmsg("<getlong>");
  b.b[0] = getbyte();
  b.b[1] = getbyte();
  b.b[2] = getbyte();
  b.b[3] = getbyte();
  return(b.longword);
}
