#ifndef _STDLIB_H_
#define _STDLIB_H_

#ifndef _ANSI_H_
#include <ansi.h>
#endif

#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1
#ifndef NULL
# define NULL (0)
#endif

#ifndef _SIZE_T_
# define _SIZE_T_
typedef unsigned	size_t;
typedef int			ssize_t;
#endif

_VOID abort _OP((_VOID));

int abs _OP((int i));

double atof _OP((const char *nptr));
int atoi _OP((const char *nptr));
long atol _OP((const char *nptr));

_VOIDPTR bsearch _OP((const _VOIDPTR key, const _VOIDPTR base, size_t nmemb, size_t size, int (*compar)(const _VOIDPTR, const _VOIDPTR)));

char *itoa _OP((int val, char *s, int radix));
char *ltoa _OP((long val, char *s, int radix));
char *utoa _OP((unsigned val, char *s, int radix));

int htoi _OP((const char *nptr));
long htol _OP((const char *nptr));

int max _OP((int i1, int i2));
int min _OP((int i1, int i2));

unsigned umin _OP((unsigned u1, unsigned u2));
unsigned umax _OP((unsigned u1, unsigned u2));

_VOID l3tol _OP((long *lp, const char *cp, int n));
_VOID ltol3 _OP((char *cp, const long *lp, int n));

_VOIDPTR ibrk _OP((size_t size));

_VOIDPTR calloc _OP((size_t nel, size_t size));
_VOIDPTR malloc _OP((size_t size));
_VOIDPTR realloc _OP((_VOIDPTR ptr, size_t size));
_VOID free _OP((_VOIDPTR ptr));

#endif	/* _STDLIB_H_ */
