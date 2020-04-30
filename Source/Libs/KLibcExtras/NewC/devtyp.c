#include <os9.h>

isatty(pn)
int   pn;
   {
   return (devtyp(pn) == 0);
   }


devtyp(pn)
int   pn;
   {
   char  pd[32];

   getstat(SS_OPT, pn, pd);
   return (pd[0]);
   }


devname(pn)
int   pn;
	{
	static  devnam[32];

	getstat(SS_DEVNAM, pn, devnam);
	return (devnam);
	}
