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


lwarning(s)
char	s[];
{
	doerr(lptr-lbase,s,lineno,WARNLEV);
}


warning(s)
char	*s;
{
	doerr(symptr-lbase,s,lineno,WARNLEV);
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
		fprintf(out, "%d\n%d\n%s\n", lno, n, errstr);
	} else {
		fprintf(out, "%s line #%d - %s\n", (ertype == WARNLEV) ? "* warning" : " fatal", lno, errstr);
	}
}


putesc(type, arg, arg1)
char	*arg, *arg1;
{
	if (!arg1) arg1 = "";

	if (!aflag) {
		switch (type) {
		case NEWFNAME:
		case OLDFNAME:
		case PSECT:
			fprintf(out, "%c%c\n%s\n%s\n", ESCHAR, type, arg, arg1);
			break;
		case NEWLINO:
			fprintf(out,"%c%c\n%d\n",ESCHAR, type, (int)arg);
			break;
		default:
			fprintf(out,"%c%c\n%s\n",ESCHAR, type, arg);
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

#ifdef unix
	if (size & 1) ++size;			/* a certain OS pukes on odd requests */
#endif
#ifdef OSK
	if ((p = (char*) ebrk(size)) == -1)
#elif defined(OS9)
	if ((p = (char*) ibrk(size)) == -1)	/* was sbrk() */
#else
	if ((p = (char*) malloc(size)) == NULL)
#endif
		fatal("out of memory");
	return p;
}


char *
copystr(p, s, e)
register char	*p, *s;
int				e;
{
	/*
		if e is nonzero, we have been told to stringify s. Add quotes, and
		escape any backslashes or embedded quotes.

		Don't look too closely at this, you may go blind.
	*/
	if (e) {
		int	instr = 0;

		if (e == 1) *p++ = '"';
		while (s && *s) switch (*s) {
			case '\\':	/* only escape backslash inside strings */
				if (instr) *p++ = '\\';
				*p++ = *s++;
				if (instr && *s == '"') *p++ = '\\';
				*p++ = *s++;	/* eat the next char too */
				break;
			case '"':	/* just entered or exited a string */
				instr = ~instr;
				*p++ = '\\';
			default:
				*p++ = *s++;
		}
		if (e == 1) *p++ = '"';
	} else while (s && *s) {
		*p++ = *s++;
	}

	return p;
}


char *
savestr(s)
register char	*s;
{
	char		*nstr, *x = s;	/* save starting pos */
	unsigned	count;

	if (s) {
		while (*s) ++s;			/* find end of string */
		while (s > x && isspace (*(s-1))) --s;	/* skip trailing spaces */
	}
	count = (s - x);
	nstr = grab (count + 1);
	strncpy (nstr, x, count);
	*(nstr+count) = '\0';

	return nstr;
}
