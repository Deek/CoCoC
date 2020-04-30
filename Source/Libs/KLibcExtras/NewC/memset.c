/*
** Set an array of n chars starting at s to the character c.
** Return s.
*/

char *
memset(s, c, n)
register char  *s, c;
int            n;
   {
   char  *os = s;

   while (n-- > 0)
      *s++ = c;
   return (os);
   }
