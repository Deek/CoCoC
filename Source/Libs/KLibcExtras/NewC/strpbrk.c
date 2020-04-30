/*
 *  return a ptrr to first occurance of any character from
 *   'brkset' in 'string'; NILL if none exists
 */

char  *
strpbrk(string, brkset)
register char  *string, *brkset;
   {
   for ( ; *string; ++string)
      if (strchr(brkset, *string))
         return (string);
   return ((char *) 0);
   }
