#include <stdio.h>

fread(ptr, eltsiz, nelt, fp)
register char  *ptr;
int            eltsiz, nelt;
FILE           *fp;
   {
   int            j, k, c;

   for (k = 0; k < nelt; k++)
      for (j = eltsiz; j-- > 0; )
         if ((c = getc(fp)) == -1)
            return (k);
         else
            *ptr++ = c;

   return (k);
   }
