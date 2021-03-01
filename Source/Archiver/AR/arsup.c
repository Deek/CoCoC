/*
** some lowlevel routines that are also in my library.
**  this file contains the C versions for those who don't
**  have access to my library
*/

#ifndef  OSK
#include <os9.h>
#endif
#include "ar.h"
#include <stdio.h>
#include <ctype.h>

/*
** get file stats using _os9 for portability
*/

get_fstat(pn, fs)
int      pn;
FILDES   *fs;
   {
   _gs_gfd(pn, fs, sizeof(FILDES));
   }


/*
** set file attributes
*/

set_fstat(pn, fs)
int      pn;
FILDES   *fs;
   {
   _ss_pfd(pn, fs, sizeof(FILDES));
   _ss_attr(pn,fs->fd_attr);
   }
/*page*/
/*
** get the file size
*/

long
get_fsize(pn)
int   pn;
   {
   long  size;

   size = _gs_size(pn);
   return (size);
   }


/*
** change the file size
*/

set_fsize(pn, size)
int   pn;
long  size;
   {
   _ss_size(pn, size);
   }


/*
**  convert a long read from disk as an array of char
**   back to a long.
*/

long
c4tol(s)
char  *s;
   {
   long  x = 0;

   x = (x + (*s++ &0xff)) << 8;
   x = (x + (*s++ &0xff)) << 8;
   x = (x + (*s++ &0xff)) << 8;
   x = (x + (*s &0xff));
   return (x);
   }
/*page*/

/*
** initialize memory (variables) of string type
*/

setmem(p, q, v)
char  *p;                                                 /* string */
int   q;                                                   /* count */
char  v;                                                   /* value */
   {
   while (q--)
      *p++ = v;
   }

