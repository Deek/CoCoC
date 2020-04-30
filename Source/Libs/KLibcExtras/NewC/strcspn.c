/*
** Return the number of leading chars in 's' NOT from 'chset'
*/

strcspn(s, chset)
char  *s, *chset;
   {
   register char  *p = s;

   while (!strchr(chset, *p))
      ++p;
   return (p - s);
   }
