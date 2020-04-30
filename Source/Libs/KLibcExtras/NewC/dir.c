#include <dir.h>
long  lseek();

/*
** routine to close up and tidy up
*/

closedir(dirp)
DIR   *dirp;
   {
   close(dirp->dd_fd);
   free(dirp);
   }


/*
** routine to open a directory and set up the data structures
*/

DIR   *
opendir(name)
char  *name;
   {
   DIR   *dirp = (DIR *) 0;

   if ((dirp = malloc(sizeof(DIR))) != 0)
      if ((dirp->dd_fd = open(name, 0x81)) < 0)
         {
         free(dirp);
         dirp = (DIR *) 0;
         }
   return (dirp);
   }


/*
** routine to return the next directory entry
**  this could be buffered, but I don't want to
**  mess with sorting out the seeks
*/

struct direct *
readdir(dirp)
DIR   *dirp;
   {
   static struct direct de;

   do {
      if (read(dirp->dd_fd, dirp->dd_buf, 32) <= 0)
         return (0);
      } while (dirp->dd_buf[0] == '\0');

   strhcpy(de.d_name, dirp->dd_buf);
   de.d_addr = ((dirp->dd_buf[29] & 0xff) << 8) + (dirp->dd_buf[30] & 0xff);
   de.d_addr = (de.d_addr << 8) + (dirp->dd_buf[31] & 0xff);
   return (&de);
   }
/*page*/
/*
** routine to seek to a specific location
*/

seekdir(dirp, loc)
DIR   *dirp;
long  loc;
   {
   lseek(dirp->dd_fd, loc, 0);
   }


/*
** routine to return the current position in a directory
*/

long
telldir(dirp)
DIR   *dirp;
   {
   return (lseek(dirp->dd_fd, 0L, 1));
   }
