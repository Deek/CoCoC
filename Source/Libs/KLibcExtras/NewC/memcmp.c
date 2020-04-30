/*
** Compare n bytes, returning *s1 - *s2.
**  This amounts to s1 >, ==, or < s2.
*/

memcmp(s1, s2, n)
register char  *s1, *s2;
int            n;
   {
   int   diff;

   if (s1 != s2)                     /* don't bother if same string */
      while (n-- > 0)
         if (diff = *s1++ - *s2++)
            return (diff);
   return (0);
   }
