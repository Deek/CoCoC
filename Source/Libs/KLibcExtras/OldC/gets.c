#include <stdio.h>

char  *gets(s)
char  *s;
   {
   register char  *p = s;
   int            c;

   while (((c = getc(stdin)) != '\n') && (c != -1))
      *p++ = c;

   if (c == -1)
      return (0);

   *p = '\0';
   return (s);
   }


char  *fgets(s, n, fp)
char           *s;
register int   n;
FILE           *fp;
   {
   int   c;
   char  *p;

   p = s;
   while ((n-- > 0) && ((c = getc(fp)) != -1))
      if ((*p++ = c) == '\n')
         break;

   *p = '\0';
   return ((c == -1) ? 0 : s);
   }
