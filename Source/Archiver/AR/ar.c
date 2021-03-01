/*
** file archive utility loosely modeled after Kernigan & Plauger
*/

#include <stdio.h>
#include <ctype.h>
#include "ar.h"


FN    *fnhead = (FN *)NULL;
char  *hid = HID,
      *suf = SUF;
char  *mod,                               /* pointer to module name */
      *archfile;                            /* name of archive file */
int   all = FALSE,                   /* true to access old versions */
      supflag = FALSE,               /* true to supress compression */
      zflag = FALSE;               /* true if names come from stdin */

char  *emalloc();


main(argc, argv)
int   argc;
char  **argv;
   {
   char  command, *p;
   int   n, updating;
   FILE  *afp;

   pflinit();
   mod = *argv++;                              /* save program name */

   if ((argc < 3) || (*(p = *argv++) != '-'))
      help();

   command = tolower(*++p);

   proc_opt(++p);                      /* process command modifiers */

   archfile = *argv++;
   n = strlen(archfile);
   if ((strcmp(archfile + n - SUFSIZ, suf)) != 0)
      archfile = strcat(strcpy(emalloc(n + SUFSIZ + 1), archfile), suf);

   if (get_names(argc -= 3, argv, command == 'u') == 0)
      if (command == 'u')
         fatal(0, "none of the targets found\n");

   if (command == 'u')
      {
      if ((afp = fopen(archfile, "r+")) == NULL)   /* try old first */
         afp = fopen(archfile, "w+");      /* not there - create it */
      }
   else
      {
      if ((afp = fopen(archfile, "r")) == NULL)   /* save file date */
         fatal(errno, "can't find %s\n", archfile);
      }

   proc_cmd(command, afp);                     /* process a command */
   }
/*page*/
/*
** process command modifiers
*/

proc_opt(p)
char   *p;
   {
   while (*p)
      switch (tolower(*p++))
         {
         case 'a' :                          /* all - even old ones */
            all = TRUE;
            break;

         case 's' :                          /* supress compression */
            supflag = TRUE;
            break;

         case 'z' :                         /* get names from stdin */
            zflag = TRUE;
            break;

         default :
            help();
         }
   }


/*
** process the command
*/

proc_cmd(command, afp)
char   command;
FILE  *afp;
   {
   switch (command)
      {
      case 'd' :
         break;

      case 't' :                               /* table of contents */
         table(afp);
         break;

      case 'u' :                                   /* update or add */
         update(afp);
         break;

      case 'p' :                                 /* print member(s) */
         extract(afp, 0);
         break;

      case 'x' :                               /* extract member(s) */
         extract(afp, 1);
         break;

      default  :
         help();
      }
   }
/*page*/
/*
** extract file(s) from the archive
**  copy a file from archive and restore it's origional attrs
*/

extract(afp, flag)
FILE  *afp;
int   flag;                     /* 0 = listing, 1 = writing to file */
   {
   FILE     *ofp;                            /* assume just listing */
   HEADER   header;
   FN       *fnp;
   FILE     *spl_open();

   if (fnhead == (FN *) NULL)
      stash_name("*");                     /* fake for special case */

   while ((get_header(afp, &header)) != EOF)
      {
      for (fnp = fnhead; fnp; fnp = fnp->fn_link)
         if (patmatch(fnp->fn_name, header.a_name, TRUE) == TRUE
            || (header.a_stat != 0 && all == TRUE))
            break;
      if (fnp == 0)
         fseek(afp, header.a_size, 1);            /* file not found */
      else
         {
         if (!flag)
            copy_from(afp, stdout, &header);
         else
				{
				printf("extracting <%s>\n", header.a_name);
            ofp = spl_open(&header);
            copy_from(afp, ofp, &header);
            set_fstat(ofp->_fd, &header.a_attr);
            fclose(ofp);
            }
         }
      }
   }
/*page*/
/*
** list a table of contents for the archive file
**  only show files matching the search mask which are current
**  unless the all flag is set, whereupon we will show old ones too
*/

table(fp)
FILE  *fp;
   {
   HEADER   header;
   FN       *fnp;
   long     n, c4tol();
   static char *attrs[8]={"---","--r","-w-","-wr","e--","e-r","ew-","ewr"};
   

   if (fnhead == (FN *) NULL)
      stash_name("*");                     /* fake for special case */

   printf("                                                  file    file   stored\n");
   printf("  file name                   ver    file date    attr    size    size\n");
   printf("----------------------------- --- -------------- ------   -----   -----\n");
   while ((get_header(fp, &header)) != EOF)
      for (fnp = fnhead; fnp; fnp = fnp->fn_link)
         {
         if (patmatch(fnp->fn_name, header.a_name, TRUE) == TRUE
            && (header.a_stat == 0 || all == TRUE))
            printf("%-29s %2d  %02d/%02d/%02d %02d:%02d %s%s %7ld %7ld\n",
               header.a_name, header.a_stat, header.a_attr.fd_date[0],
               header.a_attr.fd_date[1], header.a_attr.fd_date[2],
               header.a_attr.fd_date[3], header.a_attr.fd_date[4],
               attrs[(header.a_attr.fd_attr>>3) & 7],attrs[header.a_attr.fd_attr & 7],
               c4tol(header.a_attr.fd_fsize), header.a_size);
         fseek(fp, header.a_size, 1);
         }
   }
