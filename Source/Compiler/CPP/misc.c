/*
	@(#)misc.c	2.2.3	10/5/87
*/
/* miscellaneous routines for the c pre-processor */

#include "prep.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void fatal(char *errstr)
{
	extern int errno;

	doerr(symptr-lbase,errstr,lineno,FATERR);
	errexit(SUCCESS);		/* let cc/c68 handle the error report */
}


void error(char *s)
{
	doerr(symptr-lbase,s,lineno,ERROR);
}


void lerror(char s[])
{
	doerr(lptr-lbase,s,lineno,ERROR);
}

void doerr(int n, char errstr[], int lno, int ertype)
{
	register char *tmp;

	if (lno == lineno)
		tmp = lbase;
	else if (lno == lineno - 1)
		tmp = lastline;
	else
		tmp="";

	if (!aflag) {
		putesc(ertype, tmp);
		fprintf(out, "%d\n", lno);
		fprintf(out, "%d\n%s\n", n, errstr);
	} else
		fprintf(out, " fatal line #%d - %s\n", lno, errstr);
}


void putesc(type, arg, arg1)
{
	if (!aflag)
	{
		switch (type)
		{
		case NEWFNAME:
		case OLDFNAME:
		case PSECT:
			fprintf(out, "%c%c\n%s\n%s\n", ESCHAR, type, arg, arg1);
			break;
		case NEWLINO:
			fprintf(out,"%c%c\n%d\n",ESCHAR,type,arg);
			break;
		default:
			fprintf(out,"%c%c\n%s\n",ESCHAR,type,arg);
			break;
		}
	}
}


int hash(char *word)
{
	register int n = 0, c;

	while (c = *word++)
		n += c;
	return (n & 127);
}


char *grab(unsigned size)
{
    register char *p;

	if (size & 1)
		++size;			/* a certain OS pukes on odd requests */
#ifndef OSK
    if ((p = (char*) malloc(size)) == NULL)
#else
    if ((p = (char*) ebrk(size)) == -1)
#endif
        fatal("out of memory");
    return p;
}

char *copystr(char *p, char *s, int n)
{
    while (n-- > 0)
	{
    	*p++ = *s++;
	}
	
    return p;
}

char *savestr(char *s)
{
    return (char *)strcpy(grab(strlen(s) + 1), s);
}
