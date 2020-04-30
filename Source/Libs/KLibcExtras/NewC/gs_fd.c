
#include <direct.h>

/*
** simulate the 68K _gs_gfd() function
*/

_gs_gfd(path, buffer, count)
int            path;
struct fildes  *buffer;
int            count;
   {
   return (getstat(0x0f, path, buffer, count));
   }


/*
** simulate the 68K _ss_pfd() function
*/

_ss_pfd(path, buffer)
int            path;
struct fildes  *buffer;
   {
   return (setstat(0x0f, path, buffer));
   }
