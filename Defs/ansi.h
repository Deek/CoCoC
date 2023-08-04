#ifndef _ANSI_H_
#define _ANSI_H_
#if defined(__STDC__) || defined(_CMOC_VERSION_)
# ifndef _OP
#  define _OP(x) x
# endif
# define _VOID		void
# define _VOIDPTR	void *
#else /* ! __STDC__ */
# ifndef _OP
#  define _OP(x) ()
# endif
# define _VOID
# define _VOIDPTR	char *
# define const
# define restrict
# define volatile
#endif /* ! __STDC__ */
#endif /* _ANSI_H_ */
