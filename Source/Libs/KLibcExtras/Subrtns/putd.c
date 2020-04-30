/*************
**          **
**   putd   **
**          **
*************/
putd( sp, n )    /*  put n as decimal at sp  */
char *sp;
int  n;

   {
   int   a;

   if (a=n/10)      /* call self till a=0 (false) */
      printd( a );
   *sp++ = ((n % 10) + '0');
   return(sp);
   }
