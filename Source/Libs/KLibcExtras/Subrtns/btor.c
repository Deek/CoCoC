/*
** subroutine to convert binary to roman
*/
btor(s, n, big)
char  *s;
int   n, big;
   {
   register char  *p = s;

   n = _doseg(p, n, 900, 500, 400, 100, 'M', 'D', 'C');
   n = _doseg(strend(p), n, 90, 50, 40, 10, 'C', 'L', 'X');
   _doseg(strend(p), n, 9, 5, 4, 1, 'X', 'V', 'I');
   if (!big)
      while (*p)
         *p++ = *p | 0x20;
   return (s);
   }


_doseg(s, n, n1, n2, n3, n4, c1, c2, c3)
char  *s;
int   n, n1, n2, n3, n4;
char  c1, c2, c3;
   {
   if (n >= n1)
      {
      n -= n1;
      *s++ = c3;
      *s++ = c1;
      }
   if (n >= n2)
      {
      n -= n2;
      *s++ = c2;
      }
   if (n >= n3)
      {
      n -= n3;
      *s++ = c3;
      *s++ = c2;
      }
   while (n >= n4)
      {
      n -= n4;
      *s++ = c3;
      }
   *s = '\0';
   return (n);
   }
