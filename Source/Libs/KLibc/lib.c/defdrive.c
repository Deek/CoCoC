#include <stddef.h>
#include <module.h>

static char d[12];

static char *getdrive()
   {
   char  *np;
   char  c;
   int   *jnk;
   struct mod_config *mp;
   register char     *p;

   if ((mp = (struct mod_config *)modlink("Init", 0x0c, 0)) != -1)
      {
      np = (char *)mp + mp->m_sysdrive;
      p = d;
      while ((c = *np++) > 0)
        *p++ = c;
      *p++ = (c & 0x7f);
      *p = 0;
      munlink((struct mod_com *)mp);
      return (d);
      }
   return (0);
   }
