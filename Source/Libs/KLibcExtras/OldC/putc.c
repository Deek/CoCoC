#include <stdio.h>

int   write(), writeln(), _setbase();

putc(c, fp)
int   c;
register FILE  *fp;
   {
   if ((fp->_flag & (_INIT | _ERR | _WRITE)) != (_WRITE | _INIT))
      if ((fp->_flag & (_ERR | _WRITE)) != _WRITE)
         return (-1);
      else
         _setbase(fp);

   if (fp->_flag & _UNBUF)
      {
      if (((*(fp->_flag & _SCF ? writeln : write))(fp->_fd, (char)&c+1, 1)) == EOF)
         {
         fp->_flag |= _ERR;
         return (EOF);
         }
      }
   else
      {
      if ((fp->_flag & _WRITTEN) == 0)
         _flush(fp);
      *(fp->_ptr++) = c;
      if ((fp->_ptr >= fp->_end) || ((fp->_flag & _SCF) && (c == '\n')))
         if (_flush(fp) != 0)
            return (EOF);
      }
   return (c);
   }


/*
** put a word to a file
*/

putw(w, fp)
register unsigned   w;
FILE  *fp;
   {
   putc(w >> 8, fp);
   putc(w, fp);
   }
/*page*/
/*
** routine to tidy up before exit
*/

_tidyup()
   {
   register FILE  *fp = stdin;
   int   j;

   j = 0;
   while (j++ < _NFILE)
      fclose(fp++);
   }


/*
** close the file for fp
*/

fclose(fp)
register FILE  *fp;
   {
   int   status;

   if ((fp == 0) || (fp->_flag == 0))
      return (-1);

   if (fp->_flag & _WRITE)
      status = fflush(fp);
   else
      status = 0;

   close(fp->_fd);
   fp->_flag = 0;
   return (status);
   }
/*page*/
/*
** flush the buffers associated with an fp
*/

fflush(fp)
register FILE  *fp;
   {
   if ((fp == 0) || ((fp->_flag & (_ERR + _WRITE)) != _WRITE))
      return (-1);

   if ((fp->_flag & _INIT) == 0)
      _setbase(fp);

   _flush(fp);
   }


/*
** flusher - called by many above
*/

static _flush(fp)
register FILE  *fp;
   {
   int   n;
   int   m;

   if (!(fp->_flag & _WRITTEN) && (fp->_ptr != fp->_end))
      lseek(fp->_fd, ftell(fp), 0);

   if (n = fp->_ptr - fp->_base)
      if (fp->_flag & _WRITTEN)
         if (fp->_flag & _SCF)
            {
            fp->_ptr = fp->_base;
            while (n)
               {
               if ((m = writeln(fp->_fd, fp->_ptr, n)) == -1)
                  goto error;
               n -= m,
               fp->_ptr += m;
               }
            }
         else
            {
            if ((write(fp->_fd, fp->_base, n)) != n)
error:
               {
               fp->_flag |= _ERR;
               fp->_ptr = fp->_end;
               return (-1);
               }
            }
   
   fp->_flag |= _WRITTEN;
   fp->_end = (fp->_ptr = fp->_base) + fp->_bufsiz;
   return (0);
   }
