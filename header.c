/*
  Get header information

  Copyright (c) 1989 By Robert Thomas Chojnacki
*/
#include <stdio.h>
char *malloc();
#include "unmacro.h"
#define header_main
#include "header.h"

extern FILE *sfp;

header()
{
  char block[512];
  int i, j;
/*
 read in the first block
*/
  if (image_header == 'y') {
    hb[0] = malloc(512);
    fread(hb[0], 512, 1, sfp);
/*
  Fixup portion of header
*/
    h_size = ghw(0);
    b_offset = ghw(2);
    c_offset = ghw(4);
    d_offset = ghw(6);
    e_offset = ghw(8);
    h_majid = ghw(12);
    h_minid = ghw(14);
    h_blocks = hb[0][16] & 0xff;
    h_itype = hb[0][17] & 0xff;
    h_rpriv[0] = ghl(20);
    h_rpriv[1] = ghl(24);
    h_iochans = ghw(28);
    h_iosegpag = ghw(30);
    h_iflags = ghl(32);
    h_gsid = ghl(36);
    h_sysver = ghl(40);
/*
  Transfer address array
*/
    h_transfer1 = ghl(b_offset);
    h_transfer2 = ghl(b_offset+4);
    h_transfer3 = ghl(b_offset+8);
    if (h_transfer1)
      fprintf(stderr, "First transfer address = %08X\n", h_transfer1);
    if (h_transfer2)
      fprintf(stderr, "Second transfer address = %08X\n", h_transfer2);
    if (h_transfer3)
      fprintf(stderr, "Third transfer address = %08X\n", h_transfer3);
/*
  read in the rest of the header blocks
*/
    if (h_blocks != 1) {
      fprintf(stderr, "more than one image header block\n");
      for (i=1;i<h_blocks;i++) {
        if ((hb[i] = malloc(512)) && (i < MAXHB)) {
  	  fread(hb[i], 512, 1, sfp);
        } else {
  	  fprintf(stderr, "could not allocate room for header block %d\n", i);
  	  fread(block, 512, 1, sfp);
        }
      }
    }
/*
  interpret image section descriptors
*/
    i = h_size;		/* this is where the isd's start */
    while ((ghw(i) > 0) || (i/512 < h_blocks-1)) {
      if ((i/512 < h_blocks-1) && (ghw(i) < 0))
	i += (512 - i%512);	/* bump it to start of next block */
      if ((h_isd[h_nisd] = malloc(sizeof(h_isd_type))) && (h_nisd < MAXISD)) {
        h_isd[h_nisd]->size = ghw(i);
        h_isd[h_nisd]->pages = ghw(i+2);
        h_isd[h_nisd]->vpn = ghw(i+4);
        h_isd[h_nisd]->pfc = ghb(i+7);
        h_isd[h_nisd]->flags = ghl(i+8);
	if (!(h_isd[h_nisd]->flags & ISD_DZERO))
          h_isd[h_nisd]->bvbn = ghl(i+12);
	else
          h_isd[h_nisd]->bvbn = 0;
	if (h_isd[h_nisd]->flags & ISD_GLOBAL) {
          h_isd[h_nisd]->gsid = ghl(i+16);
          h_isd[h_nisd]->gsnlen = ghb(i+20) & 0xff;
	  for (j=0; j<h_isd[h_nisd]->gsnlen; j++)
	    h_isd[h_nisd]->gsn[j] = ghb(i+21+j);
	}
/* find number of pages in memory and base address */
	if (!(h_isd[h_nisd]->flags & ISD_GLOBAL) &&
	 !(h_isd[h_nisd]->flags & 0xff000000)) {
	  mem_pages = h_isd[h_nisd]->pages + h_isd[h_nisd]->vpn;
	  if ((h_isd[h_nisd]->vpn & 0xffff) < lowest_vpn)
	    lowest_vpn = h_isd[h_nisd]->vpn & 0xffff;
	}
      } else {
        fprintf(stderr, "could not allocate room for isd %d\n", h_isd);
      }
      i += ghw(i);
      h_nisd++;
    }
/*
  no image header
*/
  } else {
    h_size = 0;
    b_offset = 0;
    c_offset = 0;
    d_offset = 0;
    e_offset = 0;
    h_majid = 0;
    h_minid = 0;
    h_blocks = 0;
    h_itype = 0;
    h_rpriv[0] = 0;
    h_rpriv[1] = 0;
    h_iochans = 0;
    h_iosegpag = 0;
    h_iflags = 0;
    h_gsid = 0;
    h_sysver = 0;
    h_transfer1 = 0;
    h_transfer2 = 0;
    h_transfer3 = 0;
    mem_pages = tot_blocks;
  }
}

char ghb(i)
int i;
{
  int b;

  i &= 0xffff;
  b = i/512;
  i %= 512;
  return (hb[b][i] & 0xff);
}

