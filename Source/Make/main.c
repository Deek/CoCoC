/*
 *     make [-f makefile] [-inpqrst] [target(s) ...]
 *
 *     (Better than EON mk but not quite as good as UNIX make)
 *
 *     -f <file> makefile name
 *     -i ignore exit status
 *     -n Pretend to make
 *     -p Print all macros & targets
 *     -q Question up-to-dateness of target.  Return exit status 1 if not
 *     -r Do not use inbuilt rules
 *     -b Do not use built-in rules
 *     -s Make silently
 *     -t Touch files instead of making them
 *     -z <path> Get targets from path.
 *     -d Debug mode.
 *     -u Force update even if not needed.
 * EON only:
 *     -m Change memory requirements
 */

#include <stdio.h>
#include "h.h"

#ifdef unix
#include <sys/errno.h>
#endif
#ifdef eon
#include <sys/err.h>
#endif
#ifdef OS9
#include <errno.h>
#include <ctype.h>
#endif


#ifdef eon
#define MEMSPACE       (16384)
#endif


char *                 myname;
char *                 makefile;       /*  The make file  */
#ifdef eon
unsigned               memspace = MEMSPACE;
#endif

FILE *                 ifd;            /*  Input file desciptor  */
bool                   domake = TRUE;  /*  Go through the motions option  */
bool                   ignore = FALSE; /*  Ignore exit status option  */
bool                   silent = FALSE; /*  Silent option  */
bool                   print = FALSE;  /*  Print debuging information  */
bool                   rules = TRUE;   /*  Use inbuilt rules  */
bool                   dotouch = FALSE;/*  Touch files instead of making  */
bool                   quest = FALSE;  /*  Question up-to-dateness of file  */
bool                   debug = FALSE;  /*  Debug mode */
bool                   force = FALSE;  /*  Force update */


