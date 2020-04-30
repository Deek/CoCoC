
static char buf[] = "lx";

pflong(c, lp)
char  c;
long  **lp;
   {
   switch (c)
      {
      case 'd' :
      case 'o' :
      case 'x' :
         (*lp)++;
         return ("");

      default  :
         buf[1] = c;
         return (buf);
      }
   }
