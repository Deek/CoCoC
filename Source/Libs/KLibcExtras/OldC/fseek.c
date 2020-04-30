#include <stdio.h>

fseek(fp, offset, type)
register FILE  *fp;
long           offset;
int            type;
   {
   long  temp, lseek(), ftell();
   char  *p;

   if ((fp == 0) || ((fp->_flag & (_READ+_WRITE)) == 0))
      return (-1);

   if ((fp->_flag & _INIT) == 0)
      _setbase(fp);
   else
      if (fp->_flag & _WRITTEN)
         {
         fflush(fp);
         fp->_flag &= ~_WRITTEN;
         fp->_ptr = fp->_end = fp->_base + fp->_bufsiz;
         }
      else
         {
         if (fp->_ptr < fp->_end)
            {
            temp = offset;
            switch (type)
               {
               case 0 :
                  temp -= ftell(fp);

               case 1 :
                  if ((long) fp->_bufsiz >= ((temp < 0) ? -temp : temp))
                     if (fp->_base <= (p = fp->_ptr + (int) temp))
                        if (p < fp->_end)
                           {
                           fp->_ptr = p;
                           fp->_flag &= ~_EOF;
                           return (0);
                           }
               }
            if (type == 1)
               offset -= (fp->_end - fp->_ptr);
            fp->_ptr = fp->_end;
            }
         }
   fp->_flag &= ~_EOF;
   if (lseek(fp->_fd, offset, type) == -1)
      return (-1);
   return (0);
   }
/*page*/
rewind(fp)
FILE  *fp;
   {
   fseek(fp, 0L, 0);
   }


long
ftell(fp)
register FILE  *fp;
   {
   long  lseek();

   if ((fp == 0) || ((fp->_flag & (_READ+_WRITE)) == 0))
      return (-1L);
   if ((fp->_flag & _INIT) == 0)
      _setbase(fp);
   return (lseek(fp->_fd, 0L, 1) +
          (long)(fp->_ptr -
                ((fp->_flag & _WRITTEN) ? fp->_base : fp->_end)));
   }