/*page*/
/*
** add new files or replace existing files
*/

update(afp)
FILE  *afp;
   {
   FILE     *ifp;
   HEADER   header;
   FN       *fnp;
   int      synch, n;
   long     bytes, head_pos, tail_pos, copy_to(), c4tol();

   while ((get_header(afp, &header)) != EOF)
      {
      for (fnp = fnhead; fnp; fnp = fnp->fn_link)
         if ((n = patmatch(fnp->fn_name, header.a_name, TRUE)) == TRUE)
            {
            ++header.a_stat;                       /* mark it older */
            fseek(afp, (long) -sizeof(HEADER), 1);
            if ((fwrite(&header, sizeof(HEADER), 1, afp)) == NULL)
               fatal(errno, "write failure on delete\n");
            }
      fseek(afp, header.a_size, 1);
      }

   for (fnp = fnhead; fnp; fnp = fnp->fn_link)
      {
      if ((ifp = fopen(fnp->fn_name, "r")) == NULL)
			if (errno == 214)
				continue;                    /* a directory, we presume */
			else
            fatal(errno, "can't find %s\n", fnp->fn_name);
      printf("archiving <%s>\n", fnp->fn_name);
      if (supflag || ((synch = getw(ifp)) == MSYNC))
         header.a_type = PLAIN;
      else
         header.a_type = COMP1;
      strcpy(header.a_hid, hid);
      setmem(header.a_name, FNSIZ + 1, ' ');
      strcpy(header.a_name, fnp->fn_name);
      get_fstat(ifp->_fd, &header.a_attr);
      header.a_stat = '\0';
      rewind(ifp);
      head_pos = ftell(afp);                     /* save for update */
      fwrite(&header, sizeof(HEADER), 1, afp);        /* skip ahead */
      bytes = head_pos + c4tol(header.a_attr.fd_fsize) + sizeof(HEADER);
      set_fsize(afp->_fd, bytes);
      header.a_size = copy_to(afp, ifp, &header);
      tail_pos = ftell(afp);
      fclose(ifp);
      fseek(afp, head_pos, 0);             /* back up to header pos */
      if ((fwrite(&header, sizeof(HEADER), 1, afp)) == NULL)
         fatal(errno, "write error on header for %s\n", fnp->fn_name);
      fseek(afp, tail_pos, 0);                 /* go to end of file */
      }
   set_fsize(afp->_fd, tail_pos);         /* now set real file size */
   }
/*page*/
/*
** gather file names from command line or std in
**  use linked list to avoid finite limit on number of names
*/

get_names(ac, av, updating)
int   ac;
char  **av;
int   updating;                        /* TRUE if command is update */
   {
   char     *p, *q, *r, buf[80], *strhcpy();
   int      dirfd, found = 0;
   DIRENT   *dirp, *nextdir();

   while (ac--)
      if (!updating || !(index(*av, '?') || index(*av, '*')))
         found += stash_name(*av++);
      else
         {
         *(p = buf) = '\0';
         if (q = rindex((r = *av++), '/'))
            {
            strcpy(p, r);                     /* copy all to buffer */
            *(r = rindex(p, '/')) = '\0';           /* break in two */
            ++q;                              /* pointer to pattern */
            }
         else
            {
            q = r;
            r = p;                                 /* swap pointers */
            }
         dirfd = opndir(*p ? p : ".");
         if (*p)
            *r++ = '/';                        /* set up for append */
         while ((dirp = nextdir(dirfd)) != -1)
            if (patmatch(q, strhcpy(r, dirp->dir_name), TRUE))
               if ((strucmp(p, archfile)) != 0)         /* not self */
                  found += stash_name(p);
         }
   if (zflag)
      while (gets(buf))
			if (buf[0] != '\0')
            found += stash_name(buf);
   return (found);
   }


/*page*/
/*
** squirrel a name away in the linked list of targets
*/

stash_name(p)
char  *p;
   {
   static   FN    *fnp;
   char           *q;

   if (*p == '/')
      fatal(1, "absolute path illegal <%s>\n", p);
   q = emalloc(sizeof(FN) + strlen(p));
   if (fnhead == (FN *)NULL)
      fnhead = fnp = q;
   else
      fnp = fnp->fn_link = q;
   strcpy(fnp->fn_name, p);
   fnp->fn_link = (FN *)NULL;
   return (1);                                 /* we saved one name */
   }
