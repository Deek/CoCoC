/*
 * sed -- stream  editor
 *
 * Copyright 1975 Bell Telephone Laboratories, Incorporated
 *
 * Owner: lem
 */

#include <stdlib.h>

#ifdef _linux
#define register
#endif

#define CBRA	1
#define CCHR	2
#define CDOT	4
#define CCL	6
#define CNL	8
#define CDOL	10
#define CEOF	11
#define CKET	12
#define CNULL	13
#define CLNUM	14
#define CEND	16
#define CDONT	17
#define CBACK	18

#define STAR	01

#if defined(_OS9) || defined(_OSK)
# include <lowio.h>
# ifndef O_RDONLY	/* We want to use the standard name */
#  define O_RDONLY	READ
# endif
# define BLOCKSIZE	256
#else
# include <fcntl.h>	/* for open(), O_RDONLY */
# define BLOCKSIZE	512
#endif
#define NLINES	256
#define DEPTH	20
#define PTRSIZE	100
#define RESIZE	5000
#define ABUFSIZE	20
#define LBSIZE	4000
#define ESIZE	256
#define LABSIZE	50
#define NBRA	9

#ifndef MAIN
# define GLOBAL extern
#else
# define GLOBAL
#endif

GLOBAL FILE	*fin;
GLOBAL struct reptr	*abuf[ABUFSIZE];
GLOBAL struct reptr	**aptr;
GLOBAL char	*lastre;
GLOBAL char	ibuf[BLOCKSIZE];
GLOBAL char	*cbp;
GLOBAL char	*ebp;
GLOBAL char	genbuf[LBSIZE];
GLOBAL char	*loc1;
GLOBAL char	*loc2;
GLOBAL char	*locs;
GLOBAL char	seof;
GLOBAL char	*reend;
GLOBAL char	*lbend;
GLOBAL char	*hend;
GLOBAL struct reptr	*ptrend;
GLOBAL int	eflag;
GLOBAL int	dolflag;
GLOBAL int	sflag;
GLOBAL int	jflag;
GLOBAL int	numbra;
GLOBAL int	delflag;
GLOBAL long	lnum;
GLOBAL char	linebuf[LBSIZE+1];
GLOBAL char	holdsp[LBSIZE+1];
GLOBAL char	*spend;
GLOBAL char	*hspend;
GLOBAL int	nflag;
GLOBAL int	gflag;
GLOBAL char	*braelist[NBRA];
GLOBAL char	*braslist[NBRA];
GLOBAL long	tlno[NLINES];
GLOBAL int	nlno;
GLOBAL char	fname[12][40];
GLOBAL FILE	*fcode[12];
GLOBAL int	nfiles;

#define ACOM	01
#define BCOM	020
#define CCOM	02
#define CDCOM	025
#define CNCOM	022
#define COCOM	017
#define CPCOM	023
#define DCOM	03
#define ECOM	015
#define EQCOM	013
#define FCOM	016
#define GCOM	027
#define CGCOM	030
#define HCOM	031
#define CHCOM	032
#define ICOM	04
#define LCOM	05
#define NCOM	012
#define PCOM	010
#define QCOM	011
#define RCOM	06
#define SCOM	07
#define TCOM	021
#define WCOM	014
#define CWCOM	024
#define YCOM	026
#define XCOM	033

GLOBAL char	*cp;
#ifndef __STDC__
typedef union reptr {
	struct {
		char	*ad1;
		char	*ad2;
		char	*re1;
		char	*rhs;
		FILE	*fcode;
		char	command;
		char	gfl;
		char	pfl;
		char	inar;
		char	negfl;
	} reptr1;
	struct {
		char	*ad1;
		char	*ad2;
		union reptr	*lb1;
		char	*rhs;
		FILE	*fcode;
		char	command;
		char	gfl;
		char	pfl;
		char	inar;
		char	negfl;
	} reptr2;
} reptr;
#else
typedef struct reptr {
	char	*ad1;
	char	*ad2;
	union {
		char	*re1;
		struct reptr	*lb1;
	};
	char	*rhs;
	FILE	*fcode;
	char	command;
	char	gfl;
	char	pfl;
	char	inar;
	char	negfl;
} reptr;
#endif
GLOBAL reptr	ptrspace[PTRSIZE], *rep;

GLOBAL char	respace[RESIZE];

struct label {
	char	asc[9];
	struct reptr	*chain;
	struct reptr	*address;
};
GLOBAL struct label	ltab[LABSIZE];

GLOBAL struct label	*lab;
GLOBAL struct label	*labend;

GLOBAL int	f;
GLOBAL int	depth;

GLOBAL int	eargc;
GLOBAL char	**eargv;

extern char	bittab[];

GLOBAL struct reptr	**cmpend[DEPTH];
GLOBAL struct reptr	*pending;
GLOBAL char	*badp;
GLOBAL char	bad;

char	*compile();
char	*ycomp();
char	*address();
char	*text();
char	*compsub();
struct label	*search();
char	*gline();
char	*place();
