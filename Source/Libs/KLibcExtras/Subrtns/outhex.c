out4hs(n, s)
int   n;
char  *s;
   {
   register char *p = s;
   p = puthex(n, 4, p);
   *p++ = ' ';
   *p ='\0';
   return (p);
   }


out2hs(n, s)
int   n;
char  *s;
   {
   register char *p = s;
   p = puthex(n, 2, p);
   *p++ = ' ';
   *p ='\0';
   return (p);
   }


