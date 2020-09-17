#ifndef _STDDEF_H_
#define _STDDEF_H_
/*
 *	stddef.h - standard type definitions
 */
#ifndef NULL
# define NULL (0)
#endif

#ifndef _SIZE_T_
# define _SIZE_T_
typedef unsigned	size_t;
typedef int			ssize_t;
#endif

#ifndef _PTRDIFF_T_
# define _PTRDIFF_T_
typedef int			ptrdiff_t;
#endif

/*
 *	offsetof(type, field) gives the offset, in bytes, of a struct or union 
 *	member from a pointer of that struct or union type.
 */
#ifndef offsetof
# define offsetof(T,F) ((size_t)((char *)&((T *)0)->F - (char *)0))
#endif
#endif /* _STDDEF_H_ */
