/*
	$Id$
*/
#define MAIN
#include "prep.h"
#include "history.c"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int iftop;
#if defined(OSK) || defined(unix) || defined(vms)
extern char	*getenv();
#endif

direct char
	*defdev;			/* default device name from command line */

static direct char
	*devptr,			/* ptr to dev tbl form sysdevice */
	*pedit = "0",		/* default psect edition */
	*fname;				/* name of file to pre-process */

cmdstruct cmds[] =
{
	"define",	DEFINE,
	"elif",		ELIF,
	"include",	INCLUDE,
	"ifdef",	IFDEF,
	"ifndef",	IFNDEF,
	"endif",	ENDIF,
	"if",		IF,
	"else",		ELSE,
	"undef",	UNDEF,
	"asm",		ASM,
	"endasm",	ENDASM,
	"line",		LINE,
};

cmdstruct *endcmds = cmds + (sizeof cmds) / (sizeof(cmdstruct));

#ifdef DEBUG
static direct int dflag;
#endif

char mtemp[NAMESIZE+1];
static char curlinebuf[32];

errexit(n)
{
#if defined(OSK)
	if (devptr)
		detach(devptr);
#endif
	lerror("cpp fatal");
	exit(n);
}

int		nxtlno = 1;

int
main(argc,argv)
int argc;
char **argv;
{
	register char	*p, *q;
	register int	c;
	register char	*badflag = 0;
	register char	*outfname = NULL;

	/* resident system define name */
	addmac("OS9","1");			/* target operating system */
	addmac("_OS9","1");			/* more standard version */

	addmac("mc6809","1");			/* target cpu */
	addmac("__mc6809__","1");		/* target cpu */
	addmac("__MC6809__","1");

	addmac("_BIG_END","1");					/* OS9 endian marker */
/*	addmac("_LIL_END","1");					/* OS9 endian marker */

	/* the current line macro */
	(macline = addmac("__LINE__",""))->macdef->md_elem = curlinebuf;

	/* the current filename macro */
	(macfile = addmac("__FILE__",""))->macdef->md_elem = mfilename;

	/* process the arguments */
	while (--argc > 0) {
		if (*(p = *++argv) == '-') {
			while (*++p) {
				switch(*p) {
				case 'L':
				case 'l':
					lflag = 1;
					break;
				case 'E':
				case 'e':
					if (*++p == '=') pedit = p + 1;
					goto done;
				case 'D':
				case 'd':
					if (isalpha(*++p) || *p == '_') {
						q = p;
						while (isalnum(*q) || *q == '_') ++q;
						c = *q;
						*q = '\0';
						if (c == '=') q++;
						else q = "1";
						strncpy(mtemp, p, NAMESIZE);
						addmac(p, q);
						goto done;
					}
					else
					{
						badflag = *argv;
						goto done;
					}
				case 'U':
				case 'u':
					delmac(p + 1);
					goto done;
#ifdef DEBUG
				case 'x':
					dflag=1;
					break;
#endif
				case 'V':
				case 'v':
					if (*++p == '=') ++p;
					if (inclcount >= MAXLIBS) {
						fprintf(stderr, "too many include libraries (%d)\n",
							MAXLIBS);
						exit(FAILURE);
					}
					incl[inclcount++] = p;
					goto done;
				case 'a':
					aflag++;
					break;
				case 'g':
					gflag++;
					break;
				case 'o':
					if (*++p == '=') outfname = p + 1;
					goto done;
				default:
					fprintf(stderr, "cpp: unknown option -%c\n", *p);
					errexit(FAILURE);
				}
			}
done: ;
		}
		else if (fname != NULL) {
			fprintf(stderr, "cpp: only one file allowed\n");
			errexit(FAILURE);
		} else fname = *argv;
	}

/* get the default library name from environment if no -v given*/
	if (inclcount == 0) {
#ifdef OSK
		if (p = getenv("CDEF")) {
			incl[inclcount++] = p;
		} else
#endif
		{
			incl[inclcount++] = DEFDIR;
		}
	}

	if (!outfname) {
		out = stdout;	/* make sure output is good */
	} else if ((out = fopen(outfname,"w")) == NULL) {
		fprintf(stderr, "can't open output file %s (err=%d)\n", outfname,
			errno);
		exit(errno);		/* all hope lost now... */
	}

	if (!fname) {
		fname = "stdin";
		in = stdin;
	} else if ((in = fopen(fname,"r")) == NULL) {
		putesc(FATERR, fname);               /* couldn't open file */
		fprintf(out, "0\n0\ncan't open file %s (err=%d)\n", fname, errno);
		errexit(SUCCESS);	/* let c68 report the error */	
	}

	if (badflag) {
		putesc(FATERR, badflag);
		fprintf(out, "0\n0\nbad argument define\n");
		errexit(FAILURE);
	}

	strcpy(filename, fname);             /* current file name */
	strcpy(modname, makename(filename)); /* current module name */
	putesc(PSECT, modname, pedit);        /* new psect */
	putesc(NEWFNAME, filename, modname);  /* new filename */

	/* initialize preprocessor */
	preinit();

	/* read (and expand) a line */
	while (getline1() != EOF) {
		if (nxtlno != lineno - 1) {     /*   if line number changed */
			nxtlno = lineno - 1;        /*   tell the compiler */
			putesc(NEWLINO, nxtlno);
		}
		fprintf(out, "%s\n", line);     /*   write out expanded line */
		if (ferror(stdout)) {
			fatal("error writing output file");
		}
		++nxtlno;
	}

	if (iftop) {
		fatal("missing #endif");
	}

#if defined(OSK)
	if (devptr) {
		detach(devptr);
	}
#endif

#ifdef DEBUG
	if (dflag) {
		dumpmac();
	}
#endif
#ifdef MSTATS
	mstats("cpp exit");
#endif
	exit(SUCCESS);
}

setline(n)
int n;
{
	sprintf(curlinebuf, "%d", n);
}

char *
setfile(str)
char *str;
{
	sprintf(mfilename, "\"%s\"", str);

	return str;
}

#ifdef DEBUG
dumpmac()
{
	int		n;
	macro	*p;
	macdef	*m;

	for (n = 0; n < 127; ++n) {
		if ((p = mactab[n]) != NULL) {
			printf("slot %5d ", n);
			for (; p != NULL; p = p->next) {
				printf("  %9s %d ->", p->macname, p->macargs);
				for (m = p->macdef; m; m = m->next) {
					if (m->md_type)
						printf("<#%d>",m->md_type);
					else
						printf("%s",m->md_elem);
				}
				printf("<-\n");
			}
		}
	}
}
#endif


int
findcmd(s)
char *s;
{
	register cmdstruct	*p;

	for (p = cmds; p < endcmds; ++p) {
		if (strcmp(s, p->cmdname) == 0)
			return p->cmdtok;
	}
	return 0;
}


#if defined(OSK)
char *
sysdrive()
{
	register char	*p;

	if (defdev)
		return defdev;
	else if ((int)(devptr = attach((p = DEFNAME) + 1, 1)) != -1)
		return p;
	else {
		devptr = NULL;
		return DEVNAME;
	}
}
#else
char *
sysdrive()
{
	return DEFNAME;
}
#endif
