/*
** routines to initialize the Lempel-Zev version one routines
*/

#include "lz1.h"

lz1_init()
   {
   maxbits = BITS;
   n_bits  = INIT_BITS;

   maxcode    = (1 << n_bits) - 1;
   maxmaxcode = (1 << maxbits) - 1;
   }
