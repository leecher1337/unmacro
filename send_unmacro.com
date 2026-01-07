$ FTP delta.eecs.nwu.edu
user "chojnack"
gamma5
set remote "unmacro-may89"
put bugs.txt "bugs.txt"
put commands.c "commands.c"
put commands.h "commands.h"
put debug.h "debug.h"
put disassemble.c "disassemble.c"
put displacements.c "displacements.c"
put getdata.c "getdata.c"
put getdata.h "getdata.h"
put header.c "header.c"
put header.h "header.h"
put instructions.c "instructions.c"
put i_type.c "i_type.c"
put masks.c "masks.c"
put operands.c "operands.c"
put posneg.c "posneg.c"
put posneg.h "posneg.h"
put symtab.c "symtab.c"
put symtab.h "symtab.h"
put undata.c "undata.c"
put unmacro.c "unmacro.c"
put unmacro.h "unmacro.h"
dir
close
exit
$ Exit