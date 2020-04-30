/*
**  printd
**   print 'n' as decimal
*/
printd(n) 
int n;

   {
   int   a;

   if (a = n / 10)                  /* call self till a = 0 (false) */
      printd(a);
   putchar((n % 10) + '0');
   }
