#ifndef _STRING_H_
#define _STRING_H_

#ifndef _ANSI_H_
# include <ansi.h>
#endif
#ifndef _SIZE_T_
# define _SIZE_T_
typedef unsigned	size_t;
typedef int			ssize_t;
#endif

/*
 * This macro causes missing functions to generate compiler errors instead
 * of silently appearing to work until link time. To use it, define the symbol
 * MISSING_CHECK in your source or in the command line you use to compile.
 */
#ifdef MISSING_CHECK
# ifndef __NIMPL__
#  define __NIMPL__(__FUNCNAME__) (0=#__FUNCNAME__ "() missing")
# endif
# define memrchr(s,c,n)	__NIMPL__(memrchr)
# define strcoll(s1,s2)	__NIMPL__(strcoll)
# define strdup(s)		__NIMPL__(strdup)
# define strdupa(s)		__NIMPL__(strdupa)
# define strerror(n)	__NIMPL__(strerror)
# define strndup(s,n)	__NIMPL__(strndup)
# define strndupa(s,n)	__NIMPL__(strndupa)
# define strnlen(s,n)	__NIMPL__(strnlen)
# define strsep(s,n)	__NIMPL__(strsep)
# define strstr(h,n)	__NIMPL__(strstr)
# define strxfrm(d,s,n)	__NIMPL__(strxfrm)
#endif	/* MISSING_CHECK */

/* Copy N bytes from Src to Dest */
char *memcpy _OP((_VOIDPTR dest, const _VOIDPTR src, size_t n));

/* Copy up to N bytes from Src to Dest unless C found */
char *memccpy _OP((_VOIDPTR dest, const _VOIDPTR src, int c, size_t n));

/* Set N bytes of S to C */
_VOIDPTR memset _OP((const _VOIDPTR s, int c, size_t n));

/* Compare N bytes of S1 and S2 */
int memcmp _OP((const _VOIDPTR s1, const _VOIDPTR s2, size_t n));

/* Search N bytes of S for C */
_VOIDPTR memchr _OP((const _VOIDPTR s, int c, size_t n));

/* Copy Src to Dest */
char *strcpy _OP((char *dest, const char *src));

/* Copy no more than N chars of Src to Dest */
char *strncpy _OP((char *dest, const char *src, size_t n));

/* Append Src onto Dest */
char *strcat _OP((char *dest, const char *src));

/* Append no more than N chars of Src onto Dest */
char *strncat _OP((char *dest, const char *src, size_t n));

/* Compare S1 and S2 */
int strcmp _OP((const char *s1, const char *s2));

/* Compare N characters of S1 and S2 */
int strncmp _OP((const char *s1, const char *s2, size_t n));

/* Find the first occurrence of C in S */
char *strchr _OP((const char *s, int c));

/* Find the last occurrence of C in S */
char *strrchr _OP((const char *s, int c));

/* Return length of initial segment of S composed of chars NOT in Reject */
size_t strcspn _OP((const char *s, const char *reject));

/* Return length of initial segment of S composed of chars in Accept */
size_t strspn _OP((const char *s, const char *accept));

/* Find first occurrence in S of any char in Accept */
char *strpbrk _OP((const char *s, const char *accept));

/* Divide S into tokens separated by chars in Delim */
char *strtok _OP((char *s, const char *delim));

/* Return the length of S */
size_t strlen _OP((const char *s));

#ifndef _STRINGS_H_
/* get nonstandard string functions */
# include <strings.h>
#endif

#endif /* _STRING_H_ */
