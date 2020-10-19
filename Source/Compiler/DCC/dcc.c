/*
	Driver for the new C compiler

	Based on 'cc', by Carl Kreider and others

	See 'git log' for changes.
*/
#include "dcc.h"
#include <module.h>

cleanup ()
{
	if (childid)
		kill (childid, 2);

	if (newopath) {
		close (1);
		dup (newopath);
	}

	if (thisfilp)
		unlink (thisfilp);

	if (lasfilp)
		unlink (lasfilp);
}


trap (code)
int code;
{
	cleanup ();
	exit (code);
}


main (argc, argv)
int argc;
char **argv;
{
	char          **emp;
	int           suffix, j, m, deltmpflg;
	register char *p;

	j = 0;
	while ((--argc > 0) && (++j < 100)) {
		if (*(p = *++argv) == '-') {
			while (*++p) {
				switch (*p) {
					case 'a':                      /* stop at asm (no .r) (D) */
						aflag = TRUE;
						break;

					case 'b':                       /* use different "cstart" */
						bflag = TRUE;
						if (*(p + 1) != '=')
							break;
						strcpy (mainline, (p + 2));
						goto saver;

					case 'c':                         /* include comments (C) */
						cflag = TRUE;
						break;

					case 'd':                 /* make identifier (define) (C) */
					case 'D':
						if (*(p + 1) == '\0')
							goto saver;
						*--p = '-';
						*(p + 1) = 'D';
						macarray[maccnt++] = p;
						goto saver;

					case 'e':                           /* set edition number */
						emp = &edition;
						goto emcommon;

					case 'f':                         /* set outfile path (L) */
						if (*++p != '=')
							goto saver;
						strcpy (objname, (p + 1));
						if (objname[0] == '\0')
							goto saver;
						++fflag;
						suffix = findsuff (objname);
						if (suffix == 'c')
							error ("Suffix '.%c' not allowed for output", suffix);
						goto saver;

					case 'l':                        /* specify a library (L) */
						if (libcnt == 4) {
							error ("Too many libraries");
						} else if (*(p + 1) != 'g') {            /* want cgfx */
							lgflg++;
							goto saver;
						} else if (*(p + 1) == 'l') {          /* want lexlib */
							llflg++;
							goto saver;
						} else if (*(p + 1) != 's') {           /* want sys.l */
							lsflg++;
							goto saver;
						} else if (*(p + 1) != '=') {
							goto saver;
						}
						*--p = '-';
						libarray[libcnt++] = p;
						goto saver;

					case 'm':                              /* set memory size */
						emp = &xtramem;
						*p &= 0x5f;
					emcommon:
						if ((*(p + 1))) {
							*--p = '-';
							*emp = p;
						}
						goto saver;

					case 'M':                  /* ask linker for link map (L) */
						mflag = TRUE;
						break;

					case 'n':                       /* give module a name (L) */
						*--p = '-';
						modname = p;
						goto saver;

					case 'o':                             /* no optimizer (O) */
						oflag = FALSE;
						break;

					case 'O':                      /* stop after optimization */
						o2flg = TRUE;
						break;

					case 'P':
						p2flg = TRUE;                /* fall to set pflag too */
					case 'p':                             /* add profiler (C) */
						pflag = TRUE;
						break;

					case 'q':                               /* use quiet mode */
						qflag = TRUE;
						freopen ("c.errors", "w", stderr);
						break;

					case 'r':                         /* stop at .r (no link) */
						rflag = TRUE;
						break;

					case 's':                        /* no stack checking (C) */
						sflag = TRUE;
						break;

					case 'S':              /* ask linker for symbol table (L) */
						s2flg = TRUE;
						break;

					case 't':                    /* use transendental library */
						tflag = TRUE;
						break;

					case 'T':                  /* use alternate (or NO) tmpdir*/
						if (*(p + 1) != '=') {
							tmpdir = FALSE;
							break;
						} else {
							strcpy (tmpname, (p + 2));
							strcat (tmpname, "/");
						}
						goto saver;

					case 'V':                          /* give version number */
						hello = TRUE;
						break;

					case 'w':            /* waste the compile for error check */
						nullflag = TRUE;
						break;

					case 'x':            /* use the work dir for main library */
						xflag = TRUE;
						break;

					case 'z':
						zflag = TRUE;
						break;

					default:
						error ("unknown flag : -%c\n", *p);
				}	/* end of switch */
			}	/* end of inner while */
saver:
			continue;
		}   /* end of if */
		else {
			switch ((suffix = findsuff (*argv))) {
				case 'r':
					rsufflg = 1;
				case 'a':
				case 'c':
				case 'o':
					suffarray[filcnt] = suffix;
					namarray[filcnt] = *argv;
					++filcnt;
					break;

				default:
					error ("%s : no recognised suffix", *argv);
			}	/* end of switch */
		}	/* end of else */
	}	/* end of outer while */

	/* command line now parsed, start real work */
	logo ();                                            /* identify ourselves */
	if (filcnt == 0) {
		fprintf (stderr, "no files!\n");
		exit (0);
	}

	if ((aflag && rflag))
		error ("incompatible flags");

	if (fflag && filcnt > 1 && (aflag || rflag))
		error ("%s : output name not applicable", objname);

	if (!fflag)
		strcpy (objname, ((filcnt == 1) ? namarray[0] : "output"));


	if (tmpdir && !(*tmpname)) {
		for (loopcnt = 0; loopcnt < sizeof (tmproot) / sizeof (char *); loopcnt++) {
			if (access (tmproot[loopcnt], 0x83) == 0) {
				strcpy (tmpname, tmproot[loopcnt]);
				strcat (tmpname, "/");
				break;
			}
		}
	}

	strcat (tmpname, tmptail);

	mktemp (tmpname);
	strcat (tmpname, ".m");	/* add a suffix for chgsuff */

	intercept (trap);
	dummy ();

	for (j = 0; j < filcnt; ++j) {               /* for each file on cmd line */
		fprintf (stderr, "'%s'\n", namarray[j]);

		if (suffarray[j] == 'c') {
			deltmpflg = 1;                /* is C prog so prep and compile it */
			strcpy (destfile, tmpname);
			chgsuff (destfile, 'm');
			frkprmp = parmbuf;

			if (cflag)	/* we want comments */
				splcat ("-l");

			if (edition)	/* explicit edition number */
				splcat (edition);

			for (m = 0; m < maccnt;) {
				splcat (macarray[m++]);                  /* tack on "defines" */
			}
			splcat (namarray[j]);                    /* and now the file name */
			newopath = dup (1);
			close (1);
			if ((creat (destfile, 3)) != 1)
				error ("can't create temporary file for '%s'", namarray[j]);

			trmcat ();
			thisfilp = 0;
			lasfilp = destfile;
			runit (PREPROC, 1);
			close (1);
			dup (newopath);
			close (newopath);
			newopath = 0;

			/* now compile it */
			strcpy (srcfile, destfile);
			frkprmp = parmbuf;
			thisfilp = srcfile;
			splcat (srcfile);
			if (sflag)
				splcat ("-s");          /* no stack checking */

			if (nullflag) {
				splcat ("-n");          /* waste the output */
				strcpy (destfile, "/nil");
				deltmpflg = 0;
			} else {
				if (aflag) {
					if (fflag) {
						strcpy (destfile, objname);     /* explicit obj name */
					} else {
						strcpy (destfile, namarray[j]);
						chgsuff (destfile, 'a');
					}
				} else {
					chgsuff (destfile, 'i');
				}
			}

			strcpy (ofn, "-o=");
			strcat (ofn, destfile);
			splcat (ofn);
			if (pflag)
				splcat ("-p");          /* wants profiler code */
			trmcat ();
			lasfilp = destfile;
			runit (COMPILER, 0);
			unlink (srcfile);
		} else {
			deltmpflg = 0;
		}

		/* now assemble and perhaps optimize it */
		if (aflag || nullflag || (suffarray[j] == 'r')) {
			lasfilp = 0;                /* is .r so no work to do */
		} else {
			/* don't optimize ".a" or ".o" files */
			if ((suffarray[j] == 'a') || (suffarray[j] == 'o')) {
				strcpy (srcfile, namarray[j]);
				thisfilp = 0;
			} else {
				strcpy (srcfile, destfile);
				thisfilp = srcfile;
				if (oflag) {
					frkprmp = parmbuf;  /* yes,  optimize it */
					splcat (srcfile);
					if ((filcnt == 1) && !o2flg) {
						strcpy (destfile, tmpname);
						chgsuff (destfile, 'm');
					} else {
						if (fflag && o2flg) {
							strcpy (destfile, objname);
						} else {
							chgsuff (namarray[j], 'o');
							strcpy (destfile, namarray[j]);
						}
					}
					splcat (destfile);
					trmcat ();
					lasfilp = destfile;
					runit (OPTIMIZER, 0);
					if (deltmpflg)
						unlink (srcfile);
					strcpy (srcfile, destfile);
					thisfilp = srcfile;
				}
			}
			if (!o2flg) {
				frkprmp = parmbuf;
				splcat (srcfile);
				if ((filcnt == 1) && !rflag) {
					strcpy (destfile, tmpname);
					chgsuff (destfile, 'r');
				} else {
					if (fflag && rflag) {
						strcpy (destfile, objname);
					} else {
						chgsuff (namarray[j], 'r');
						strcpy (destfile, namarray[j]);
					}
				}
				strcpy (ofn, "-o=");
				strcat (ofn, destfile);
				splcat (ofn);
				trmcat ();
				lasfilp = destfile;
				runit (ASSEMBLER, 0);
				if (deltmpflg)
					unlink (srcfile);
			}
		}
	}   /* end of for each file */

	if (nullflag || aflag || rflag || o2flg)
		exit (0);

	/* now link all together*/
	frkprmp = parmbuf;
	if ((p = chkccdev ()) == 0)
		error ("Cannot find default system drive");
	if (bflag) {
		strcpy (ofn, mainline);         /* use cstart.r or whatever */
	} else {
		strcat (strcat (strcpy (ofn, p), "/lib/"), mainline);   /* global */
	}
	splcat (ofn);
	if ((filcnt == 1) && (suffarray[0] != 'r')) {
		splcat (thisfilp = destfile);
	} else {
		for (thisfilp = 0, j = 0; j < filcnt; ++j) {
			splcat (namarray[j]);
		}
	}
	strcpy (ofn, "-o=");
	if (fflag == 0)
		chgsuff (objname, 0);
	strcat (ofn, objname);
	splcat (ofn);
	for (j = 0; j < libcnt; j++) {
		splcat (libarray[j]);
	}

	if (lgflg) {
		strcat (strcat (strcpy (ofn, "-l="), p), "/lib/cgfx.l");
		splcat (ofn);
	}

	if (llflg) {
		strcat (strcat (strcpy (ofn, "-l="), p), "/lib/lexlib.l");
		splcat (ofn);
	}

	if (lsflg) {
		strcat (strcat (strcpy (ofn, "-l="), p), "/lib/sys.l");
		splcat (ofn);
	}

	if (p2flg) {
		strcat (strcat (strcpy (ofn, "-l="), p), "/lib/dbg.l");
		splcat (ofn);
	}

	strcpy (ofn, "-l=");
	if (xflag == 0)
		strcat (strcat (ofn, p), "/lib/");
	strcat (ofn, (tflag) ? "clibt.l" : "clib.l");
	splcat (ofn);
	if (modname)
		splcat (modname);
	if (xtramem)
		splcat (xtramem);
	if (edition)
		splcat (edition);
	if (mflag)
		splcat ("-m");
	if (s2flg)
		splcat ("-s");
	trmcat ();
	lasfilp = 0;
	runit (LINKER, 0);
	cleanup ();
}


