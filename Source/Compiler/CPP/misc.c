/*
	@(#)misc.c	2.2.3	10/5/87
*/
/* miscellaneous routines for the c pre-processor */

#include "prep.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

fatal(errstr)
char	*errstr;
{
	extern int errno;

	doerr(symptr-lbase,errstr,lineno,FATERR);
	errexit(SUCCESS);		/* let cc/c68 handle the error report */
}


error(s)
char	*s;
{
	doerr(symptr-lbase,s,lineno,ERROR);
}


lerror(s)
char	s[];
{
	doerr(lptr-lbase,s,lineno,ERROR);
}


doerr(n, errstr, lno, ertype)
int		n, lno, ertype;
char	errstr[];
{
	register char	*tmp;

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


putesc(type, arg, arg1)
{
	if (!aflag) {
		switch (type) {
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


int
hash(word)
char	*word;
{
	register int	n = 0, c;

	while (c = *word++) n += c;
	return (n & 127);
}


char *
grab(size)
unsigned	size;
{
	register char	*p;

	if (size & 1) ++size;			/* a certain OS pukes on odd requests */
#ifdef OSK
	if ((p = (char*) ebrk(size)) == -1)
#elif defined(OS9)
	if ((p = (char*) sbrk(size)) == -1)
#else
	if ((p = (char*) malloc(size)) == NULL)
#endif
		fatal("out of memory");
	return p;
}


char *
copystr(p, s, n)
char	*p, *s;
int		n;
{
	while (n-- > 0) *p++ = *s++;

	return p;
}


char *
savestr(s)
char	*s;
{
    return (char *)strcpy(grab(strlen(s) + 1), s);
}
