/*
  Image header definitions

  Copyright (c) 1989 By Robert Thomas Chojnacki
*/

#define	MAXHB	32		/* max number of header blocks */
#define MAXISD	256		/* max number of isd's */

typedef struct h_isd_struct {	/* isd = image section descriptor */
  short int size;		/* size of isd in bytes */
  short int pages;		/* number of pages in this section */
  short int vpn;		/* starting virtual page number */
  char fill_1;
  char pfc;			/* page fault cluster */
  int flags;			/* section flags */
  int bvbn;			/* base virtual block number */
  int gsid;			/* identifier for global section */
  int gsnlen;			/* length of global section name */
  char gsn[44];			/* global section name */
} h_isd_type;

#define ISD_GLOBAL	1	/* global section */
#define ISD_COR		2	/* copy on reference */
#define ISD_DZERO	4	/* demand zero */
#define ISD_WRITE	8	/* writeable */
#define ISD_MCF		112	/* match control field */
#define ISD_LC		128	/* last cluster in P0 space */
#define ISD_CA		256	/* copy always */
#define ISD_BI		512	/* based image */
#define ISD_FV		1024	/* fixed vector */
#define ISD_VCIS	262144	/* vector contained in image section */
#define ISD_ISP		524288	/* image section is protected */

#define FLAGS_LNKDEBUG	1
#define FLAGS_LNKNOTFR	2
#define FLAGS_NOP0BUFS	4
#define FLAGS_PICIMG	8
#define FLAGS_P0IMAGE	16
#define FLAGS_DBGDMT	32
#define FLAGS_INISHR	64

char ghb();			/* get header byte */
short int ghw();		/* get header word */

#ifdef header_main
char *hb[MAXHB];
char image_header;
h_isd_type *h_isd[MAXISD];
int h_nisd=0;
int h_size;
int b_offset, c_offset, d_offset, e_offset;
int h_majid, h_minid, h_blocks, h_itype;
int h_rpriv[2], h_iochans, h_iosegpag, h_iflags, h_gsid, h_sysver;
int h_transfer1, h_transfer2, h_transfer3;
int mem_pages=0, lowest_vpn = 0x7fffffff;
#else
extern char *hb[];
extern char image_header;
extern h_isd_type *h_isd[];
extern int h_nisd;
extern int h_size;
extern int b_offset, c_offset, d_offset, e_offset;
extern int h_majid, h_minid, h_blocks, h_itype;
extern int h_rpriv[2], h_iochans, h_iosegpag, h_iflags, h_gsid, h_sysver;
extern int h_transfer1, h_transfer2, h_transfer3;
extern int mem_pages, lowest_vpn;
#endif