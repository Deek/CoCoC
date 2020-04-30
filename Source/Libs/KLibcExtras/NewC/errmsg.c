/*
** function to emulate 68K _errmsg()
*/

#include <stdio.h>

_errmsg(code, msk, arg1, arg2, arg3)
int   code;
char  *msk;
int   arg1, arg2, arg3;
   {
   fprintf(stderr, "%s: ", _prgname());
   fprintf(stderr, msk, arg1, arg2, arg3);
   return (code);
   }
