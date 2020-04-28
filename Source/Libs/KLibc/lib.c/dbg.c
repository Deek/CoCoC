/*
** attempt at basic trace type of debug at a macro level,  ie report
**  entry and exit from functions.
*/

#include <ctype.h>
#include <dbg.h>
#include <stdio.h>

static char  *sp, *_getsp();
static int   callev = 0;
static char  *lp, line[80];
static char  *imsg = "-> <";
static char  *omsg = "-< <";
static char  *nl = ">\n";
static char  *spaces = "----------------------------------------";
static char  *uf = " *** nesting underflow *** ";
static char  *uspmsg = " user stack starts at ";
static char  *prompt = "dgb (b,d,g*,m,s) ? ";
static int   trace  = 0;
static char  match[9];



_prof(pf, s)
int   pf;       /* really pointer to a function address, but .... */
char  *s;          /* pointer to the name of the function */
   {
   int   n;

   if (trace)
      {
      sp = _getsp() + 10;
      if ((n = callev++) > 40)
         n = 40;
      write(2, spaces, n);
      write(2, imsg, strlen(imsg));
      write(2, s, strlen(s));
      writeln(2, nl, strlen(nl));
      if (trace & E_INTERACT)
         interact(s);
      }
   }


/*page*/
_eprof(s)
char  *s;         /* name of function we are leaving */
   {
   int   n;

   if (trace)
      {
      sp = _getsp() + 8;
      if ((n = --callev) < 0)
         {
         n = callev = 0;
         write(2, uf, strlen(uf));
         }
      else
         if (n > 40)
            n = 40;
      write(2, spaces, n);
      write(2, omsg, strlen(omsg));
      write(2, s, strlen(s));
      writeln(2, nl, strlen(nl));
      if (trace & L_INTERACT)
         interact(s);
      }
   }
/*page*/
static
interact(s)
char  *s;
   {
   if ((match[0] == 0) || (strcmp(match, s) == 0))
      for (;;)
         {
         write(2, prompt, strlen(prompt));
         readln(0, line, 20);
         lp = skipspace(line);
         switch (tolower(*lp++))
            {
            case 'b' :
               lp = skipspace(lp);
               if (*lp == '\n')
                  match[0] = '\n';
               else
                  {
                  *(lp + 8) = '\0';           /* 8 chars max */
                  strcpy(match, lp);
                  }
               break;
   
            case 'd' :
               dump(lp);
               break;
   
            case 'g' :
            case '\n' :
               return;
   
            case 'm' :
               memchg(lp);
               break;
            
            case 's' :
               showsp();
               break;
            }
         }
   }


_trace(n)
int   n;
   {
   if ((trace = n) == TROFF)
      match[0] = '\0';                /* wipe saved name */
   }


_dumprof(){}

/*page*/
char  *h1 =
{"addr   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  0 2 4 6 8 a c e \n"};
char *h2 =
{"----  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ----------------\n"};

/*
** full screen display of 256 bytes from the prom
*/
static dump(s)
char  *s;
   {
   int   j, k, incr;
   char  *addr;
   register char  *p;
   char  c;
   char  working = 1;

   addr = gethex(skipspace(s));
   showsp();
   while (working)
      {
      writeln(2, h1, 99);
      writeln(2, h2, 99);
      for (j = 0; j < 256; j += 16)
         {
         p = line;
         p = itoh(j + addr, 4, p);
         *p++ = ' ';
         for (k = 0; k < 16; k += 1)
            p = itoh(addr[j + k], 2, p);
         *p++ = ' ';
         for (k = 0; k < 16; k += 1)
            *p++ = (((c = addr[j + k]) < 32) ? '.' : c & 0x7f);
         *p++ = '\n';
         writeln(2, line, 99);
         }
      write(2, "\l (u,.,gnnn,q) ? ", 17);
      readln(0, line, 32);
      p = skipspace(line);
      switch (tolower(*p & 0xff))
         {
         case 'g' :
            incr = 0;
            addr = gethex(skipspace(++p));
            break;

         case 'u' :
         case '^' :
               incr = -256;
               break;

         case 'q' :
               working = 0;
               break;

         default   :
               incr = 256;
               break;
         }
      addr += incr;
      }
   }
/*page*/
/*
** changes memory locations in driver's buffer.
**  do not read prom here as user may kick out and come back.
*/

static memchg(s)
char  *s;
   {
   register char  *p;
   char  n;
   char  *addr;
   int   incr, cmd;
   char done = 0;

   addr = gethex(skipspace(s));
   showsp();
   while (!done)
      {
      p = line;
      p = itoh(addr, 4, p);
      p = itoh(*addr, 2, p);
      write(2, line, p - line);
      readln(0, line, 10);
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
            if (*p != '\n')
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
                  incr = 0;
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
** converts integer 'n' into 'd' digits in string 's'.
*/

static itoh(n, d, s)
int   n, d;
char *s;
   {
   register char  *p = s + d + 1;
   char  c;
   s = p;
   *p-- = '\0';
   *p = ' ';
   for (; d; d--)
      {
      c = (n & 0x0f) + '0';
      if (c > '9')
            c += 7;
      n = n >> 4;
      *--p = c;
      }
   return (s);
   }


/*
** Get a hex digit from 's' into 'x'.
*/

static gethex(s)
char  *s;
   {
   char  ch;
   int   x = 0;
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


static skipspace(s)
char  *s;
   {
   while (*s == ' ')
      s++;
   return(s);
   }


static showsp()
   {
   char  *p;

   strcpy(p = line, uspmsg);
   p = itoh(sp, 4, p + strlen(p));
   strcpy(p, "\n");
   writeln(2, line, strlen(line));
   }
