isxdigit(c)
char   c;
   {
   if (c < '0')
      return (0);
   else
      if (c <= '9')
         return (1);
      else
         if (c < 'A')
            return (0);
         else
            if (c <= 'Z')
               return (1);
            else
               if (c < 'a')
                  return (0);
               else
                  if (c <= 'z')
                     return (1);
                  else
                     return (0);
   }
