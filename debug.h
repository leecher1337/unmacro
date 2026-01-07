/*
  Remove the following comment to display debug messages

  Copyright (c) 1988 By Robert Thomas Chojnacki, All rights reserved.
*/
/* #define	debug */
#ifdef	debug
#define	debmsg(a)	fprintf(stderr, a);
#else
#define	debmsg(a)	;
#endif