#include <stdio.h>
#include <ctype.h>
#define TRUE   1
#define FALSE  0
#define opt(X) flags[X - 'a']
char *myname;
char flags[26];
/*
cc2 - Level II C Compiler Executive
   (c) Copyright 1987 Rick Adams
      Color Central Software
           712 Brett Ave
       Rohnert Park, CA 94928
This program is copyrighted; it is NOT in the public domain.
But you may distribute it, upload it, download it, etc, etc,
as long as it's FREE.
*/
main(argc, argv)
int argc;
char *argv[];
{
     short     nsources, nlibs, ndefs, i;
     char      *p, *p2, *basename(), *index();
     char      *sourcefiles[40], *libfiles[40], *defsymbs[40];
     char      *edition, *memreq, outfile[40], outname[40];
     char      ext, option;
     static char buffer[300];
     printf("* cc2 - (c) Copyright 1987 Rick Adams\n");
     myname = argv[0];
     memreq = edition = "\0";
     nsources = nlibs = ndefs = 0;
     outfile[0] = outname[0] = '\0';
     for (i = 0; i < 26; i++)
          flags[i] = 0;
     while (--argc > 0)
          if (*(p = *++argv) == '-') {
               while (option = tolower(*++p)) {
                    if (index("acdeflmnopqrsxz", option) != NULL) {
                         opt(option) = TRUE;
                         if (option == 'f') {
                              if ((p = index(p, '=')) != NULL)
                                   strcpy(outfile, p + 1);
                              else
                                   error("missing -f filename");
                              break;
                         }
                         else if (option == 'n') {
                              if ((p = index(p, '=')) != NULL)
                                   strcpy(outname, p + 1);
                              else
                                   error("missing -n filename");
                              break;
                         }
                         else if (option == 'm') {
                              if ((p = index(p, '=')) != NULL)
                                   memreq = p + 1;
                              else
                                   error("missing -m argument");
                              break;
                         }
                         else if (option == 'l') {
                              if ((p = index(p, '=')) != NULL)
                                   libfiles[nlibs++] = p + 1;
                              else
                                   error("missing -l filename");
                              break;
                         }
                         else if (option == 'e') {
                              if ((p = index(p, '=')) != NULL)
                                   edition = p + 1;
                              else
                                   error("missing -e argument");
                              break;
                         }
                         else if (option == 'd') {
                              defsymbs[ndefs++] = p + 1;
                              break;
                         }
                    }
                    else
                         error("illegal option");
               }
          }
     else
          sourcefiles[nsources++] = p;
     p = buffer;
     if (opt('z'))
          while(gets(p) != NULL) {
               sourcefiles[nsources++] = p;
               p += ((strlen(p)) + 1);
/*             while (strlen(p) != 0) {
                    sourcefiles[nsources++] = p;
                    if ((p2 = index(p, ' ')) != NULL) {
                         *p2 = '\0';
                         p = p2 + 1;
                    }
                    else
                         p += strlen(p);
               }
*/
          }
     if (nsources == 0)
          error("no source files");
     if (strlen(outfile) == 0)
          strcpy(outfile, basename(sourcefiles[0]));
     if (strlen(outname) == 0)
          strcpy(outname, basename(sourcefiles[0]));
     for (i = 0; i < nsources; i++) {
          ext = extension(sourcefiles[i]);
          if (!opt('q')) {
               docmd("ECHO '%s'\n", sourcefiles[i]);
          }
          switch (ext) {
          case 'c':
               cprep(sourcefiles[i], defsymbs, ndefs, edition);
               if (opt('a') == 0) {
                    casm(sourcefiles[i]);
                    docmd("DEL %s.a\n", basename(sourcefiles[i]));
               }
               break;
          case 'a':
               casm(sourcefiles[i]);
               break;
          case 'r':
               break;
          default:
               error("illegal source file extension");
          }
     }
     if (!opt('a') && !opt('r')) {
          clink(sourcefiles, nsources, libfiles, nlibs, outname,
          outfile, memreq);
          for (i = 0; i < nsources; i++) {
               if (extension(sourcefiles[i]) != 'r')
                    docmd("DEL %s.r\n", basename(sourcefiles[i]));
          }
     }
     exit(0);
}
error(mesg)
char *mesg;
{
     printf("%s : %s\n", myname, mesg);
     exit(0);
}
char *basename(file)
char *file;
{
     char *rindex(), *p;
     static char base[40];
     strcpy(base, file);
     if ((p = rindex(base, '.')) != NULL)
          *p = '\0';
     p = base;
     return(p);
}
extension(file)
char *file;
{
     char *p, *rindex();
     if ((p = rindex(file, '.')) == NULL)
          error("no extension on source file(s)");
     else
          return(*(p + 1));
}
cprep(file, defsymbs, ndefs, edition)
char *file;
char *defsymbs[];
short ndefs;
char *edition;
{
     char *basename();
     char *optim;
     char defs[80];
     char passopts[20];
     short i;
     strcpy(defs, " ");
     for (i = 0; i < ndefs; i++) {
          strcat(defs, "-D");
          strcat(defs, defsymbs[i]);
          strcat(defs, " ");
     }
     if (!opt('o'))
          optim = "! C.OPT ";
     else
          optim = "";
     passopts[0] = '\0';
     if (opt('p'))
          strcat(passopts, "-p ");
     if (opt('s'))
          strcat(passopts, "-s ");
     if (opt('c'))
          strcat(defs, "-l ");
     if (opt('e')) {
          strcat(defs, "-E=");
          strcat(defs, edition);
          strcat(defs, " ");
     }
     docmd("C.PREP%s%s ! C.COMP %s%s> %s.a\n",
     defs, file, passopts, optim, basename(file));
}
casm(file)
char *file;
{
     char *basename();
     docmd("C.ASM %s.a -o=%s.r\n", basename(file), basename(file));
}
clink(sourcefiles, nsources, libfiles, nlibs, name, output, memreq)
char *name, *output, *sourcefiles[], *libfiles[];
char *memreq;
short nsources, nlibs;
{
     static char namopt[20], memopt[20], sources[200];
     static char libs[200], temp[200];
     char  *basename();
     short  i;
     sources[0] = '\0';
     for (i = 0; i < nsources; i++) {
          strcat(sources, " ");
          strcat(sources, basename(sourcefiles[i]));
          strcat(sources, ".r");
     }
     strcpy(libs, "");
     for (i = 0; i < nlibs; i++) {
          strcat(libs, " ");
          strcat(libs, "-l=");
          strcat(libs, libfiles[i]);
     }
     strcat(libs, " -l=/dd/lib/clib.l");
     if (opt('m'))
          sprintf(memopt, " -M=%s", memreq);
     else
          memopt[0] = '\0';
     if (opt('n'))
          sprintf(namopt, " -n=%s", name);
     else
          namopt[0] = '\0';
     docmd("C.LINK /dd/lib/cstart.r%s %s -o=%s%s%s\n",
     sources, libs, output, namopt, memopt);
}
docmd(s1, s2, s3, s4, s5, s6, s7)
char *s1, *s2, *s3, *s4, *s5, *s6, *s7;
{
     short status;
     static char cmd[300];
     sprintf(cmd, s1, s2, s3, s4, s5, s6, s7);
     if (opt('x'))
          printf("%s", cmd);
     else {
          if (os9fork("shell", strlen(cmd), cmd, 1, 1, 8) == -1)
               error("cannot spawn process");
          wait(&status);
          if (status)
               exit(0);
     }
}