#include <stdio.h>

fwrite(ptr, eltsiz, nelt, fp)
register char  *ptr;
int            eltsiz, nelt;
FILE           *fp;
   {
   int            k, j;

   for (k = 0; k < nelt; k++)
      for (j = 0; j++ < eltsiz; )
         {
         putc(*ptr++, fp);
         if (ferror(fp))
            return (k);
         }
   return (k);
   }
