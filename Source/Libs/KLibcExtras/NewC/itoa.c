itoa(n, s)        /* convert n to characters in s */
int   n;
char  *s;
   {
   int   sign = 0;
   register char  *p = s;

   if (sign = (n < 0))
         n = -n;
   do
      {
      *p++ = n % 10 + '0';
      } while ((n /= 10) > 0);
   if (sign)
         *p++ = '-';
   *p = '\0';
   reverse(s);
   }
