/*
**      skip blanks and tabs in character buffer.
**      return number of characters skipped.
*/
char  *skipbl(p)
register char  *p;
   {
   while (*p == ' ' || *p == '\t')
      ++p;
   return (p);
   }


/*
**      skip over word and punctuation
*/
char  *skipwd(p)
register char  *p;
   {
   while (*p != ' ' && *p != '\t' && *p != '\0')
      ++p;
   return (p);
   }