void
main(argc, argv)
int                    argc;
char **                        argv;
{
       register char *         p;              /*  For argument processing  */
       int                     estat = 0;      /*  For question  */
       register struct name *  np;
       bool                    inptargs = FALSE;
       char *                  targetpath;
       char                    nexttarget[256];

#ifdef OS9
       pflinit();
#endif

       myname = (argc-- < 1) ? "make" : *argv++;

       while ((argc > 0) && (**argv == '-'))
       {
               argc--;         /*  One less to process  */
               p = *argv++;    /*  Now processing this one  */

               while (*++p != '\0')
               {
                       switch(*p)
                       {
                       case 'f':       /*  Alternate file name  */
                               if (*(p+1) == '=') ++p;  /* go past '=' */
                               if (*++p == '\0')
                               {
                                       if (argc-- <= 0)
                                               usage();
                                       p = *argv++;
                               }
                               makefile = p;
                               goto end_of_args;
#ifdef eon
                       case 'm':       /*  Change space requirements  */
                               if (*++p == '\0')
                               {
                                       if (argc-- <= 0)
                                               usage();
                                       p = *argv++;
                               }
                               memspace = atoi(p);
                               goto end_of_args;
#endif
                       case 'n':       /*  Pretend mode  */
                               domake = FALSE;
                               break;
                       case 'i':       /*  Ignore fault mode  */
                               ignore = TRUE;
                               break;
                       case 's':       /*  Silent about commands  */
                               silent = TRUE;
                               break;
                       case 'd':
                               debug = TRUE;
                               break;
                       case 'p':
                               print = TRUE;
                               break;
                       case 'b':
                       case 'r':
                               rules = FALSE;
                               break;
                       case 't':
                               dotouch = TRUE;
                               break;
                       case 'q':
                               quest = TRUE;
                               break;
                       case 'u':
                               force = TRUE;
                               break;
                       case 'z':       /*  Get target list from path */
                               inptargs = TRUE;
                               if (*(p+1) == '=') ++p;  /* go past '=' */
                               if (*++p == '\0')
                               {
                                       if (argc-- <= 0)
                                               usage();
                                       p = *argv++;
                               }
                               targetpath = p;
                               goto end_of_args;
                       default:        /*  Wrong option  */
                               usage();
                       }
               }
       end_of_args:;
       }

#ifdef eon
       if (initalloc(memspace) == 0xffff)  /*  Must get memory for alloc  */
               fatal("Cannot initalloc memory");
#endif

#ifdef DEFN0
       if (debug)
               printf("Reading default rules from \"%s\".\n",DEFN0);
       if((ifd=fopen(DEFN0,"r"))==(FILE *)0){
               printf("Error, couldn't open default rules file \"%s\".\n",
                       DEFN0);
       } else {
               input(ifd,DEFN0);
               fclose(ifd);
       }
#else
       if (debug)
               printf("Setting up pre-compiled default rules.\n");
       makerules();
#endif

       setmacro("$", "$");

       if (debug)
               printf("Getting macros from command line.\n");

       while (argc && (p = index(*argv, '=')))
       {
               char            c;

               c = *p;
               *p = '\0';
               setmacro(*argv, p+1);
               *p = c;

               argv++;
               argc--;
       }

       if (debug)
                printf("Opening makefile.\n");

       if (strcmp(makefile, "-") == 0) /*  Can use stdin as makefile  */
               ifd = stdin;
       else
               if (!makefile)          /*  If no file, then use default */
               {
                       ifd = (FILE *) 0;
#ifdef DEFN2
                       /* If a second default name exists, try it first */
                       if ((ifd = fopen(DEFN2, "r")) == (FILE *)0)
                               if (errno != ERRNOTFND)
                                     fatal("Can't open %s; error %02x", DEFN2, errno);
                       else
                               makefile = DEFN2;
#endif
                       if ((ifd == (FILE *)0)
                                 && ((ifd = fopen(DEFN1, "r")) == (FILE *)0))
                               if (!argc && !inptargs)
                                   fatal("Can't open %s", DEFN1);
                               else
                                   fprintf(stderr,"Can't open \"%s\".\n", DEFN1);
                       else
                               makefile = DEFN1;
               }
               else
                       if ((ifd = fopen(makefile, "r")) == (FILE *)0)
                               fatal("Can't open %s", makefile);
       if (debug)
               printf("Reading makefile.\n");

       input(ifd,makefile);     /*  Input all the gunga  */
       fclose(ifd);    /*  Finished with makefile  */

       if (print)
               prt();  /*  Print out structures  */

#ifdef DEBUG
       if (debug)
               printf("Checking for special targets.\n");
#endif

       np = newname(".SILENT");
       if (np->n_flag & N_TARG)
               silent = TRUE;

       np = newname(".IGNORE");
       if (np->n_flag & N_TARG)
               ignore = TRUE;

       precious();

       /* If there is a command-line target, then use that */
       if (!firstname && !argc && !inptargs)
               fatal("No targets defined");

       if (debug)
               printf("Checking target definitions for circles.\n");

       circh();        /*  Check circles in target definitions  */

       if (inptargs) {   /* Get targets from path */
               if (strcmp(targetpath,"-")==0) ifd=stdin;
               else {
                       if (debug)
                               printf("Opening %s for targets.\n",targetpath);
                       if ((ifd=fopen(targetpath,"r"))==(FILE *)0)
                               fatal("Can't open %s.",targetpath);
               }               
               while (fgets(nexttarget,256,ifd)) {
                       register char *a, *b;

                       for (a=nexttarget;isspace(*a);a++)
                                ;
                       for (b=a ; *b ; b++ )
                                if (isspace(*b)) *b = '\0';
                       if (debug) printf("Making target %s.\n",a);
                       estat |= make(newname(a),0);
               }
               fclose(ifd);
       } else if (!argc)   /* Use firstname as implicit target */
               estat = make(firstname, 0);
       else while (argc--) /* Use targets on command line */
       {
               if (!print && !silent && strcmp(*argv, "love") == 0)
                       printf("Not war!\n");
               estat |= make(newname(*argv++), 0);
       }

       if (quest)
               exit(estat);
       else
               exit(0);
}


usage()
{
#ifdef eon
   /* For eon, we list the -m option */
   fprintf(stderr, "Usage: %s [-bdinpqrstum] [-f makefile]",myname);
#else
   fprintf(stderr, "Usage: %s [-bdinpqrstu] [-f makefile]", myname);
#endif
   fprintf(stderr, " [-z <path>] [macro=val] [target(s)]\n");
#ifdef OS9
   /* For OS9, we include more complete help info */
   fprintf(stderr, "Options: -b  Turn off built-ins   -q  Query target\n");
   fprintf(stderr, "         -d  Debug mode           -r  Turn off built-ins\n");
   fprintf(stderr, "         -i  Ignore errors        -s  Make silently\n");
   fprintf(stderr, "         -n  Do not execute cmds  -t  Touch files\n");
   fprintf(stderr, "         -p  Print structures     -u  Force update\n");
   fprintf(stderr, "         -z  <path>   Get targets from <path>\n");
   fprintf(stderr, "         -f  <path>   Use <path> as makefile\n");
#endif
       exit(1);
}


void
fatal(msg, a1, a2, a3, a4, a5, a6)
char   *msg;
{
       fprintf(stderr, "%s: ", myname);
       fprintf(stderr, msg, a1, a2, a3, a4, a5, a6);
       fputc('\n', stderr);
       exit(1);
}