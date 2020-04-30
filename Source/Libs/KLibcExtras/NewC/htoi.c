/*
** Get a hex digit from 's' into 'x'.
*/

#include <ctype.h>

htoi(s)
register char  *s;
   {
   char  ch;
   int   x = 0;

   while (*s == ' ' || *s == '\t')
      ++s;

   while (isxdigit(ch = *s++))
      {
      x = (x << 4) + toupper(ch) - (ch > '9' ? '7' : '0');
      }
   return (x);
   }
