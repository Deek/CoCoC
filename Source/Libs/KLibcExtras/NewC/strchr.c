/*
** Return a pointer to the char 'c' in 's' if found
**  else return NULL
*/

char  *
strchr(s, c)
register char  *s, c;
   {
   do {
      if (*s == c)
         return (s);
      } while (*s++);
   return ((char *) 0);
   }
