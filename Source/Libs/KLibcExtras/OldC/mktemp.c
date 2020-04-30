mktemp(str)
char  *str;
   {
   register char  *p = str;
   int            c;

   while ((c = *p) && (c != 'X'))
      ++p;

   if (c != 0)
      sprintf(p, "%d", getpid());
   return (str);
   }