short int ghw(i)
int i;
{
  int b;

  i &= 0xffff;
  b = i/512;
  i %= 512;
  return (((hb[b][i] & 0xff) + (hb[b][i+1] & 0xff)*256) & 0xffff);
}

int ghl(i)
int i;
{
  int b;

  i &= 0xffff;
  b = i/512;
  i %= 512;
  return ((hb[b][i] & 0xff) + (hb[b][i+1] & 0xff)*256
   + (hb[b][i+2] & 0xff)*65536 + (hb[b][i+3] & 0xff)*16777216);
}

/*
  display header information
*/
disph(fp)
FILE *fp;
{
  int i, j, gsnlen;

  if (image_header == 'y') {
    fprintf(fp, ";header statistics:\n");
    fprintf(fp, ";\theader size = %d. bytes\n", h_size);
    fprintf(fp, ";\toffsets to b, c, d, e = %04X, %04X, %04X, %04X\n",
     b_offset, c_offset, d_offset, e_offset);
    fprintf(fp, ";\tmajor id = %02X, minor id = %02X\n", h_majid, h_minid);
    fprintf(fp, ";\tnumber of header blocks = %d.\n", h_blocks);
    fprintf(fp, ";\timage type, priv mask = %02X, %08X, %08X\n", h_itype,
     h_rpriv[0], h_rpriv[1]);
    fprintf(fp, ";\ti/o channels = %d., i/o seg pages = %d.\n", h_iochans,
     h_iosegpag);
    fprintf(fp, ";\timage flags = %08X, gsid = %08X, sys version = %08X\n",
     h_iflags, h_gsid, h_sysver);
    if (h_iflags & FLAGS_LNKDEBUG)
      fprintf(fp, ";\t    LNKDEBUG\n");
    if (h_iflags & FLAGS_LNKNOTFR)
      fprintf(fp, ";\t    LNKNOTFR\n");
    if (h_iflags & FLAGS_NOP0BUFS)
      fprintf(fp, ";\t    NOP0BUFS\n");
    if (h_iflags & FLAGS_PICIMG)
      fprintf(fp, ";\t    PICIMG\n");
    if (h_iflags & FLAGS_P0IMAGE)
      fprintf(fp, ";\t    P0IMAGE\n");
    if (h_iflags & FLAGS_DBGDMT)
      fprintf(fp, ";\t    DBGDMT\n");
    if (h_iflags & FLAGS_INISHR)
      fprintf(fp, ";\t    INISHR\n");
    fprintf(fp, ";\ttransfer addresses = %08X, %08X, %08X\n", h_transfer1,
     h_transfer2, h_transfer3);
    fprintf(fp, ";image section descriptors:\n");
    for (i=0; i<h_nisd; i++) {
      fprintf(fp, ";    isd %d\n", i);
      fprintf(fp, ";\tsize = %04X", h_isd[i]->size & 0xffff);
      fprintf(fp, " pages = %04X", h_isd[i]->pages & 0xffff);
      fprintf(fp, " starting vpn = %04X", h_isd[i]->vpn & 0xffff);
      fprintf(fp, " pfc = %02X\n", h_isd[i]->pfc & 0xff);
      fprintf(fp, ";\tflags = %08X", h_isd[i]->flags);
      if (!(h_isd[i]->flags & ISD_DZERO))
        fprintf(fp, " base vbn = %04X", h_isd[i]->bvbn);
      putc('\n', fp);
      if (h_isd[i]->flags & ISD_GLOBAL)
	fprintf(fp, ";\t    Global section\n");
      if (h_isd[i]->flags & ISD_COR)
	fprintf(fp, ";\t    Copy on reference\n");
      if (h_isd[i]->flags & ISD_DZERO)
	fprintf(fp, ";\t    Demand zero\n");
      if (h_isd[i]->flags & ISD_WRITE)
	fprintf(fp, ";\t    Writable\n");
      if (h_isd[i]->flags & ISD_LC)
	fprintf(fp, ";\t    Last cluster in P0 space\n");
      if (h_isd[i]->flags & ISD_CA)
	fprintf(fp, ";\t    Copy always\n");
      if (h_isd[i]->flags & ISD_BI)
	fprintf(fp, ";\t    Based Image\n");
      if (h_isd[i]->flags & ISD_FV)
	fprintf(fp, ";\t    Fixup vector\n");
      if (h_isd[i]->flags & ISD_ISP)
	fprintf(fp, ";\t    Image section is protected\n");
      if (h_isd[i]->flags & ISD_GLOBAL) {
        fprintf(fp, ";\tgsid = %08X name = '", h_isd[i]->gsid);
        gsnlen = h_isd[i]->gsnlen;
	for (j=0; j<gsnlen; j++)
	  putc(h_isd[i]->gsn[j], fp);
	fprintf(fp, "'\n");
      }
    }
  } else {
    fprintf(fp, ";no file header\n");
  }
}
