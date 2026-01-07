/*
  interpret commands -- the main purpose of commands.c is to parse the command
line for the command and any parameters needed by that command.  The actual
command action is done in unmacro.c.

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#define	commandsfile
#include <stdio.h>
#include <string.h>
#include "unmacro.h"
#include "commands.h"
#include "debug.h"
commands(command)
int command[8];
{
  char cmdline[128];
  int i, cmd, cmd2, cmd3, cmd4;

  debmsg("<commands>");
  command[1] = -1;
  command[2] = -1;
  command[3] = -1;
  arg2[0] = 0;
  arg3[0] = 0;
  arg4[0] = 0;
  cmdline[0] = 0;
/*
  read in command
*/
  fprintf(stderr, prompt, pc);
  if (scanf("%128[^\n]s", cmdline) == EOF)
    exit(1);
  do {} while (getchar() != '\n');
#ifdef debug
  fprintf(stderr, "cmdline = '%s'\n", cmdline);
  fprintf(stderr, "lastcommand (in) = %d\n", lastcommand);
#endif
  for (i = 0; i < strlen(cmdline); i++)
    cmdline[i] = toupper(cmdline[i]);
/*
  parse line
*/
  if (!strlen(cmdline)) {
    if ((lastcommand != C_DISASSEMBLE) && (lastcommand != C_DUMP)) {
      command[0] = C_DISASSEMBLE;
      lastcommand = C_DISASSEMBLE;
    }
    if (lastcommand == C_DUMP)
      if (pc + 0x100 <= top)
	pc += 0x100;
    return(0);
  }
  sscanf(cmdline, "%128s", arg1);
  sscanf(cmdline, "%*s%128s", arg2);
  sscanf(cmdline, "%*s%*s%128s", arg3);
  sscanf(cmdline, "%*s%*s%*s%128s", arg4);
#ifdef	debug
  fprintf(stderr, "cmdline = %s\n", cmdline);
  fprintf(stderr, "arg1 = %s\n", arg1);
  fprintf(stderr, "arg2 = %s\n", arg2);
  fprintf(stderr, "arg3 = %s\n", arg3);
  fprintf(stderr, "arg4 = %s\n", arg4);
