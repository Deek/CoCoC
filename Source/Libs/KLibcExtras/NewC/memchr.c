/*
** Return a pointer to the character 'c' in 's' if there,
**  or NULL if not found in n chars; don't stop at \0.
*/

char *
memchr(s, c, n)
register char  *s, c;
int            n;
   {
   while (n-- > 0)
      if (*s++ == c)
         return (--s);
   return (0);
   }
