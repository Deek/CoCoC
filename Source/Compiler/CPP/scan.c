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
	gch (KEEPSP);

	while (cch) {
		if (isspace (cch)) cch = ' ';	/* eat tabs, form feeds, etc. */
		if (cch == '"' || cch == '\'') {
			char delim = cch;
			do {
				*pptr++ = cch;
				if (cch == '\\') {
					gch (SKIPSP);
					*pptr++ = cch;
				}
				gch (SKIPSP);
			} while (cch && cch != delim);
			*pptr++ = cch;
			gch (KEEPSP);
		} else if (isalpha(cch) || cch == '_') {    /* possible macro name? */
			getword(savem, LINESIZE);
			if (dodef && strcmp(savem, "defined") == 0) {
				register int	paren;

				nxtch();
				if (cch == '(') {
					paren = TRUE;
					gch (KEEPSP);
				} else paren = FALSE;

				if (isalpha(cch) || cch == '_') {
					getword(savem, LINESIZE);
					*pptr++ = findmac(savem) ? '1' : '0';
				} else error("expected identifier after defined");

				if (paren) {
					if (cch != ')')
						gch (KEEPSP);
					gch (KEEPSP);
				}
			} else if ((nptr = findmac(savem)) == NULL || nptr->expanding) {
				pptr = copystr(pptr, savem, FALSE);
			} else {
				/* it's a macro and it's not already expanding -- expand it */
				char	*mptr, savec;
				int		n, flag;
				macdef	*mac;
				char	templine[LINESIZE];

				nptr->expanding = TRUE;	/* flag it to avoid re-expansion */
				flag = isspace(cch) ? ' ' : '\0';	/* whitespace following? */
				nxtch(); /* find first non-whitespace char */

				/*
				 * the macro is simple, just copy out the text.
				 */
				if (nptr->macargs < 0) {
					cptr = strcpy (templine, nptr->macdef->md_elem);
				} else if (nptr->macargs >= 0) {
					cptr = templine;
					if (cch != '(') { /* no arg list, expand to macro name */
						strcpy (templine, nptr->macname);
					} else {	/* have to process arguments */
						int		nargc = 0, plevel = 0;
						char	*sscan;
						char	*nargv[MAXARGS];	/* -> actual args */

						/* 
						 * Now buzz thru the actual arguments
						 * to count and find their contents
						 */
						nargv[0] = sscan = savem;
						while ((*arggch) (KEEPSP), cch) {
top:						if (cch == ')') {	/* Decrease depth, or end */
								if (plevel--) {
									*sscan++ = cch;
									continue;
								}
								*sscan++ = '\0';
								nargv[++nargc] = sscan;
								break;
							} else if (cch == '(') {	/* Increase depth */
								*sscan++ = cch;
								++plevel;
								continue;
							} else if (cch == ',') {	/* end arg */
								if (plevel) {
									*sscan++ = cch;
									continue;
								}
								*sscan++ = '\0';
								nargv[++nargc] = sscan;

								(*arggch) (SKIPSP); /* skip spaces after */
								while (isspace (cch))
									(*arggch) (SKIPSP);
								goto top;
							} else if (cch == '\'' || cch == '"') {	/* str/chr */
								int a = cch;

								*sscan++ = cch;
								do {
									if (cch == '\\') {
										(*arggch) (SKIPSP);
										*sscan++ = cch;
									}
									(*arggch) (SKIPSP);
									*sscan++ = cch;
								} while (cch && cch != a);
								continue;
							} else {
								*sscan++ = cch;
							}
						}

						/*
						 * Now we know how many arguments are used
						 * and needed, we can complain about the
						 * user's life choices.
						 */
						if ((nargc > nptr->macargs) && !(nptr->macvar)) {
							/* empty args exempted */
							if (nptr->macargs != 0 && *(nargv[0]))
								lerror ("too many macro arguments");
						} else if (nargc < nptr->macargs) {
							lerror ("too few macro arguments");
						}

						/* 
						 * Now copy out the macro definition
						 * text,  replacing the dummy arguments
						 * with the actual ones.
						 */
						gch (KEEPSP);
						mptr = templine;
						for (mac = nptr->macdef; mac; mac = mac->next) {
							if (n = mac->md_type) {	/* do we need to stringify? */
								int stringify = (n < 0) ? 1 : 0;
								if (stringify) n = -n;
								if (nptr->macvar && nptr->macargs == n) { /* var */
									int m = --n;

									if (stringify) {
										*mptr++ = '"';
										++stringify;
									}
									while (m < nargc) {
										mptr = copystr (mptr, nargv[m], stringify);
										if (++m < nargc) {
											*mptr++ = ',';
											*mptr++ = ' ';
										}
									}
									if (stringify) {
										*mptr++ = '"';
									}
								} else {
									if (--n < nargc)
										mptr = copystr (mptr, nargv[n], stringify);
								}
							} else
								mptr = copystr (mptr, mac->md_elem, FALSE);
						}
						*mptr = '\0';
					}
				}

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
					gch (KEEPSP);	/* eat the ) */
					gch (KEEPSP);	/* ..and prime up */
				}
				nptr->expanding = FALSE;
			}
		} else if (cch == '0') {       /* beware o' them hex constants */
			*pptr++ = cch;
			gch (KEEPSP);
			if (cch == 'x' || cch == 'X') {
				do {
					*pptr++ = cch;
					gch (KEEPSP);
				} while (isxdigit(cch));
			}
		} else {
			*pptr++ = cch;
			gch (KEEPSP);
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
