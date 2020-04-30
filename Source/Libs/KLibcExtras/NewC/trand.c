/* Global definitions for random number table
   and the twin pointers.                      */

static unsigned _rtable[55];
static int _rindx1, _rindx2;

srand(n)
int   n;
   {
   _rtable[0] = n | 0x0001;

   for (n = 1; n < 55; ++n)
      _rtable[n] = 27189 * _rtable[n - 1] + 1;

   _rindx1 = 54;
   _rindx2 = 23;
   }


rand(sf)
int   sf;
   {
   unsigned n;

   n = _rtable[_rindx1] += _rtable[_rindx2];
   if (--_rindx1 < 0)
         _rindx1 = 54;
   if (--_rindx2 < 0)
         _rindx2 = 54;

   return (n % sf);
   }
