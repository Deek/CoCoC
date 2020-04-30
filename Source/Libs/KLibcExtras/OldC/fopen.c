#include <stdio.h>


static FILE *findfp()
   {
   register FILE  *fp = _iob;

   do {
      if ((fp->_flag & (_READ + _WRITE)) == 0)
         return (fp);
      else
         ++fp;
      } while (fp <= &_iob[_NFILE]);
   errno = 0xc8;
   return (0);
   }


static common(pn, action, fp)
int   pn;
char  *action;
register FILE  *fp;
   {
   if (fp == 0)
      fp = findfp();
   if (fp)
      {
      fp->_fd = pn;
      if (action[1] == '+' || action[2] == '+')
         fp->_flag |= (_READ + _WRITE);
      else
         fp->_flag |=
                ((action[0] == 'r' || action[0] == 'd') ? _READ : _WRITE);

      fp->_ptr = fp->_end = fp->_base + fp->_bufsiz;
      return (fp);
      }
   return (0);
   }
/*page*/
static openit(name, action)
register char  *name;
char  *action;
   {
   int   pn, mask = 0;

   switch(action[1])
      {
      case '\0':
         break;

      case 'x' :
         if (action[2] == '+')
            mask = 4 + 2 + 1;
         else
            mask = 4;
         break;

      case '+' :
         mask = 2 + 1;
         break;

      default:
         goto skip;
      }

   switch (action[0])
      {
      case 'r':
         pn = open(name, mask | 1);
         break;

      case 'a' :
         if ((pn = open(name, mask | 2)) != -1)
            {
            lseek(pn, 0L, 2);
            return (pn);
            }

      case 'w' :
         pn = create(name, mask | 2);
         break;

      case 'd' :
         pn = open(name, mask | 0x81);
         break;

skip:
      default  :
         errno = 0xcb;
         return (0);
      }
   return (pn);
   }
/*page*/
FILE *
fdopen(fp, action)
FILE  *fp;
char  *action;
   {
   return (common(fp, action, 0));
   }


FILE *
fopen(name, action)
char  *name, *action;
   {
   register int   pn;

   if ((pn = openit(name, action)) == -1)
      return (0);
   else
      return (common(pn, action, 0));
   }


FILE *
freopen(name, action, fp)
char  *name, *action;
FILE  *fp;
   {
   register int   pn;

   fclose(fp);
   if ((pn = openit(name, action)) < 0)
      return (0);
   return (common(pn, action, fp));
   }
