/*
** clears 'cnt' bytes of string 's'
*/

strclr(s, cnt)
char *s;
int cnt;
   {
   register char *pnt = s;
   while (cnt--)
      *pnt++ = '\0';
   return (s);
   }


