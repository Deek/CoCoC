/*
** Get a hex digit from 's' into 'x'.
*/

#include <ctype.h>

htoi(s)
char  *s;
   {
   char  ch;
   int   x = 0;

   while (isxdigit(ch = *s++))
      {
      x = (x << 4) + ch - (ch > '9' ? '7' : '0');
      }
   return (x);
   }
