/*
** finds the end of a string
*/

strend(s)
register char  *s;
   {
   while (*s++)
      ;
   return (--s);
   }
