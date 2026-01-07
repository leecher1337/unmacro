/*
  Converts an operand to a ascii displacement

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include "unmacro.h"
#include "commands.h"
#include "getdata.h"
#include "debug.h"
displacements(operand, size, line)
char operand;
int size;
char *line;
{
  union {
    char b[4];
    int word;
  } b;

  debmsg("<displacements>");
  if (size == 1) {
    sprintf(line, "L%X", operand + pc);
    if (operand+pc <= top) {
      bytetype[operand+pc-base] = -C0_INSTRUCTION;
    }
  } else {
    b.word = 0;
    b.b[0] = operand;
    b.b[1] = getbyte();
    sprintf(line, "L%X", b.word + pc);
    if (b.word+pc <= top) {
      bytetype[b.word+pc-base] = -C0_INSTRUCTION;
    }
  }
}
