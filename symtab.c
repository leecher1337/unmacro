/*
  Routines to add, remove, modify, and retrieve symbol names and values to
  and from a symbol table.

  Copyright (c) 1989 by Robert Thomas Chojnacki
*/
#include <stdio.h>
char *malloc();
#include <string.h>

#define symtab		/* for symtab.h */
#include "symtab.h"

/*
  Add a name to the symbol table
*/
sym_add(name, value)
char *name;
int value;
{
  int len;
/*
  make sure everything is OK
*/
  if (sym_count == MAXSYM) {
    fprintf(stderr, "sym_add: symbol table full\n");
    return 1;
  } else if ((len = strlen(name)) > SYMNAMALLO) {
    name[SYMNAMALLO-1] = 0;
    fprintf(stderr, "sym_add: symbol name truncated to '%s'\n", name);
  } else if (sym_findname(value)) {
    fprintf(stderr, "sym_add: symbol value already assigned to '%s'\n",
     sym_findname(value));
    return 2;
  } else if (!value) {
    fprintf(stderr, "sym_add: cannot use value of zero.\n");
    return 3;
  } else if (sym_findvalue(name)) {
    fprintf(stderr, "sym_add: symbol name already assigned to '%08X'\n",
     sym_findvalue(name));
    return 4;
  }
/*
  allocate some buffer space for name
*/
  if (!(sym_name[sym_count] = malloc(len))) {
    fprintf(stderr, "sym_add: unable to allocate %d bytes\n", len);
    return 5;
  }
/*
  insert name and value then leave
*/
  strcpy(sym_name[sym_count], name);
  sym_value[sym_count++] = value;
  return 0;
}

/*
  Find name by value
*/
char *sym_findname(value)
int value;
{
  int i;

  for (i=0; i<sym_count; i++)
    if (sym_value[i] == value)
      return sym_name[i];

  return 0;
}

/*
  Find value by name;
*/
sym_findvalue(name)
char *name;
{
  int i;

  for (i=0; i<sym_count; i++)
    if (!strcmp(sym_name[i], name))
      return sym_value[i];

  return 0;
}
