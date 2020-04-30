#include <stdio.h>

setbuf(fp, addr)
register FILE  *fp;
char           *addr;
   {
   if ((fp->_flag & _WRITTEN) != 0)
      fflush(fp);
   fp->_flag &= ~(_WRITTEN + _UNBUF + _BIGBUF);
   if (addr)
      {
      if (fp->_bufsiz == 0)
         fp->_bufsiz = BUFSIZ;
      fp->_flag |= _BIGBUF;
      fp->_ptr = fp->_end = fp->_bufsiz + (fp->_base = addr);
      }
   else
      {
      fp->_flag |= _UNBUF;
      fp->_ptr = fp->_end = &fp->_bufsiz;   /* why not &fp->_save? */
      }
   }
