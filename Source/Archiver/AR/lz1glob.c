/* compressglob -- globals for compress */

#include "lz1.h"

WORD        maxbits,
            n_bits,
            maxmaxcode,
            maxcode,
            free_ent,
            offset,
            stats_flg,
            verbose,
            debug;

long        lz_bytes;
UWORD       buf[BITS];
CodeTable   Code[1 << BITS];
