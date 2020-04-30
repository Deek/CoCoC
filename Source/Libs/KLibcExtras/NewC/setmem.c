/*
** initialize memory (variables) of string type
*/

setmem(p, q, v)
char  *p;                                                 /* string */
int   q;                                                   /* count */
char  v;                                                   /* value */
   {
   while (q--)
      *p++ = v;
   }