#endif
  for (cmd = 0; cmd < sizeof(unmacrocommands)/4; cmd++)
    if (!strncmp(arg1, unmacrocommands[cmd], strlen(arg1)))
      break;
  if (cmd < sizeof(unmacrocommands)/4) {
    command[0] = cmd;
    lastcommand = cmd;
    switch (cmd) {
/*
  DISASSEMBLE - get sub-commands
*/
     case C_DISASSEMBLE:
      if (!strlen(arg2))
	return(0);
      for (cmd2 = 0; cmd2 < sizeof(disassembl)/4; cmd2++)
	if (!strncmp(arg2, disassembl[cmd2], strlen(arg2)))
	  break;
      command[1] = cmd2;
      if (cmd2 == sizeof(disassembl)/4) {
        comderr("DISASSEMBLE", 1, disassembl, sizeof(disassembl)/4);
	command[0] = -1;
      } else  if (cmd2 == C0_UNTIL) {
	for (cmd3 = 0; cmd3 < sizeof(disassembleuntil)/4; cmd3++)
	  if (!strncmp(arg3, disassembleuntil[cmd3], strlen(arg3)))
	    break;
	  command[2] = cmd3;
	  if (cmd3 == sizeof(disassembleuntil)/4) {
	    command[0] = -1;
	    comderr("DISASSEMBLE UNTIL", 1, disassembleuntil,
	     sizeof(disassembleuntil)/4);
	  } else if (cmd3 == C0x22_PC) {
	    if (!sscanf(arg4, "%x", &command[3])) {
	      fprintf(stderr, "bad address for DISASSEMBLE UNTIL PC\n");
              command[3] = 0;
	    }
	  }
      }
      break;
/*
  SET - get sub-commands
*/
     case C_SET:		/* SET */
      for (cmd2 = 0; cmd2 < sizeof(setshow)/4; cmd2++)
	if (!strncmp(arg2, setshow[cmd2], strlen(arg2)))
	  break;
      if (cmd2 == sizeof(setshow)/4) {
        comderr("SET", 1, setshow, sizeof(setshow)/4);
	command[0] = -1;
      } else {
	command[1] = cmd2;
	switch(cmd2) {
/*
  SET BYTE/WORD/LONG/QUAD/OCTA/RANGE subcommands
*/
	 case C1_RANGE:
	  if (!sscanf(arg4, "%x", &command[3])) {
	    fprintf(stderr, "SET RANGE has invalid address\n");
	    command[0] = -1;
	    return(0);
	  }
	 case C1_BYTE:
	 case C1_WORD:
	 case C1_LONG:
	 case C1_QUAD:
	 case C1_OCTA:
	  for (cmd3 = 0; cmd3 < sizeof(setshowbyte)/4; cmd3++)
	    if (!strncmp(arg3, setshowbyte[cmd3], strlen(arg3)))
	      break;
	  if (cmd3 == sizeof(setshowbyte)/4) {
	    command[0] = -1;
            comderr("SET BYTE/LONG...", 1, setshowbyte, sizeof(setshowbyte)/4);
	      break;
	  } else {
	    command[2] = cmd3;
	  }
	  break;
/*
  SET MODE subcommands
*/
	 case C1_MODE:
	  for (cmd3 = 0; cmd3 < sizeof(setmode)/4; cmd3++)
	    if (!strncmp(arg3, setmode[cmd3], strlen(arg3)))
	      break;
	  if (cmd3 == sizeof(setmode)/4) {
	    command[0] = -1;
            comderr("SET MODE", 1, setmode, sizeof(setmode)/4);
	    break;
	  } else {
	    command[2] = cmd3;
	  }
	  break;
/*
  SET CASE subcommands
*/
	 case C1_CASE:
	  for (cmd3 = 0; cmd3 < sizeof(setcase)/4; cmd3++)
	    if (!strncmp(arg3, setcase[cmd3], strlen(arg3)))
	      break;
	  if (cmd3 == sizeof(setcase)/4) {
	    command[0] = -1;
            comderr("SET CASE", 1, setcase, sizeof(setcase)/4);
	      break;
	  } else {
	    command[2] = cmd3;
	  }
	  break;
	}
      }
      break;
/*
  SHOW - get sub-commands
*/
     case C_SHOW:		/* SHOW */
      for (cmd2 = 0; cmd2 < sizeof(setshow)/4; cmd2++)
	if (!strncmp(arg2, setshow[cmd2], strlen(arg2)))
	  break;
      if (cmd2 == sizeof(setshow)/4) {
	command[0] = -1;
        comderr("SHOW", 1, setshow, sizeof(setshow)/4);
      } else {
	command[1] = cmd2;
	switch(cmd2) {
/*
  SHOW BYTE/WORD/LONG/QUAD/OCTA subcommands
*/
	 case C1_BYTE:
	 case C1_WORD:
	 case C1_LONG:
	 case C1_QUAD:
	 case C1_OCTA:
	  break;
/*
  SHOW MODE subcommands
*/
	 case C1_MODE:
	  break;
/*
  SHOW CASE subcommands
*/
	 case C1_CASE:
	  break;
/*
  SHOW STACK subcommands
*/
	 case C1_STACK:
	  break;
	}
      }
      break;
/*
  GOTO - get address
*/
     case C_GOTO:		/* GOTO */
      if (!sscanf(arg2, "%x", &command[1])) {
	fprintf(stderr, "bad address for GOTO\n");
        command[1] = 0;
      }
      break;
/*
  PUSH current pc on stack
*/
     case C_PUSH:		/* PUSH */
      break;
/*
  POP current pc on stack
*/
     case C_POP:		/* POP */
      break;
/*
  SYMBOL - symbol table names
*/
     case C_SYMBOL:		/* SYMBOL */
      for (cmd2 = 0; cmd2 < sizeof(symbol)/4; cmd2++)
	if (!strncmp(arg2, symbol[cmd2], strlen(arg2)))
	  break;
      if (cmd2 == sizeof(symbol)/4) {
	command[0] = -1;
        comderr("SYMBOL", 1, symbol, sizeof(symbol)/4);
      } else {
	command[1] = cmd2;
	switch(cmd2) {
/*
  SYMBOL DEFINE
*/
	 case C13_DEFINE:
	  break;
/*
  SYMBOL LIST
*/
	 case C13_LIST:
	  break;
/*
  SYMBOL REMOVE
*/
	 case C13_REMOVE:
	  break;
	}
      }
      break;
     }
   } else {
    command[0] = -1;
    return(-1);
   }
#ifdef	debug
  fprintf(stderr, "command[0,1,2,3] = %d, %d, %d, %d\n", command[0],
   command[1], command[2], command[3]);
  fprintf(stderr, "lastcommand = %d\n", lastcommand);
#endif
  return(0);
}
/*
  command error display routine
*/
comderr(t1, argno, textlist, textsize)
char *t1;
int argno;
char **textlist;
int textsize;
{
  int i, linesize=0, newline;

  fprintf(stderr, "Syntax error for %s (argument %d)\n", t1, argno);
  fprintf(stderr, "Valid arguments are:\n");
  for (i = 0; i < textsize; i++) {
    newline = 0; 
    fprintf(stderr, "%s ", textlist[i]);
    linesize += strlen(textlist[i]) + 1;
    if (linesize > 65) {
      fprintf(stderr, "\n");
      newline = 1;
      linesize = 0;
    }
  }
  if (!newline)
    fprintf(stderr, "\n");
}
