#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef ERROR
#define ERROR -1
#endif

#define LINEMAX 250  /* Max # of chars allowed + CR and EOF and NULL */
#define HTAB 9
#define LINEFEED 10
#define VTAB 11
#define FORMFEED 12
#define NEWLINE 13
#define SPACE 32
#define DEAD 3
#define IDNT_INIT(x) ((isalpha(x)) || ((x)=='_'))
#define IDNT_TYPE(x) (IDNT_INIT(x) || (x>='0' && x<='9'))
#define STRNG_TBL_MAX 6500
#define MAX_ARGS 4
#define MAX_LENGTH 120
#define MAX_NEST_IF 9   /* This value+1 = total # of nested #ifs allowed */
#define MAX_INCLUDE 8
#define MAX_DEFS 400
#define killine() *line=0

char *skipbl();
char *getoknum();
char *itoa();
long time();
char *ctime();
char *escseq();

#ifndef MAIN
extern
#endif
char line[LINEMAX+3];

#ifndef MAIN
extern
#endif
char eflag;

#ifndef MAIN
extern
#endif
int fpath[MAX_INCLUDE];

#ifndef MAIN
extern
#endif
int fptr
#ifdef MAIN
=0
#endif
;

#ifndef MAIN
extern
#endif
char *defnam[MAX_DEFS];  /* points to define names in $trng table */

#ifndef MAIN
extern
#endif
char *deftok[MAX_DEFS];  /* points to define tokens in $trng table */

#ifndef MAIN
extern
#endif
char *defarg[MAX_DEFS];  /* points to define args in $trng table */

#ifndef MAIN
extern
#endif
char dstrtbl[STRNG_TBL_MAX]; /* define $trng table */

#ifndef MAIN
extern
#endif
char *dptr; /* ptr to end of define $trng table */

#ifndef MAIN
extern
#endif
direct int defcntr      /* # of entries in def ptr arrays */
#ifdef MAIN
=5
#endif
;

#ifndef MAIN
extern
#endif
int iflvl       /* #if nest level cntr */
#ifdef MAIN
=-1
#endif
;

#ifndef MAIN    /* #if/#else/#elif/#endif variables */
extern
#endif
int ifstat[MAX_NEST_IF+1], ifnest[MAX_NEST_IF+1];

#ifndef MAIN    /* used to control processing of lines inside #ifs */
extern
#endif
int procsw  /* process switch (T=normal, F=discard) */
#ifdef MAIN
=TRUE
#endif
;

#ifndef MAIN
extern
#endif
int lnflag[2];  /* #line flag [0]=print b4 line [1]=print after */

#ifndef MAIN
extern
#endif
char ifnbuf[MAX_INCLUDE][35];   /* include file name buffer */

#ifndef MAIN
extern
#endif
int incln[MAX_INCLUDE]; /* include line number temp storage */

#ifndef MAIN
extern
#endif
int _line_; /* source file line number counter */

#ifndef MAIN
extern
#endif
int cflag
#ifdef MAIN
=FALSE
#endif
;

#ifndef MAIN
extern
#endif
int lflag   /* copy source lines to compiler to add to ASM output */
#ifdef MAIN     /* Only for microware compatibility */
=FALSE
#endif
;

#ifndef MAIN
extern
#endif
long clock;

#ifndef MAIN
extern
#endif
char *timdat,_date_[14],_time_[11]; /*5/24/2011 room for double-quotes*/

#ifndef MAIN
extern
#endif
char *stdef[5];

