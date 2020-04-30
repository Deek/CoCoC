/*
**      Returns true if string s matches pattern p.
*/

#define ifup(a) (f ? toupper(a) : a)

patmatch(p, s, f)
char           *p;                                       /* pattern */
register char  *s;                               /* string to match */
int            f;                            /* flag for case force */
   {
   char  pc,                     /* a single character from pattern */
         sc;                      /* a single character from string */
   int   found, compl;

   while (pc = ifup(*p++))
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
               if (pc == '[')                    /* character class */
                  {
                  sc = ifup(*s++);
                  if (compl = (*p == '^'))      /* class inversion? */
                     p++;
                  found = 0;
                  while ((pc = ifup(*p++)) != ']')
                     if (pc == 0)                  /* check for end */
                        {                     /* no terminating ']' */
                        p--;
                        break;
                        }
                     else
                        if (*p == '-')           /* check for range */
                           {
                           ++p;
                           found |= ((pc <= sc) && (sc <= ifup(*p++)));
                           }
                        else
                           found |= (pc == sc);

                  if (!found ^ compl)
                     return (0);
                  }
               else
                  if (pc != ifup(*s++))
                     return (0);
      }
   return (*s == 0);        /* p exhausted, ret true if s exhausted */
   }
