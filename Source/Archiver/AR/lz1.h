/*
 * header file for LZ compression routines
 */
#ifndef WORD
# ifdef  OSK
#  define WORD short
#else
#  define WORD int
# endif
#endif

#define BITS            11           /* maximum number of bits/code */
#define INIT_BITS       9            /* initial number of bits/code */
/*
 * One code can represent 1 << BITS characters, but to get a code of
 * length N requires an input string of at least N * (N - 1) / 2
 * characters. To overflow the decompress stack, an input file would
 * have to have at least MAXSTACK * (MAXSTACK - 1) / 2 consecutive
 * occurrences of a particular character, which is unlikely for the
 * value used here. (Do keep the initial advice in mind, though.)
 */
#define MAXSTACK        2000                /* size of output stack */

#define TAG             2995               /* suggested by M. Meyer */

/*
 * The following should be changed to fit your machine and the type
 * you choose for the elements of the array buf.
 * (If you avoid insert_bit() and fetch(), never mind.)
 */
#define LOG2WSIZE       4         /* log2(size of base type of buf) */
#define WSIZE           16              /* size of base type of buf */

#define BytesToBits(b)  ((b) << 3)
#define LowOrder(n)     (~(~0 << (n)))           /* thanks to K & R */
#define HighOrder(n)    (~0 << (n))

typedef struct {
     unsigned WORD   next,     /* chain of entries with same prefix */
                     chain,       /* chain prefixed with this entry */
                     prefix,          /* prefix code for this entry */
                     suffix;             /* last char in this entry */
        } CodeTable;

extern WORD       maxbits,         /* user settable max # bits/code */
                  n_bits,            /* initial number of bits/code */
                  maxmaxcode,      /* max permissible maxcode value */
                                            /* (i.e. 2 ** BITS - 1) */
                  maxcode,                       /* 2 ** n_bits - 1 */
                  free_ent,                   /* first unused entry */
                  offset,        /* cursor into buf (units of bits) */
                  stats_flg;         /* should we print statistics? */

extern WORD       verbose,
                  debug;

extern long       lz_bytes;
extern WORD       buf[BITS];
extern CodeTable  Code[1 << BITS];
