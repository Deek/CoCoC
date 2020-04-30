/*
** function to swap bytes in an int
*/
swab(i)
unsigned i;
   {
   return (((i >> 8) & 0x00ff) + (i << 8));
   }