runit (cmd, code)
char  *cmd;
int code;
{
/*	fprintf (stderr, "   %-6s:\n", cmd); */
	fprintf (stderr, "   %-6s:  %s", cmd, parmbuf);
	if (zflag)
		return;
	if ((childid = os9fork (cmd, frkprmsiz, parmbuf, 1, 1, 0)) < 0)
		error ("cannot execute %s", cmd);
	wait (&childstat);
	childid = 0;
	if (childstat > code)
		trap (childstat);
}


chkccdev ()
{
	char          *s, c;
	register char *p;
	mod_data      *q;
	mod_config    *r;

	if ((q = modlink ("ccdevice", 4, 0)) != -1) {
		strcpy (devnam1, (char *) q + q->m_data);
		munlink (q);
		return (devnam1);
	} else {
		if ((r = modlink ("Init", 0x0c, 0)) != -1) {
			s = (char *) r + r->m_sysdrive;
			p = devnam1;
			while ((c = *s++) > 0) {
				*p++ = c;
			}
			*p++ = (c & 0x7f);
			*p = 0;
			munlink (r);
			return (devnam1);
		}
	}
	return (0);
}


error (format, arg)
char  *format, *arg;
{
	hello = TRUE;
	logo ();                            /* print logo if not done yet */
	fprintf (stderr, format, arg);
	putc ('\n', stderr);
	trap (0);
}


chgsuff (s, c)
char  *s, c;
{
	register char *p = s;

	while (*p++) {
		;
	}
	if (*(p - 3) != '.')
		return;
	if (c == '\0') {
		*(p - 3) = 0;
	} else {
		*(p - 2) = c;
	}
}



findsuff (p)
register char *p;
{
	int  j;
	char c;

	j = 0;
	while (c = *p++) {
		if (c == '/') {
			j = 0;
		} else {
			j++;
		}
	}
	if ((j <= 29) && (j > 2) && (*(p - 3) == '.')) {
		return (*(p - 2) | 0x40);
	} else {
		return (0);
	}
}


splcat (s)
char  *s;
{
	register char *p = s;

	*frkprmp++ = 0x20;
	while (*frkprmp++ = *p++) {
		;
	}
	--frkprmp;
}


trmcat ()
{
	*frkprmp++ = '\n';
	*frkprmp = '\0';
	frkprmsiz = frkprmp - parmbuf;
}


dummy ()
{
}


logo ()
{
	if (hello)
		fprintf (stderr, "DCC %d.%d.%d\n", VERSION, REVISION, PATCHLVL);
}
