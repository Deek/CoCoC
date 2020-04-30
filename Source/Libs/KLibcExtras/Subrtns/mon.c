/*
** A *VERY* mini-monitor to examine and change memory
*/

#include <stdio.h>

static char  *lp, line[80];
static char  *prompt = "mon (d,m,q) ? ";

main()
   {
   char  c, *skipspace();
   long  sp_save, trap1();

   sp_save = gemdos(0x20, 0L);
   putchar(0x1b);
   putchar('e');
   for (;;)
      {
      printf(prompt);
      gets(line);
      lp = skipspace(line);
      c = *lp++;
      switch (tolower(c))
         {
         case 'd' :
            dump(lp);
            break;

         case 'm' :
            memchg(lp);
            break;
         
         case 'q' :
            exit (0);
         }
      }
   gemdos(0x20, sp_save);
   }


/*page*/
char  *h1 =
{" addr    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  0 2 4 6 8 a c e \n"};
char *h2 =
{"------  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ----------------\n"};

/*
** full screen display of 256 bytes from memory
*/
dump(s)
char  *s;
   {
   int   j, k, incr;
   char  *addr, *p, c, *skipspace();
   long  gethex();
   char  working = 1;

   addr = gethex(skipspace(s));
   while (working)
      {
      fputs(h1, stdout);
      fputs(h2, stdout);
      for (j = 0; j < 256; j += 16)
         {
         p = addr + j;
         printf("%06lx  ", p);
         for (k = 0; k < 16; ++p,++k)
            printf("%02x ", *p & 0xff);
         putchar(' ');
         for (p = addr + j, k = 0; k < 16; ++p,++k)
            printf("%c", (((c = *p) < 32) ? '.' : c & 0x7f));
         printf("\n");
         }
      write(2, "\l (u,.,gnnn,q) ? ", 17);
      gets(line);
      p = skipspace(line);
      switch (tolower(*p & 0xff))
         {
         case 'g' :
            addr = gethex(skipspace(++p));
            break;

         case 'u' :
         case '^' :
               addr -= 256;
               break;

         case 'q' :
               working = 0;
               break;

         default   :
               addr += 256;
               break;
         }
      }
   }
/*page*/
/*
** changes memory locations in driver's buffer.
**  do not read prom here as user may kick out and come back.
*/

memchg(s)
char  *s;
   {
   char  *p, n, *addr, *skipspace();
   int   incr, cmd, done = 0;
   long  gethex();

   addr = gethex(skipspace(s));
   while (!done)
      {
      printf("%06lx %02x ", addr, *addr);
      gets(line);
      p = skipspace(line);
      if (isxdigit(*p))
         {
         n = gethex(p);
         *addr = n & 0xff;
         incr = 1;
         }
      else
         {
         cmd = tolower(*p & 0xff);
         if (*p != 0x0d)
            ++p;
         p = skipspace(p);
         switch(cmd)
            {
            case 'g' :
               incr = 0;
               addr = gethex(p);
               break;

            case 'u' :
            case '^' :
               incr = (isxdigit(*p) ? -(gethex(p)) : -1);
               break;

            case 'q':
               done = 1;
               break;

            default  :
               incr = (isxdigit(*p) ? gethex(p) : 1);
               break;
            }
         }
      addr += incr;
      }
   }
/*page*/
/*
** Get a hex digit from 's' into 'x'.
*/

long
gethex(s)
char  *s;
   {
   char  ch;
   long  x = 0;
   while (isxdigit(ch = *s++))
      {
      if (ch >= 'a')
         ch -= ('a' - 10);
      else
         if (ch >= 'A')
            ch -= ('A' - 10);
         else
            ch -= '0';
      x = (x << 4) + ch;
      }
   return (x);
   }


char *
skipspace(s)
char  *s;
   {
   while (*s == ' ')
      s++;
   return(s);
   }


isxdigit(c)
char  c;
   {
   if (c >= '0' && c <= '9')
      return (TRUE);
   if (c >= 'A' && c <= 'F')
      return (TRUE);
   if (c >= 'a' && c <= 'f')
      return (TRUE);
   return (FALSE);
   }


tolower(c)
char  c;
   {
   if (c >= 'A' && c <= 'Z')
      c -= 0x20;
   return (c);
   }


gets(s)
char  s[];
   {
   int   p, code;

   p = 0;
   code = gemdos(1);
   while (code != 0x0d && p < 77)
      {
      if (code != 8)
         s[p++] = code;
      else
         if (p > 0)
            {
            s[--p] = '\0';
            putchar(' ');
            putchar(8);
            }
      code = gemdos(1);
      }
   s[p++] = 0x0d;
   s[p] = '\0';
   printf("\n");
   }
