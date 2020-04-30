/*
** changes memory locations in driver's buffer.
**  do not read prom here as user may kick out and come back.
*/

#include  <ctype.h>
#include  "burn.exh"

memchg(addr)
int   addr;
   {
   register char  *p;
   char  n;
   int   incr;
   char done = 0;
   while (!done)
      {
      p = obuf;
      setstat(bpn, SETREAD, addr);
      read(bpn, &n, 1);
      p = itoh(addr, 4, p);
      p = itoh(n, 2, p);
      write(STDOUT, obuf, p - obuf);
      readln(STDIN, line, 10);
      p = line;
      while (*p == ' ')
         p++;
      if (isxdigit(*p))
            {
            n = gethex(p);
            setstat(bpn, SETRECV, addr);
            write(bpn, &n, 1);
            incr = 1;
            }
         else
            {
            switch(tolower(*p++))
               {
               case 'u' :
               case '^' :
                  incr = (isxdigit(*p) ? -(gethex(p)) : -1);
                  break;
               case '\r':
                  done = 1;
                  incr = 0;
                  break;
               default  :
                  incr = (isxdigit(*p) ? gethex(p) : 1);
                  break;
               }
            }
      addr += incr;
      if (addr > size)
            addr -= size;
      if (addr < size)
            addr += size;
      }
   }
