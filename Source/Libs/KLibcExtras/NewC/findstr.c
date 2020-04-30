#include <stdio.h>

/*
** routine to find a substring in a larger string
*/

char  *
findstr(s1, s2)
char  *s1, *s2;
   {
   char  *p1, *p2;
   int   cnt;

   for (cnt = strlen(s1) - strlen(s2); cnt >= 0; --cnt)
      {
      for (p1 = s1++, p2 = s2; *p1 == *p2; p1++, p2++)
         ;
      if (*p2 == '\0')
         return (--s1);
      }
   return ((char *) 0);
   }
