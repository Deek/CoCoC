/*
	@(#)scan.c	2.2.1	6/15/87
*/
#include "prep.h"

/*
	Skill with psionic powers is required is
	useful to understand why such a complex function
	is required to do an apparently simple task.
	I hope the comments help...
*/

extern char *strcpy(), *copystr();
extern macro *findmac();

/*
 * added -- dodef, a flag indicating whether we should notice the
 * "defined" construct...ANSI X3J11's draft standard appears to
 * indicate that it is only to be paid attention to in #if and
 * #elif directives.
 *
 * Note the use of the "expanding" flag to support X3J11 3.8.3.4
 * wherein it is written that during the scan of the replacement
 * text of a macro, the name of the macro being replaced is not
 * re-replaced.
 *
 * added -- still more stuff!  This time, we allow the specification
 * of a function that we will use to snarf characters for the argument
 * list, because at the top level, we can cross newline boundaries!
 *
 * WARNING, WILL ROBINSON!
 * The simplest method of so doing causes us to mess with the scanned
 * line when we cross line boundaries, so we *must* copy actual parameters
 * of macros as we traverse them!  This is, as Steppenwolf would say,
 * "evil, wicked, mean, and nasty," but the philosophical basis of an
 * ANSI-conforming preprocessor is so different from this that the whole
 * megillah is going to come tumbling down soon anyway.
 *
 * (To see what is going on underneath, see parse.c and the top-level
 * arggch function, supergch().)
 */

scan(lp, dodef, arggch)
char	*lp;
int		dodef;
int		(*arggch)();
{
	register macro	*nptr;
	register char	*cptr, *savep, *saveb;
	char			savem[LINESIZE];
	
	/*
	 * make gch() give chars from the portion of the line yet to be scanned
	 * for macro names.
	 */
	savep = lptr;
	saveb = lbase;
	lbase = lptr = lp;
	gch(1);
	
	while (cch) {
		if (cch == '"' || cch == '\'') {
			char delim = cch;
			do {
				*pptr++ = cch;
				if (cch == '\\') {
					gch(0);
					*pptr++ = cch;
				}
				gch(0);
			} while (cch && cch != delim);
			*pptr++ = cch;
			gch(1);
		} else if (isalpha(cch) || cch == '_') {    /* possible macro name? */
			getword(savem, LINESIZE);
			if (dodef && strcmp(savem, "defined") == 0) {
				register int	paren;
				
				nxtch();
				if (cch == '(') {
					paren = TRUE;
					gch(1);
				} else
					paren = FALSE;
				if (isalpha(cch) || cch == '_') {
					getword(savem, LINESIZE);
					*pptr++ = findmac(savem) ? '1' : '0';
				} else
					error("expected identifier after defined");
				if (paren) {
					if (cch != ')')
						gch(1);
					gch(1);
				}
			} else if ((nptr = findmac(savem)) == NULL || nptr->expanding)
				pptr = copystr(pptr, savem, strlen(savem));
			else {
				/* it's a macro and it's not already expanding -- expand it */
				register char	*mptr, savec;
				register int	count, n, flag;
				register macdef	*mac;
				auto char		templine[LINESIZE];
				
				nptr->expanding = TRUE;	/* flag it to avoid re-expansion */
				
				flag = isspace(cch) ? cch : 0; /* white space following? */
				nxtch();
				if (nptr->macargs == 0 && cch != '(')
					lerror("macro arguments required");
				if (nptr->macargs >= 0) {
					if (cch == '(') {   /* arguments to process? */
						register int	nargc = 0;
						register char	*sscan;
						char			*nargv[MAXARGS];  /* -> actual args */
						int				narglen[MAXARGS]; /* and their lengths */
					  
						/* 
						 * Now buzz thru the actual arguments
						 * to determine the number and length of each.
						 */
						sscan = savem;
						(*arggch)(1);
						*sscan++ = cch;
						while (cch && cch != ')') {
							register int	bcount, a;

							/*  actual arg starts here */
							nargv[nargc] = sscan - 1;
							bcount = count = 0;
							while (cch) {
								switch(cch) {
								case ',':
									if (bcount == 0) {
										(*arggch)(1);
										*sscan++ = cch;
										goto loopend;
									}
									break;
								case ')':
									if (bcount) {
										--bcount;
										break;
									}
									goto loopend;
								case '(':
									++bcount;
									break;
								case '\'':
								case '"':
									a = cch;
									do {
										if (cch == '\\') {
											(*arggch)(0);
											*sscan++ = cch;
											++count;
										}
										(*arggch)(0);
										*sscan++ = cch;
										++count;
									} while (cch && cch != a);
								}
								++count;
								(*arggch)(1);
								*sscan++ = cch;
							}
				loopend:    narglen[nargc++] = count;
						}
						/* 
						 * Now copy out the macro definition
						 * text,  replacing the dummy arguments
						 * with the actual ones.
						 */
						if ((nptr->macargs == 0 && nargc) || nargc > nptr->macargs)
							lerror("too many macro arguments");
						gch(1);
						mptr = templine;
						for (mac = nptr->macdef; mac; mac = mac->next) {
							if (n = mac->md_type) {
								if (--n < nargc)
									mptr = copystr(mptr, nargv[n], narglen[n]);
								else
									lerror("too few macro arguments");
							} else
								mptr = copystr(mptr, mac->md_elem, 
										strlen(mac->md_elem));
						}
						*mptr = '\0';
						cptr = templine;
					} else if (nptr->macargs)
						lerror("macro arguments required");
				}
				/*
				 * else the macro has no parameters, just copy out
				 * the macro text.
				 */
				if (nptr->macargs <= 0)
					cptr = strcpy(templine, nptr->macdef->md_elem);
				/*
				 * Now, recursively expand the macro just expanded.
				 * (Hope the stack don't overflow...)
				 */
				savec = cch;
				scan(cptr, dodef, gch);
				cch = savec;
				if (flag)
					*pptr++ = flag;	/* put white space on output */
				if (cch == '(' && nptr->macargs == 0) { /* null argument list? */
					gch(1);                 /* eat the ) */
					gch(1);                 /* ..and prime up */
				}
				nptr->expanding = FALSE;
			}
		} else if (cch == '0') {       /* beware o' them hex constants */
			*pptr++ = cch;
			gch(1);
			if (cch == 'x' || cch == 'X') {
				do {
					*pptr++ = cch;
					gch(1);
				} while (isxdigit(cch));
			}
		} else {
			*pptr++ = cch;
			gch(1);
		}
	}

	/*
	 * Mark the current end of the output string, restore the expansion line
	 * pointer, and pray that it worked.
	 */
	*pptr = '\0';
	lptr = savep;
	lbase = saveb;
} 
