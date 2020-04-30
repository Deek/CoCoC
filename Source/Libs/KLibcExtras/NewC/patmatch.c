/*
**      Returns true if string s matches pattern p.
*/

patmatch(p, s, f)
char           *p;                                       /* pattern */
register char  *s;                               /* string to match */
int            f;                            /* flag for case force */
   {
   char  pc;                     /* a single character from pattern */

   while (pc = (f ? toupper(*p++) : *p++))
      {
      if (pc == '*')
         {
         do {                    /* look for match till s exhausted */
            if (patmatch (p, s, f))
               return (1);
            } while (*s++);
         return (0);
         }
      else
         if (*s == 0)
            return (0);                       /* s exhausted, p not */
         else
            if (pc == '?')
               s++;                       /* matches all, just bump */
            else
               if (pc != (f ? toupper(*s++) : *s++))
                  return (0);
      }
   return (*s == 0);        /* p exhausted, ret true if s exhausted */
   }
