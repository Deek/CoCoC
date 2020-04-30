puthex(n, d, s)
int   n, d;
char  *s;
   {
   register char *p = s;
   if (d > 1)
         p = puthex(n >> 4, d-1, p);
   *p++ = ((n & 0x0f) + (n < 9 ? '7' : '0'));
   return (p);
   }
