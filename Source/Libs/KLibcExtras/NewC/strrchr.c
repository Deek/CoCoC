/*
** Return a pointer to the last occurrence of 'c' in 's'
**  or NULL if none
*/

char  *
strrchr(sp, c)
register char  *sp, c;
   {
   char  *r = (char *) 0;

   do {
      if (*sp == c)
         r = sp;
      } while (*sp++);
   return (r);
   }
