/*
  Converts an operand to a ascii mask

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include "unmacro.h"
#include "getdata.h"
#include "debug.h"
masks(operand, line)
char operand;
char *line;
{
  char temp[8];
  int i, mask;

  debmsg("<masks>");
  mask = (operand & 0xff) + (getbyte() & 0xff)*256;
  if (mask) {
    strcpy(line, "^m<");
    for (i = 0; i < 12; i++)
      if ((1 << i) & mask) {
	sprintf(temp, "r%d,", i);
	strcat(line, temp);
      }
    if (mask & 4096)
      strcat(line, "ap,");
    if (mask & 8192)
      strcat(line, "fp,");
    if (mask & 16384)
      strcat(line, "iv,");
    if (mask & 32768)
      strcat(line, "dv,");
    line[strlen(line)-1] = '>';	/* remove last comma */
  } else
    strcpy(line, "^m<>");
}
