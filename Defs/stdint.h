#ifndef _STDINT_H_
#define _STDINT_H_

/*
	Type definitions for integral types with fixed or minimum sizes

	Note: ISO C requires a number of types that we don't have, chiefly the
	unsigned types (K&R C only has one, "unsigned"). Unfortunately, there
	isn't much we can do about that short of a new type system (someday...)
*/
#if defined(__STDC__) || defined(__DCC__)
typedef signed char		int8_t;
typedef signed int		int16_t;
typedef signed long		int32_t;
typedef unsigned char	uint8_t;
typedef unsigned int	uint16_t;
typedef unsigned long	uint32_t;
#else
typedef char			int8_t;
typedef short int		int16_t;
typedef long			int32_t;
typedef unsigned int	uint16_t;
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
#if defined(__STDC__) || defined(__DCC__)
typedef unsigned long	uintmax_t;
#else
typedef unsigned	uintmax_t;
#endif

/*
	Macros for integral type limits
*/
#define	INT8_MIN		(-128)
#define	INT16_MIN		(-32767-1)
#define	INT32_MIN		(-2147483647-1)

#define	INT8_MAX		(127)
#define	INT16_MAX		(32767)
#define	INT32_MAX		(2147483647L)

#define	UINT8_MAX		(255)
#define	UINT16_MAX		(65535)
#if defined(__STDC__) || defined(__DCC__)
#define	UINT32_MAX		(4294967295L)
#endif

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
