/*
**  function to return a char or -1 if none ready
*/

inkey(pn)
int   pn;                                             /* path number */
   {
   char  c;                                         /* read variable */

   if ((getstat(SS_READY, pn) == -1)
         return (-1);                     /* return not ready status */

   read(pn, &c, 1);                                  /* get one char */
   return (c);
   }


