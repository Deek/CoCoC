/*
	@(#)prep.h 2.2.2 10/5/87
*/
#include <stdio.h>
#include <ctype.h>
#include "defdir.h"

#ifdef MAIN
#define global
#else
#define global extern
#endif

#ifdef vms
#include <errno.h>
#include <errnodef.h>
#include <ssdef.h>
#define SUCCESS	SS$_NORMAL
#define FAILURE C$_ERROR
#else
#define SUCCESS	0
#define FAILURE	1
extern int errno;
#endif

#ifndef OS9
#define direct
#endif

#define TRUE		1
#define FALSE		0

#define FNAMSIZE    128     /* length of file names */
#define NAMESIZE    256     /* length of macro names */
#define HOLDSIZE    512     /* size of string hold buffer */
#define LINESIZE    512     /* size of line buffer */

#define MAXARGS     32      /* maximum number of macro arguments */
#define MAXLIBS		32		/* maximum number of include libraries */

#define ESCHAR      '#'     /* compiler escape character */
#define ERROR       '0'     /* error */
#define FATERR      '1'     /* fatal error */
#define ASSLINE     '2'     /* assembly source line */
#define NEWLINO     '5'     /* new source line number */
#define SOURCE      '6'     /* source comment line */
#define NEWFNAME    '7'     /* new file name */
#define OLDFNAME	'8'		/* old file name */
#define PSECT       'P'     /* generate PSECT directive */

#define HIDER       0x7f    /* a seldom-used ascii value to hide quotes */

#define SKIPSP      0       /* gch() skips white space and comments */
#define KEEPSP      1       /* gch() skips comments, keeps white space */

#define DEFINE      1
#define INCLUDE     2
#define IFDEF       3
#define IFNDEF      4
#define ENDIF       5
#define ELSE        6
#define UNDEF       7
#define ASM         8
#define ENDASM      9
#define LINE        10
#define IF			11
#define ELIF		12

typedef struct filstr {
    struct filstr	*next;				/* next file in include list */
    FILE			*fp;				/* file pointer */
    char			fname[FNAMSIZE];	/* file name string pointer */
    short			lno;				/* line number of include */
    char			modname[FNAMSIZE];	/* C module name */
} include;

typedef struct macdefst {
    struct macdefst	*next;			/* next element of macro definition */
    short			md_type;		/* element type 0=text, n=replace arg n*/
    char			*md_elem;		/* ptr to element string */
} macdef;

typedef struct macstr {
    struct macstr	*next;		/* next macro name in hash list */
    char			*macname;	/* macro name */
    short			macargs;	/* number of arguments in macro def */
								/* -1=none, 0=null '()', n=number of args */
    macdef			*macdef;	/* ptr to macro definition string */
    short			expanding;	/* expanding flag to support X3J11 3.8.3.4 */
} macro;

typedef struct{
    char	*cmdname;			/* command name */
    short	cmdtok;				/* command token */
} cmdstruct;

global direct FILE
	*out,					/* output file ptr */
    *in;                    /* current input file ptr */
    
global char
	*incl[MAXLIBS],			/* pointers to include libraries */
    line[LINESIZE],			/* input line buffer */
    lastline[LINESIZE],		/* saved last line */
    filename[LINESIZE],		/* current file name */
	mfilename[LINESIZE],	/* current file name for __FILE__ */
    modname[LINESIZE],		/* current module name */
    hold1buf[HOLDSIZE],		/* string holding buffer */
    holdbuf[HOLDSIZE];		/* string holding buffer */

global direct char
    *symptr,                /* current symbol ptr for errors */
    *lbase,					/* points at start of line being scanned for errors */
    *lptr,                  /* input line pointer */
    *pptr;                  /* output line pointer */

global direct int
	cch,					/* current input character */
    process,                /* process input lines */
    asmflag,                /* copying assembly source lines */
    lflag,                  /* output source lines as comments */
	aflag,					/* output intended for assembler */
	gflag,					/* global symbolic debugging flag */
	inclcount,				/* number of include libraries */
    lineno;                 /* current line number */

global direct include *inclptr;           /* include file list ptr */

global macro *mactab[128];         /* macro name hash table */
global direct macro
	*macline,				/* macro definition for __LINE__ */
	*macfile;				/* macro definition for __FILE__ */

global direct long
	sym,			/* lexical token */
	symval;         /* value associated with lexical token */

extern long parsexp();
extern long primary();

extern macro *addmac();
extern char *savestr(), *setfile(), *makename();
