strins(to, from, offset)
char *to, *from;
int offset;
   {
   int i, j, len1, len2;
   len1 = strlen(to);
   len2 = strlen(from);
   i = len1;
   j = len1 + len2;
   while (i >= offset)
      to[j--] = to[i--];
   i = 0;
   j = offset;
   while (i < len2)
      to[j++] = from[i++];
   return (to);
   }


strdel(s, cnt, offset)
char *s;
int cnt, offset;
   {
   int len;
   len = strlen(s);
   if (offset + cnt >= len)
      return (s);
   strcpy((s + offset), (s + offset + cnt));
   return (s);
   }


strsub(s1, s2)
char *s1, *s2;
   {
   int cnt, len1, len2;
   char *pnt1, *pnt2;
   len1 = strlen(s1);
   len2 = strlen(s2);
   if (len1 < len2)
      return (-1);
   cnt = 0;
   len1 -= len2;
   while (len1--)
      {
      pnt1 = s1++;
      pnt2 = s2;
      while (*pnt1 == *pnt2)
         {
         pnt1++;
         pnt2++;
         }
      if (*pnt2 == '\0')
         return (cnt);
      cnt++;
      }
   return (-1);
   }
