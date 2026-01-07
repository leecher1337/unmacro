/*
  disassemble a line

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include <string.h>
#include "unmacro.h"
#include "commands.h"
#include "getdata.h"
#include "posneg.h"
#include "debug.h"
disassemble()
{
  char line[16], altline[16], arg_line[16];
  int i, args, alt, mode, literal_type=0;
  int opcode, operand, op_ref, op_type, pre_pc, tmp_pc;
  int address;
  short int casedisp;
  debmsg("<disassemble>");
/*
  disassemble instruction op-code
*/
  pre_pc = pc;
  opcode = getbyte();
  op_type = i_type(opcode, &op_ref);
  args = instructions(opcode, line, &alt, altline);
  operand = getbyte() & 0xff;
  printf("\t");
  if ((alt == 1) && (operand == 0xfd)) {
    printf("%s", altline);
  } else if ((alt == 3) && (operand == 0xff)) {
    printf("%s", altline);
  } else {
    printf("%s", line);
  }
/*
  disassemble instruction operands
*/
  if (args > 0) {
    mode = -1;
    printf("\t");
    for (i = 0; i < args; i++) {
      if ((i+1 == op_ref) && (op_type == 1)) {
	displacements(operand, 1, arg_line);
      } else if ((i+1 == op_ref) && (op_type == 2)) {
	masks(operand, arg_line);
      } else if ((i+1 == op_ref) && (op_type == 4)) {
	displacements(operand, 2, arg_line);
      } else if ((i == 1) && (op_type == 5)) {
        mode = operands(operand, literal_type, arg_line);
	if (mode == 14) {
	  pc -= 4;
	  address = getlong() + pc;
	  if ((address >= base) && (address <= top))
	    bytetype[address-base] = C0_ENTRY;
	} else if (mode == 9) {
	  pc -= 4;
	  address = getlong();
	  if (sym_findname(address))
	    sprintf(arg_line, "G^@#%s", sym_findname(address));
	}
      } else {
        mode = operands(operand, literal_type, arg_line);
	if (mode == 4)
	  args++;
      }
      printf("%s", arg_line);
      if (i < args-1) {
        if (mode != 4)
    	  printf(",");
      operand = getbyte() & 0xff;
      }
    }
    printf("\t\t;%X\n", pre_pc);
/*
  handle CASE instructions
*/
    if (op_type == 3) {
      pre_pc = pc;
      limit = lastliteral;
      printf("L%X:", pre_pc);
      for (i = 0; i <= limit; i++) {
	casedisp = getword();
	printf("\t.word\tL%X - L%X\n", casedisp + pre_pc, pre_pc);
	tmp_pc = casedisp + pre_pc - base;
        if ((tmp_pc >= 0) && (tmp_pc <= top-base))
	  bytetype[tmp_pc] = -C0_INSTRUCTION;
      }
    }
  } else if (args == 0) {
    --pc;
    printf("\t\t;%X\n", pre_pc);
/*
  Reserved instruction handling
*/
  } else {
    --pc;
    printf(" operand at %X\n", pre_pc);
  }
}
