
#include "ar.h"

/*
** open a directory, returning an fd or -1
*/

int
opndir(s)
char  *s;
   {
   return (open(s, 0x81));
   }


/*
** return a pointer to the next directory entry
*/

static   DIRENT   dent;

DIRENT *
nextdir(fd)
int   fd;
   {
   char  c;

   do {
      if (read(fd, &dent, sizeof(dent)) <= 0)
         return (-1);
      } while ((c = dent.dir_name[0]) == 0 || (c & 0x7f) == '.');
   return (&dent);
   }
