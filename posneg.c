/*
  defines the pos and neg functions

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
#include <stdio.h>
#include "debug.h"
char pos(b)
char b;
{
  debmsg("<pos>");
  return((b < 0) ? -b : b);
}

char neg(b)
char b;
{
  debmsg("<neg>");
  return((b < 0) ? b : -b);
}
