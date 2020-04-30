#include <stdio.h>

_setbase(fp)
register FILE  *fp;
   {
   if ((fp->_flag & _DEVMASK) == 0)
      {
      char  buf[32];

      getstat(0, fp->_fd, buf);
      fp->_flag |= ((buf[0] == 0) ? _SCF : _RBF);
      }

   if (((fp->_flag |= _INIT) & (_BIGBUF+_UNBUF)) != 0)
      return;

   if (fp->_bufsiz == 0)
      fp->_bufsiz = ((fp->_flag & _SCF) ? 128 : 256);

   if ((fp->_base != 0) || ((fp->_base = ibrk(fp->_bufsiz)) != -1))
      fp->_flag |= _BIGBUF;
   else
      {
      fp->_flag |= _UNBUF;
      fp->_base = &fp->_save;
      fp->_bufsiz = 1;
      }

   fp->_ptr = fp->_end = fp->_base + fp->_bufsiz;
   }
