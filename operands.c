/*
  Convert a binary operand to ascii

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include "unmacro.h"
#include "commands.h"
#include "posneg.h"
#include "getdata.h"
#include "debug.h"
operands(boperand, literal_type, line)
char boperand;
int literal_type;
char *line;
{
  char sreg[4];
  int operand, mode, reg, byte, address;
/*
  comment
*/
  debmsg("<operands>");
  operand = boperand & 0xff;	/* clear top bytes */
  mode = (operand >> 4) & 0xf;	/* get mode */
  reg = operand & 0xf;		/* get register */
#ifdef	debug
  fprintf(stderr, "op, mode, reg = %2X, %X, %X", operand, mode, reg);
#endif
  if (reg < 12) {
    sprintf(sreg, "r%d", reg);
  } else if (reg == 12) {
    strcpy(sreg, "ap");
  } else if (reg == 13) {
    strcpy(sreg, "fp");
  } else if (reg == 14) {
    strcpy(sreg, "sp");
  } else if (reg == 15) {
    strcpy(sreg, "pc");
  }
#ifdef	debug
  fprintf(stderr, " (%s)\n", sreg);
#endif
/*
  extract operand based on its mode
*/
  switch (mode) {
   case 0:				/* literals */
   case 1:
   case 2:
   case 3:
    sprintf(line, "s^#^x%X", operand);
    lastliteral = boperand;
    break;
   case 4:				/* indexed mode */
    if (reg < 15) {
      sprintf(line, "[%s]", sreg);
    } else {
      return(-mode);		/* cant use this mode with PC */
    }
    break;
   case 5:				/* register mode */
    if (reg < 15) {
      sprintf(line, "%s", sreg);
    } else {
      return(-mode);		/* cant use this mode with PC */
    }
    break;
   case 6:				/* register deferred */
    if (reg < 15) {
      sprintf(line, "(%s)", sreg);
    } else {
      return(-mode);		/* cant use this mode with PC */
    }
    break;
   case 7:				/* autodecrement */
    if (reg < 15) {
      sprintf(line, "-(%s)", sreg);
    } else {
      return(-mode);		/* cant use this mode with PC */
    }
    break;
   case 8:				/* autoincrement */
    if (reg < 15) {
      sprintf(line, "(%s)+", sreg);
    } else {
      if (literal_type == C0_BYTE) {
        lastliteral = getbyte() & 0xff;
        sprintf(line, "i^#^x%02X", lastliteral);
      } else if (literal_type == C0_WORD) {
        lastliteral = getword() & 0xffff;
        sprintf(line, "i^#^x%04X", lastliteral);
      } else {
        lastliteral = getlong();
        sprintf(line, "i^#^x%08X", lastliteral);
      }
      sprintf(line, "i^#^x%08X", lastliteral);
    }
    break;
   case 9:				/* autoincrement deferred */
    if (reg < 15) {
      sprintf(line, "@(%s)+", sreg);
    } else {
      address = getlong();
      sprintf(line, "@#^x%08X", address);
      if ((address >= base) && (address <= top)) {
	bytetype[address-base] = neg(bytetype[address-base]);
      }
    }
    break;
   case 10:
    if (reg < 15) {
      sprintf(line, "b^^x0%X(%s)", getbyte() & 0xff, sreg);
    } else {
      sprintf(line, "b^^x0%X", getbyte() + pc);
    }
    break;
   case 11:
    if (reg < 15) {
      sprintf(line, "@b^^x0%X(%s)", getbyte() & 0xff, sreg);
    } else {
      sprintf(line, "b^^x%0X", getbyte() + pc);
    }
    break;
   case 12:
    if (reg < 15) {
      sprintf(line, "w^^x0%X(%s)", getword() & 0xffff, sreg);
    } else {
      sprintf(line, "w^^x0%X", getword() + pc);
    }
    break;
   case 13:
    if (reg < 15) {
      sprintf(line, "@w^^x0%X(%s)", getword() & 0xffff, sreg);
    } else {
      sprintf(line, "@w^^x0%X", getword() + pc);
    }
    break;
   case 14:
    if (reg < 15) {
      sprintf(line, "l^^x%X(%s)", getlong(), sreg);
    } else {
      address = getlong() + pc;
      sprintf(line, "l^^x%X", address);
      if ((address >= base) && (address <= top)) {
	bytetype[address-base] = neg(bytetype[address-base]);
      }
    }
    break;
   case 15:
    if (reg < 15) {
      sprintf(line, "@l^^x%X(%s)", getlong(), sreg);
    } else {
      address = getlong() + pc;
      sprintf(line, "@l^^x%X", address);
      if ((address >= base) && (address <= top)) {
	bytetype[address-base] = -C0_LONG;
      }
    }
    break;
  }
  debmsg(">operands<");
  return(mode);
}
