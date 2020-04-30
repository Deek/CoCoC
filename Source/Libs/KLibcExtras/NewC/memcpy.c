/*
** Copy n bytes from s2 to s1.
** Return s1
*/

char *
memcpy(s1, s2, n)
register char  *s1, *s2;
int            n;
   {
   char  *os1 = s1;

   while (n-- > 0)
      *s1++ = *s2++;
   return (os1);
   }
