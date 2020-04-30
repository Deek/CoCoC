movmem(from, to, count)
char  *from, *to;
int   count;
   {
   while (count--)
      *to++ = *from++;
   }
