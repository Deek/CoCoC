#ifndef _TYPES_H_
#define _TYPES_H_

/* OSK/OS-9000 system type names */

/* The old compiler doesn't have unsigned char/long, fake it */
#ifndef _BYTE
# if defined(__STDC__) || defined(__DCC__)
#  define _BYTE unsigned char
# else
#  define _BYTE char
# endif
#endif

#ifndef _DWORD
# if defined(__STDC__) || defined(__DCC__)
#  define _DWORD unsigned long
# else
#  define _DWORD long
# endif
#endif

#ifndef _USHORT
# if defined(__STDC__) || defined(__DCC__)
#  define _USHORT unsigned short
# else
#  define _USHORT unsigned
# endif
#endif

typedef _BYTE		u_char;
typedef _USHORT		u_short;
typedef unsigned	u_int;
typedef _DWORD		u_long;
typedef _BYTE		u_int8;
typedef unsigned	u_int16;
typedef _DWORD		u_int32;

typedef char	int8;
typedef int		int16;
typedef long	int32;

typedef u_char	process_id;
typedef u_int	signal_code;
typedef u_int	path_id;
typedef u_int	error_code;
typedef u_char	prior_level;

#endif /* _TYPES_H_ */
