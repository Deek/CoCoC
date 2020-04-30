/*
** converts integer 'n' into 'd' digits in string 's'.
*/

#include <ctype.h>

itoh(n, d, s)
int   n, d;
char *s;
   {
   register char  *p = s + d + 1;
   char  c;
   s = p;
   *p-- = '\0';
   *p = ' ';
   for (; d; d--)
      {
      c = (n & 0x0f) + '0';
      if (c > '9')
            c += 7;
      n = n >> 4;
      *--p = c;
      }
   return (s);
   }
