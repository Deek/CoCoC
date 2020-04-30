/*
**              pattern matching functionn
**      Returns true if string s matches pattern p.
*/
patmatch (p, s)
char   *p,                                      /* pattern */
       *s;                                      /* string to match */
   {
   char  pc,                      /* a single character from pattern */
         sc;                       /* a single character from string */
   bool found, compl;

   while (pc = *p++)
      {
      if (pc == '*')
         {
         do
            if (patmatch (p, s))
               return (TRUE);
         while (*s++);
         return (FALSE);
         }
      else
         if (!*s)
            return (FALSE);
         else
            if (pc == '?')
               s++;
            else
               if (pc == '[')          /* character class */
                  {
                  sc = *s++;
                  if (compl = (*p == '^'))       /* class inversion? */
                     p++;
                  found = FALSE;
                  while ((pc = *p++) != ']')        /* check for end */
                     if (pc)
                        if (*p == '-')            /* check for range */
                           found = found || between (pc, sc, *++p);
                        else
                           found = found || (pc == sc);
                     else                      /* no terminating ']' */
                        {
                        pc--;
                        break;
                        }
                  if (!found ^ compl)
                     return (FALSE);
                  }
               else
                  if (pc != *s++)
                     return (FALSE);
      }
   return (!*s);
   }

/* end of patmatch.c */
