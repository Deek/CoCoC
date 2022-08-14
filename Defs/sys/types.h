#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

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

typedef _BYTE nlink_t;	/* file descriptors: # of links */

typedef _BYTE pid_t;	/* process ID */
typedef unsigned uid_t;	/* user ID (we don't have groups) */
typedef unsigned id_t;	/* Object big enough to hold any "id" */

typedef _BYTE mode_t;	/* Filesystem attributes, dsewrewr */
typedef _DWORD ino_t;	/* filesystem sector number (we don't have inodes) */
typedef long off_t;		/* file sizes, locations, etc. */

#ifndef _CLOCK_T_
# define _CLOCK_T_
typedef unsigned clock_t;
#endif
#ifndef _TIME_T_
# define _TIME_T_
# if defined (__STDC__) || defined (__DCC__)
typedef unsigned long time_t;
# else
typedef long time_t;
# endif
#endif

#ifndef _SIZE_T_
#define _SIZE_T_
typedef unsigned	size_t;
typedef int			ssize_t;
#endif

#endif /* _SYS_TYPES_H_ */
