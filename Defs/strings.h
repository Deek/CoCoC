#ifndef _STRINGS_H_
#define _STRINGS_H_

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
# define bcmp(s,n)		__NIMPL__(bcmp)
# define bcopy(s,d,n)	__NIMPL__(bcopy)
# define bzero(s,n)		__NIMPL__(bzero)
# define ffs(i)			__NIMPL__(ffs)
# define ffsl(i)		__NIMPL__(ffsl)
#endif	/* MISSING_CHECK */

/* Find the first occurrence of c in s */
char *index _OP((const char *s, int c));

/* Find the last occurrence of c in s */
char *rindex _OP((const char *s, int c));

/* Compare S1 and S2 in uppercase */
int strucmp _OP((const char *s1, const char *s2));
/* Compare N characters of S1 and S2 in uppercase */
int strnucmp _OP((const char *s1, const char *s2, size_t n));

#ifndef stricmp
# define stricmp strucmp
# define strnicmp strnucmp
#endif

#ifndef strcasecmp
# define strcasecmp strucmp
# define strncasecmp strnucmp
#endif

/* Return a pointer to the end of S */
char *strend _OP((const char *s));

/* Copy Src to Dest, converting from high-bit termination */
char *strhcpy _OP((char *dest, const char *src));

/* Return nonzero if P is a prefix of S */
int strpfx _OP((const char *s, const char *p));

/* Copy Src to Dest, transforming to uppercase */
char *strucpy _OP((char *dest, const char *src));

/* Append Src onto Dest, transforming to uppercase */
char *strucat _OP((char *dest, const char *src));

/* Overwrite up to N characters with spaces, not counting null terminator */
char *strclr _OP((char *s, size_t n));

/* Reverse the order of characters in S, then return it */
char *reverse _OP((char *s));

/* Compare 'wildcard' Pattern against S, case-insensitively if Flag */
int patmatch _OP((const char *pattern, const char *s, int flag));

/* Return position of Substr in S */
int findstr _OP((int pos, const char *s, const char *substr));
int findnstr _OP((int pos, const char *s, const char *substr, size_t size));

#endif /* _STRINGS_H_ */
