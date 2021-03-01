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
#ifdef   OSK
   _gs_gfd(pn, fs, sizeof(FILDES));
#else

   struct registers regs;

   regs.rg_a = pn;
   regs.rg_b = SS_FD;
   regs.rg_x = fs;
   regs.rg_y = sizeof(FILDES);
   _os9(I_GETSTT, &regs);
#endif
   }


/*
** set file attributes
*/

set_fstat(pn, fs)
int      pn;
FILDES   *fs;
   {
#ifdef   OSK
   _ss_pfd(pn, fs, sizeof(FILDES));
#else

   struct registers regs;

   regs.rg_a = pn;
   regs.rg_b = SS_FD;
   regs.rg_x = fs;
   regs.rg_y = sizeof(FILDES);
   _os9(I_SETSTT, &regs);
#endif
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

#ifdef   OSK
   size = _gs_size(pn);
#else
   getstat(SS_SIZE, pn, &size);
#endif
   return (size);
   }


/*
** change the file size
*/

set_fsize(pn, size)
int   pn;
long  size;
   {
#ifdef   OSK
   _ss_size(pn, size);
#else
   setstat(SS_SIZE, pn, size);
#endif
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
**      Returns true if string s matches pattern p.
*/

patmatch(p, s, f)
char           *p;                                       /* pattern */
register char  *s;                               /* string to match */
char           f;                            /* flag for case force */
   {
   char  pc;                     /* a single character from pattern */

   while (pc = (f ? toupper(*p++) : *p++))
      {
      if (pc == '*')
         {
         do {                    /* look for match till s exhausted */
            if (patmatch (p, s, f))
                  return (1);
            } while (*s++);
         return (0);
         }
      else
         if (*s == 0)
            return (0);                       /* s exhausted, p not */
         else
            if (pc == '?')
               s++;                       /* matches all, just bump */
            else
               if (pc != (f ? toupper(*s++) : *s++))
                  return (0);
      }
   return (!*s);            /* p exhausted, ret true if s exhausted */
   }


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


#include <ctype.h>
/*
** special strcmp to ignore case
*/

strucmp(s1, s2)
char          *s1;
register char *s2;
   {
   while (toupper(*s1) == toupper(*s2))
      {
      if (*s2++ == 0)
            return (0);
      s1++;
      }
   return (toupper(*s1) - toupper(*s2));
   }
