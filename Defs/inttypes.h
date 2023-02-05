#ifndef _INTTYPES_H_
#define _INTTYPES_H_
#if (!defined(__STDC__) && !defined(__DCC__))
# error "<inttypes.h> is not compatible with Microware C."
#endif
#ifndef _STDINT_H_
# include <stdint.h>
#endif

/* printing signed integers */
#define PRId8	"d"
#define PRId16	"d"
#define PRId32	"ld"
#define PRIdMAX	"ld"
#define PRIdPTR	"d"
#define PRIi8	"i"
#define PRIi16	"i"
#define PRIi32	"li"
#define PRIiMAX	"li"
#define PRIiPTR	"i"

/* printing unsigned integers */
#define PRIo8	"o"	/* octal */
#define PRIo16	"o"
#define PRIo32	"lo"
#define PRIoMAX	"lo"
#define PRIoPTR	"o"
#define PRIu8	"u"	/* decimal */
#define PRIu16	"u"
#define PRIu32	"lu"
#define PRIuMAX	"lu"
#define PRIuPTR	"u"
#define PRIx8	"x"	/* hex */
#define PRIx16	"x"
#define PRIx32	"lx"
#define PRIxMAX	"lx"
#define PRIxPTR	"x"
#define PRIX8	"X"
#define PRIX16	"X"
#define PRIX32	"lX"
#define PRIXMAX	"lX"
#define PRIXPTR	"X"

/* scanning signed integers */
#define SCNd16	"d"
#define SCNd32	"D"
#define SCNdMAX	"D"
#define SCNdPTR	"d"
#define SCNi16	"d"
#define SCNi32	"D"
#define SCNiMAX	"D"
#define SCNiPTR	"d"

/* scanning unsigned integers */
#define SCNo16	"o"	/* octal */
#define SCNo32	"O"
#define SCNoMAX	"O"
#define SCNoPTR	"o"
#define SCNu16	"u"	/* decimal */
#define SCNu32	"U"
#define SCNuMAX	"U"
#define SCNuPTR	"u"
#define SCNx16	"x"	/* hex */
#define SCNx32	"X"
#define SCNxMAX	"X"
#define SCNxPTR	"x"

/* #define imaxdiv_t ldiv_t */

#endif /* _INTTYPES_H_ */