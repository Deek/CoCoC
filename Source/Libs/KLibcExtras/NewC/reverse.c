/*
** reverses string 's'
*/

reverse(s)
char *s;
   {
   register char *hd = s;
   char *tl, temp;
   tl = hd + strlen(hd) - 1;
   while (hd < tl)
      {
      temp = *hd;
      *hd++ = *tl;
      *tl-- = temp;
      }
   return (s);
   }


