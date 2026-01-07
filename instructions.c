/*
  this converts an opcode to an ascii string.  It returns the number
  of operands that the opcode requires.  If it returns -1 then this
  is a reserved instruction.

  Copyright (c) 1988, Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include "unmacro.h"
#include "debug.h"
instructions(opcode, line, alt, altline)
int opcode;	/* input: opcode to decode */
char *line;	/* output: text line with ascii instruction */
int *alt;	/* output: 1 if this may have a FD byte after the instrx,
                           2 if this has an alternate name (i.e. branches),
                           3 if this may have a FF byte after the instrx,
                           0 if no alterante possibilities */
char *altline;	/* output: alternate line */
{
  static char *opcodes[256] = {
   "halt", "nop", "rei", "bpt", "ret", "rsb", "ldpctx", "svpctx",
   "cvtps", "cvtsp", "index", "crc", "prober", "probew", "insque", "remque",
   "bsbb", "brb", "bneq", "beql", "bgtr", "bleq", "jsb", "jmp",
   "bgeq", "blss", "bgtru", "blequ", "bvc", "bvs", "bcc", "bcs",
   "addp4", "addp6", "subp4", "subp6", "cvtpt", "mulp", "cvttp", "divp",
   "movc3", "cmpc3", "scanc", "spanc", "movc5", "cmpc5", "movtc", "movtuc",
   "bsbw", "brw", "cvtwl", "cvtwb", "movp", "cmpp3", "cvtpl", "cmpp4",
   "editpc", "matchc", "locc", "skpc", "movzwl", "acbw", "movaw", "pushaw",
   "addf2", "addf3", "subf2", "subf3", "mulf2", "mulf3", "divf2", "divf3",
   "cvtfb", "cvtfw", "cvtfl", "cvtrfl", "cvtbf", "cvtwf", "cvtlf", "acbf",
   "movf", "cmpf", "mnegf", "tstf", "emodf", "polyf", "cvtfd", "Reserved",
   "adawi","Reserved","Reserved","Reserved","insqhi","insqti","remqhi","remqti",
   "addd2", "addd3", "subd2", "subd3", "muld2", "muld3", "divd2", "divd3",
   "cvtdb", "cvtdw", "cvtdl", "cvtrdl", "cvtbd", "cvtwd", "cvtld", "acbd",
   "movd", "cmpd", "mnegd", "tstd", "emodd", "polyd", "cvtdf", "Reserved",
   "ashl", "ashq", "emul", "ediv", "clrq", "movq", "movaq", "pushaq",
   "addb2", "addb3", "subb2", "subb3", "mulb2", "mulb3", "divb2", "divb3",
   "bisb2", "bisb3", "bicb2", "bicb3", "xorb2", "xorb3", "mnegb", "caseb",
   "movb", "cmpb", "mcomb", "bitb", "clrb", "tstb", "incb", "decb",
   "cvtbl", "cvtbw", "movzbl", "movzbw", "rotl", "acbb", "movab", "pushab",
   "addw2", "addw3", "subw2", "subw3", "mulw2", "mulw3", "divw2", "divbw3",
   "bisw2", "bisw3", "bicw2", "bicw3", "xorw2", "xorw3", "mnegw", "casew",
   "movw", "cmpw", "mcomw", "bitw", "clrw", "tstw", "incw", "decw",
   "bispsw", "bicpsw", "popr", "pushr", "chmk", "chme", "chms", "chmu",
   "addl2", "addl3", "subl2", "subl3", "mull2", "mull3", "divl2", "divl3",
   "bisl2", "bisl3", "bicl2", "bicl3", "xorl2", "xorl3", "mnegl", "casel",
   "movl", "cmpl", "mcoml", "bitl", "clrl", "tstl", "incl", "decl",
   "adwc", "sbwc", "mtpr", "mfpr", "movpsl", "pushl", "moval", "pushal",
   "bbs", "bbc", "bbss", "bbcs", "bbsc", "bbcc", "bbssi", "bbcci",
   "blbs", "blbc", "ffs", "ffc", "cmpv", "cmpzv", "extv", "extzv",
   "insv", "acbl", "aoblss", "aobleq", "sobgeq", "sobgtr", "cvtlb", "cvtlw",
   "ashp", "cvtlp", "callg", "calls", "xfc", "escd", "esce", "escf"};
  static char *alt_opcodes[256] = {
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "bnequ", "0", "beqlu", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "bgequ", "blssu",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "cvtdh", "cvtgf", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "addg2", "addg3", "subg2", "subg3", "mulg2", "mulg3", "divg2", "divg3",
   "cvtgb", "cvtgw", "cvtgl", "cvtrgl", "cvtbg", "cvtwg", "cvtlg", "acbg",
   "movg", "cmpg", "mnegg", "tstg", "emodg", "polyg", "cvtgh", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "addh2", "addh3", "subh2", "subh3", "mulh2", "mulh3", "divh2", "divh3",
   "cvthb", "cvthw", "cvthl", "cvtrhl", "cvtbh", "cvtwh", "cvtlh", "acbh",
   "movh", "cmph", "mnegh", "tsth", "emodh", "polyh", "cvthg", "0",
   "0", "0", "0", "0", "clro", "movo", "movao", "pushao",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "cvtfh", "cvtfg", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "clrf", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "movaf", "pushaf",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "0", "0",
   "0", "0", "0", "0", "0", "0", "cvthf", "cvthd",
   "0", "0", "0", "0", "0", "bugl", "bugw", "0"};
  static int opcode_flags[256] = {
   0, 0, 0, 0, 0, 0, 0, 0,	/* 00 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 2, 0, 2, 0, 0, 0,	/* 10 */
   0, 0, 0, 0, 0, 0, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0,	/* 20 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 1, 1, 0, 0, 0, 0,	/* 30 */
   0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1,	/* 40 */
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, -1,	/* 50 */
   0, -1, -1, -1, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1,	/* 60 */
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, -1,	/* 70 */
   0, 0, 0, 0, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0,	/* 80 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,	/* 90 */
   1, 1, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,	/* A0 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,	/* B0 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,	/* C0 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 2, 0, 0, 0,	/* D0 */
   0, 0, 0, 0, 0, 0, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0,	/* E0 */
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 1, 1,	/* F0 */
   0, 0, 0, 0, 0, 3, 3, 0};
  static int opcode_args[256] = {
   0, 0, 0, 0, 0, 0, 0, 0,	/* 00 */
   4, 4, 6, 4, 3, 3, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1,	/* 10 */
   1, 1, 1, 1, 1, 1, 1, 1,
   4, 6, 4, 6, 5, 6, 5, 6,	/* 20 */
   3, 3, 4, 4, 5, 5, 6, 6,
   1, 1, 2, 2, 3, 3, 3, 4,	/* 30 */
   4, 4, 3, 3, 2, 4, 2, 1,
   2, 3, 2, 3, 2, 3, 2, 3,	/* 40 */
   2, 2, 2, 2, 2, 2, 2, 4,
   2, 2, 2, 1, 5, 3, 2, -1,	/* 50 */
   2, -1, -1, -1, 2, 2, 2, 2,
   2, 3, 2, 3, 2, 3, 2, 3,	/* 60 */
   2, 2, 2, 2, 2, 2, 2, 4,
   2, 2, 2, 1, 5, 3, 2, -1,	/* 70 */
   3, 3, 4, 4, 1, 2, 2, 1,
   2, 3, 2, 3, 2, 3, 2, 3,	/* 80 */
   2, 3, 2, 3, 2, 3, 2, 3,
   2, 2, 2, 2, 1, 1, 1, 1,	/* 90 */
   2, 2, 2, 2, 3, 4, 2, 1,
   2, 3, 2, 3, 2, 3, 2, 3,	/* A0 */
   2, 3, 2, 3, 2, 3, 2, 3,
   2, 2, 2, 2, 1, 1, 1, 1,	/* B0 */
   1, 1, 1, 1, 1, 1, 1, 1,
   2, 3, 2, 3, 2, 3, 2, 3,	/* C0 */
   2, 3, 2, 3, 2, 3, 2, 3,
   2, 2, 2, 2, 1, 1, 1, 1,	/* D0 */
   2, 2, 2, 2, 1, 1, 2, 1,
   3, 3, 3, 3, 3, 3, 3, 3,	/* E0 */
   2, 2, 4, 4, 4, 4, 4, 4,
   4, 4, 3, 3, 2, 2, 2, 2,	/* F0 */
   6, 3, 2, 2, 0, 0, 0, -1};
/*
  comment
*/
  debmsg("<instuctions>");
  opcode = opcode & 0xff;		/* clear top bytes */
  strcpy(line, opcodes[opcode]);	/* return the opcode string */
  strcpy(altline, alt_opcodes[opcode]);	/* return the alternate opcode string */
  *alt = opcode_flags[opcode];		/* return type of opcode */
  return(opcode_args[opcode]);		/* return # of opcode arguments */
}
