#include <stdio.h>
#include <ctype.h>
#include <modes.h>


typedef struct {
     char dir_name[29];
     char dir_addr[3];
     } DIRENT;

typedef struct {
     char      fd_att;
     unsigned  fd_own;
     char      fd_date[5];
     char      fd_link;
     long      fd_fsize;
     char      fd_dcr[3];
     } FILDES;


/*
** snatch memory from the system and read in directory,
**  saving only good files.  Then sort into order.
**
** returns the size of block and the end via reference
**  parameters, but only valid if operation was successful.
**
** returns  -1 on memory error
**           0 on open fail (not dir)
**         pnt on success
*/

DIRENT *
readir(s, end)
char     *s;
DIRENT   **end;
   {
   char     *buf;
   FILDES   fdbuf;
   DIRENT   *bufp;
   DIRENT   dirbuf;
   int      mem, n;
   int      fd, j;
   int      count = 0;

   if ((fd = open(s, S_IFDIR + S_IREAD)) == -1)
      return (-1);                                         /* not dir */
   readfd(fd, &fdbuf);           /* 'from' file desc - exits if error */
   mem = (int)(fdbuf.fd_fsize);
   if ((buf = bufp = malloc(mem)) == -1)      /* get mem for name buf */
      exit(_errmsg(0, "out of memory))'

   while ((n = read(fd, &dirbuf, sizeof(dirbuf))) > 0)  /* copy names */
      {
      if ((dirbuf.dir_name[0] & 0x7f) != 0)
         {
         strhcpy(bufp, &dirbuf);
         bufp->dir_name[28] = '\0';                   /* just in case */
         for (j = 0; j < 3; j++)
            bufp->dir_addr[j] = dirbuf.dir_addr[j];
         ++bufp;                                /* point to next spot */
         ++count;
         }
      }
   close(fd);
   qsort(buf, count, sizeof(dirbuf), strucmp);           /* now sort */
   *end = bufp;
   return (buf);
   }
