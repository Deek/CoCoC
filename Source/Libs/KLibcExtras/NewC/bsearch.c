/*
** performs a binary search
*/

char  *
bsearch(key, base, nel, width, compar)
char  *key, *base;
int   nel, width;
int   (*compar)();
   {
   int   i, l = 1;
   int   val, hit;

   for (;;)
      {
      if (nel < l)
         return (NULL); /* not here */

      i = (nel + l) / 2;
      val = (*compar)(key, hit = base + (i - 1) * width);
      if (val == 0)
         return (hit);                                  /* found it */

      if (val < 0)
         nel = i - 1;                               /* below middle */
      else
         l = i + 1;                                 /* above middle */
      }
   }
