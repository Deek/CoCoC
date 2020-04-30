#include <ctype.h>
/*
** special strcmp to ignore case
*/

strucmp(s1, s2)
char          *s1;
register char *s2;
   {
   while (toupper(*s1) == toupper(*s2))
      {
      if (*s2++ == 0)
            return (0);
      s1++;
      }
   return (toupper(*s1) - toupper(*s2));
   }
