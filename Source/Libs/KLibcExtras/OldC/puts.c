#include <stdio.h>

puts(s)
char  *s;
   {
   fputs(s, stdout);
   putc('\n', stdout);
   }


fputs(s, fp)
register char  *s;
FILE           *fp;
   {
   char  c;

   while (c = *s++)
      putc(c, fp);
   }
