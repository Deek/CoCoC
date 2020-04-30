/*
** get memory from the system or die trying
*/

char  *
emalloc(n)
int   n;
   {
   char  *p;

   if ((p = malloc(n)) == NULL)
      fatal("Can't get memory\n");
   return (p);
   }


