#include <stdio.h>

getc(fp)
register FILE  *fp;
   {
   if (fp == 0)
      return (-1);
   if ((fp->_flag & _WRITTEN) != 0)
      return (-1);
   if (fp->_ptr < fp->_end)
      return (*fp->_ptr++ & 0xff);
   return (_fillbuf(fp));
   }


ungetc(ch, fp)
int   ch;
register FILE  *fp;
   {
   if (fp == 0)
      return (-1);
   if ((fp->_flag & _READ) == 0)
      return (-1);
   if (ch == -1)
      return (-1);
   if (fp->_ptr <= fp->_base)
      return (-1);
   *--fp->_ptr = ch;
   return (ch);
   }


getw(fp)
register FILE  *fp;
   {
   int   l, r;

   if ((l = getc(fp)) == -1)
      return (-1);
   if ((r = getc(fp)) == -1)
      return (-1);
   return ((l << 8) + r);
   }

/*page*/
static
_fillbuf(fp)
register FILE  *fp;
   {
   int   reslt, readln(), read();

   if ((fp->_flag & (_INIT+_ERR+_EOF+_READ)) != (_INIT+_READ))
      if ((fp->_flag & (_EOF+_ERR+_READ)) != _READ)
         return (-1);
      else
         _setbase(fp);
   if (fp == &_iob[0])
      if (fp->_flag & _SCF)
         fflush(stdout);
   if (fp->_flag & _BIGBUF)
      reslt = (*(fp->_flag & _SCF ? readln : read))(fp->_fd, fp->_base, fp->_bufsiz);
   else
      reslt = read(fp->_fd, fp->_base = &fp->_save, 1);
   if (reslt <= 0)
      {
      fp->_flag |= (reslt != 0) ? _ERR : _EOF;
      return (-1);
      }
   fp->_ptr = fp->_base + 1;
   fp->_end = fp->_base + reslt;
   return (*fp->_base & 0xff);
   }
