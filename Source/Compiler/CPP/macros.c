/*
	$Id$
*/
#include "prep.h"
#include <string.h>

macdef *
revlist(p)
register macdef	*p;
{
	register macdef	*prev,*next;

	prev = NULL;
	while (p) {
		next = p->next;		/* the next is the next... */
		p->next = prev;		/* the last is the preeve... */
		prev = p;			/* after the next swap... */
		p = next;
	}
	return prev;			/* ..it's time to leave!!! */
}


macro *
addmac(name,string)
register char	*name,*string;
{
	register int n;
	register macro *mptr,*nptr;
	register char *cptr;
	register macdef *mlist;
	register int argc;
	auto char *argv[MAXARGS];
	auto char argname[NAMESIZE];
	auto char dummyargs[LINESIZE];

	/* insert entry in macro name hash table */
	nptr = (macro *) grab(sizeof(macro));
	mptr = mactab[n = hash(name)];
	mactab[n] = nptr;
	nptr->next = mptr;

	/* save macro name */
	/*    strcpy(nptr->macname,name); */
	nptr->macname = savestr(name);
	nptr->expanding = FALSE;	/* we are not now expanding this macro! */

	/* save pre-defined string (no args possible) */
	if (string) {
#ifdef DEBUG
		if (dflag) fprintf(stderr, "new macro: %s=%s\n", name, string);
#endif
		(nptr->macdef = (macdef *)grab(sizeof(macdef)))->md_elem = savestr(string);
		nptr->macargs = -1;
		return nptr;
	}

	/* look for possible dummy arg list */
	argc = -1;
	if (cch == '(') {
		gch(KEEPSP);
		cptr = dummyargs;
		argc = 0;
		while (1) {
			nxtch();
			if (cch == '\0' || cch == ')') break;
			argv[argc] = cptr;
			if (isalpha(cch) || cch == '_') {
				register char	*s;

				getword(s=argname,NAMESIZE);
				while(*cptr++ = *s++) ;
				++argc;
				nxtch();
				if(cch == ',')
					gch(KEEPSP);
			} else {
				lerror("macro definition error");
				do gch(SKIPSP);
				while (cch != '\0') ;
				break;
			}
		}
		gch(SKIPSP);
	}
	nxtch();

	/* now plow thru the definition looking for dummy args */
	nptr->macargs = argc;
	mlist = (macdef *) grab(sizeof(macdef));
	mlist->md_type = 0;
	mlist->next = NULL;
	if(cch) {
		cptr = holdbuf;

		while (cch) {
			if ((!kflag) && (cch == '"' || cch == '\'')) {
				char delim = cch;
				*cptr++ = cch; gch(KEEPSP);
				while (cch && cch != delim) {
					*cptr++ = cch; gch(KEEPSP);
					if (cch == '\\') {
						*cptr++ = cch; gch(KEEPSP);
						*cptr++ = cch; gch(KEEPSP);
					}
				}
				if (!cch)
					lerror("missing end of string or character literal");
				*cptr++ = cch; gch(KEEPSP);
				continue;
			} else
tryagain:	if (cch == '#') { /* stringify or concatenation */
				gch(KEEPSP);
				if (cch == '#') { /* concatenation */
					gch(KEEPSP);
					nxtch();	/* eat trailing spaces */
					*cptr = '\0';
					while (isspace(*(cptr-1))) { /* eat leading spaces too */
						*--cptr = '\0';
					}
					continue;
				} else if (argc > 0) {
					if (isspace(cch)) {
						nxtch();
						if (!(isalpha(cch) || cch == '_')) {
							goto badhash;
						} else {
							goto tryagain;
						}
					} else if (isalpha(cch) || cch == '_') { /* stringify */
						register char *s = hold1buf;

						getword(s, LINESIZE);
						for (n = 0; n < argc; ++n) {
							if (strcmp (s, argv[n]) == 0) {
								register macdef *m;

								*cptr = '\0';
								mlist->md_elem = savestr(holdbuf);
								mlist->md_type = 0;
								cptr = holdbuf;

								/*
									new argument node

									the md_type is negative to tell macro
									expansion to safely quote the new string
								*/
								m = (macdef *) grab(sizeof(macdef));
								m->md_type = -(n+1); /* right here */
								m->md_elem = NULL;
								m->next = mlist;
								mlist = m;

								/* new text node */
								m = (macdef *) grab(sizeof(macdef));
								m->md_type = 0;
								m->next = mlist;
								mlist = m;
								break;
							}
						}
						if (n >= argc) { /* we didn't match */
badhash:					*cptr = '\0';
							lerror("# not followed by macro argument");
							continue;
						}
					}
				} else {
					*cptr++ = '#';
					*cptr++ = cch;
					gch(KEEPSP);
					continue;
				}
			} else if (isalpha(cch) || cch == '_') {
				register char *s = hold1buf;

				getword(s,LINESIZE);
				for (n = 0; n < argc; ++n) {
					if (strcmp(s,argv[n]) == 0) {
						register macdef *m;

						/* save the text processed so far */
						*cptr = '\0';
						if (cptr > holdbuf) {
							mlist->md_elem = savestr(holdbuf);
							mlist->md_type = 0;
							cptr = holdbuf;

							/* add a macro arg node to the list */
							m = (macdef *) grab(sizeof(macdef));
							m->next = mlist;
							mlist = m;
						} else
							m = mlist;

						/* save arg number in node */
						m->md_type = n+1;
						m->md_elem = 0;

						/* and another text node */
						m = (macdef *) grab(sizeof(macdef));
						m->next = mlist;
						m->md_type = 0;
						m->md_elem = NULL;
						mlist = m;
						goto breaker;
					}
				}
				while(*cptr++ = *s++)
					;
				cptr--;     /* back-up over the null */
		breaker:;
			} else if (cch == '0') {
				*cptr++ = cch;
				gch(KEEPSP);
				if (cch == 'x' || cch == 'X') {
					do {
						*cptr++ = cch;
						gch(KEEPSP);
					} while(isxdigit(cch));
				}
#ifdef CUTOUT
			} else if (cch == '"' || cch == '\'') {
				*cptr++ = HIDER;
				*cptr++ = cch-16;       /* hide the quotes inside macros */
				gch(KEEPSP);
#endif
			} else {
				*cptr++ = cch;
				gch(KEEPSP);
			}
		}

		/* finally, mark end of string and stash it (safely) */
		*cptr = '\0';
		mlist->md_elem = savestr(holdbuf);

		/* reverse order of macdef list and save the pointer */
		nptr->macdef = revlist(mlist);
	} else {		/* no args, no text */
		mlist->next = NULL;
		mlist->md_elem = "";
		mlist->md_type = 0;
		nptr->macdef = mlist;
/*		nptr->next = NULL; */
	}

#ifdef DEBUG
	if (dflag) {
		macdef	*temp = nptr->macdef;

		fprintf(stderr, "new macro: %s=", name);
		while (temp) {
			if (temp->md_type)
				fprintf(stderr, "<arg%d>", temp->md_type);
			else
				fprintf(stderr, "%s", temp->md_elem);
			temp = temp->next;
		}
		fputs("\n", stderr);
	}
#endif

	return nptr;
}


macro *
findmac(name)
register char	*name;
{
	register macro	*ptr;
	register int	c;

	c = name[NAMESIZE-1];
	name[NAMESIZE-1] = '\0';
	for (ptr = mactab[hash(name)]; ptr; ptr = ptr->next) {
		if(strcmp(name,ptr->macname) == 0) return ptr;
	}
	name[NAMESIZE-1] = c;

	return 0;
}

delmac(name)
char	*name;
{
	register macro	*p;
	char			nameb[NAMESIZE];

	/* forget the macro name */
	strcpy(nameb, name);         /* (because of the way findmac works...) */
	while (p = findmac(nameb)) p->macname[0] = 0;
}


nxtch()
{
	while (isspace(cch))
		gch(KEEPSP);
}
