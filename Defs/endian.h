#ifndef	_ENDIAN_H_
#define	_ENDIAN_H_

#define LITTLE_ENDIAN	1234
#define BIG_ENDIAN		4321
#define PDP_ENDIAN		2143

#define BYTE_ORDER		BIG_ENDIAN

/* This is used by OS9000 today -- _BIG_END and _LIL_END */
#ifndef _BIG_END
# define _BIG_END	1
#endif
#undef _LIL_END

#endif	/* _ENDIAN_H_ */
