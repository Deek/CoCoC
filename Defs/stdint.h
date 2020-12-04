#ifndef _STDINT_H_
#define _STDINT_H_

/*
	Type definitions for integral types with fixed or minimum sizes

	Note: ISO C requires a number of types that we don't have, chiefly the
	unsigned types (K&R C only has one, "unsigned"). Unfortunately, there
	isn't much we can do about that short of a new type system (someday...)
*/

typedef char		int8_t;
typedef short		int16_t;
typedef long		int32_t;
typedef unsigned	uint16_t;

#if 0
/*
	Fastest minimum width integer types
*/
typedef int			int_fast8_t;
typedef int			int_fast16_t;
typedef long		int_fast32_t;
typedef unsigned	uint_fast8_t;
typedef unsigned	uint_fast16_t;

/*
	Minimum width integer types
*/
typedef char		int_least8_t;
typedef int			int_least16_t;
typedef long		int_least32_t;
typedef unsigned	uint_least8_t;
typedef unsigned	uint_least16_t;

#define	UINT_FAST16_MAX	UINT16_MAX
#define	UINT_LEAST8_MAX	UINT16_MAX
#define	UINT_LEAST16_MAX UINT16_MAX

#define	INT_FAST8_MIN	INT16_MIN
#define	INT_FAST16_MIN	INT16_MIN
#define	INT_FAST32_MIN	INT32_MIN

#define	INT_FAST8_MAX	INT16_MAX
#define	INT_FAST16_MAX	INT16_MAX
#define	INT_FAST32_MAX	INT32_MAX

#define	INT_LEAST16_MAX	(INT16_MAX)
#define	INT_LEAST16_MIN	(INT16_MIN)
#define	INT_LEAST8_MAX	(INT8_MAX)
#define	INT_LEAST8_MIN	(INT8_MIN)
#endif

/*
	Pointer-sized integer types
*/
typedef int			intptr_t;
typedef unsigned	uintptr_t;

/*
	Maximum sized integer types
*/
typedef long		intmax_t;
typedef unsigned	uintmax_t;

/*
	Macros for integral type limits
*/
#define	INT8_MIN		(-128)
#define	INT16_MIN		(-32767-1)
#define	INT32_MIN		(-2147483647-1)

#define	INT8_MAX		(127)
#define	INT16_MAX		(32767)
#define	INT32_MAX		(2147483647L)

#define	UINT16_MAX		(65535)

/*
	Macros for dealing with integers
*/
#define INT8_C(val)		(val)
#define INT16_C(val)	(val)
#define INT32_C(val)	(val ## L)
#define INTMAX_C		INT32_C

#define UINT8_C(val)	(val & 0xFF)
#define UINT16_C(val)	(val)

#endif /* _STDINT_H_ */