/*page*/
/*
** get the next header from the file, perhaps skipping the body
*/

get_header(fp, hp)
FILE     *fp;
HEADER   *hp;
   {
   if ((fread(hp, sizeof(HEADER), 1, fp)) == NULL)
      return (EOF);
   if (strncmp(hp->a_hid, hid, HIDSIZ) != 0)
    if (hp->a_hid[0] && hp->a_hid[0]!=26) /* check for xmodem padding */
     fatal(1, "file not archive or damaged\n");
    else
     return (EOF); /* if it is padding, return EOF */
   return (0);
   }


/*
** here we will recreate a tree that was collapsed into the archive file
*/

FILE  *
spl_open(hp)
HEADER   *hp;
   {
   char  buf[FNSIZ+3];
   FILE  *ofp;
   char  *p;
   long  c4tol();

   p = hp->a_name;
   while (p = index(p, '/'))
      {
      *p = '\0';                            /* truncate temporarily */
      if (access(hp->a_name, DIRCHK) == -1)
         if (mknod(hp->a_name, DIRMAK) == -1)     /* try to make it */
            fatal(errno, "can't make <%s>\n", hp->a_name);
      *p++ = '/';                             /* put back the delim */
      }

   strcpy(buf, hp->a_name);
   if (hp->a_stat)
      sprintf(&buf[strlen(buf)], ".%d", hp->a_stat); /* make  unique */
   if ((ofp = fopen(buf, "w")) == NULL)
      fatal(errno, "create failure on %s\n", buf);
   set_fsize(ofp->_fd, c4tol(hp->a_attr.fd_fsize));
   return (ofp);
   }
/*page*/
/*
** copy an archived file from an archive
*/

copy_from(ifp, ofp, hp)
FILE     *ifp, *ofp;
HEADER   *hp;
   {
   long  bytes = hp->a_size;
   int   byt;

   switch (hp->a_type)
      {
      case PLAIN :
         while (bytes--)
            {
            if ((byt = getc(ifp)) == ERROR)
               fatal(errno, "read error while extracting\n");
            if (putc(byt, ofp) == ERROR)
               fatal(errno, "write error while extracting\n");
            }
         break;

      case COMP1 :
         if ((byt = de_LZ_1(ifp, ofp, bytes)) == -2)
            fatal(1, "archive damaged or file not compressed\n");
         break;

      default :
         fatal(1, "unknown compression algorithm\n");
      }
   }
/*page*/
/*
** copy an file to an archive
*/

long
copy_to(ofp, ifp, hp)
FILE     *ofp, *ifp;
HEADER   *hp;
   {
   long  bytes = 0;
   int   byt;

   switch (hp->a_type)
      {
      case PLAIN :
         while ((byt = getc(ifp)) != ERROR)
            if (putc(byt, ofp) == ERROR)
               fatal(errno, "write error while archiving\n");
            else
               ++bytes;
         if (ferror(ifp))
            fatal(errno, "read error while archiving\n");
         break;

      case COMP1 :
         if ((byt = LZ_1(ifp, ofp, &bytes)) == -3)
            fatal(1, "string table overflow on compression\n");
         break;

      default :
         fatal(1, "unknown compression algorithm\n");
      }
   return (bytes);
   }
/*page*/
/*
** get memory from the system or die trying
*/

char  *
emalloc(n)
int   n;
   {
   char  *p;

   if ((p = malloc(n)) == NULL)
      fatal(errno, "Can't allocate memory\n");
   return (p);
   }


/*
** help for the user
*/

help()
   {
   fprintf(stderr, "Ar V1.3 - archive file manager\n");
   fprintf(stderr, "Usage:  Ar -<cmd>[<modifier>] [file .. ]\n");
   fprintf(stderr, "      <cmd> is one of the following:\n");
   fprintf(stderr, "         t  show table of contents for archive\n");
   fprintf(stderr, "         u  update/add file(s) to the archive\n");
   fprintf(stderr, "         p  print file(s) from the archive\n");
   fprintf(stderr, "         x  extract file(s) from the archive\n");
   fprintf(stderr, "      <modifier> is one of the following:\n");
   fprintf(stderr, "         a  all versions (for extract)\n");
   fprintf(stderr, "         s  supress file compression\n");
   fprintf(stderr, "         z  read names for <cmd> from std in\n");
   fprintf(stderr, "\n      File names can include the meta chars ");
   fprintf(stderr, "* and ?, or path lists.\n");
   exit (1);
   }


/*
** print a fatal error message and exit
*/

fatal(code, msg, arg1, arg2)
int   code;
char  *msg;
int   arg1, arg2;
   {
   fprintf(stderr, "%s: ", mod);
   fprintf(stderr, msg, arg1, arg2);
   exit (code);
   }

