#ifndef _DIRENT_H_
#define _DIRENT_H_

#ifndef _ANSI_H_
# include <ansi.h>
#endif

#ifndef _SYS_TYPES_H_
# include <sys/types.h>
#endif

/* Maximum size of a directory entry (same as <limits.h>) */
#ifndef NAME_MAX
# define NAME_MAX	29
#endif

#define DIRSIZ	(sizeof (struct dirent))

/* Standard directory entry structure returned by readdir() */
struct dirent {
	ino_t	d_ino;				/* file descriptor address (no real inodes) */
	char	d_name[NAME_MAX+1];	/* directory entry name    */
};
#define DIRENT	struct dirent

/* "opaque" type used to hold the state of an open directory */
typedef struct _dirstm {
	int		dd_fd;				/* fd for open directory   */
	char	dd_buf[32];			/* a one entry buffer      */
} DIR;

/*
 * These macros get info out of a 'struct dirent *'. Don't play any games with
 * pointer expressions, because these macros can evaluate their argument more
 * than once.
 *
 * _D_EXACT_NAMLEN (DP) returns the length of DP->d_name, not including the
 * terminating zero byte.
 *
 * _D_ALLOC_NAMLEN (DP) returns a size, at least _D_EXACT_NAMLEN(DP)+1 bytes,
 * representing the allocation size needed to duplicate the name string.
 */
#define _D_EXACT_NAMLEN(d) (strlen ((d)->d_name))
#define _D_ALLOC_NAMLEN(d) (sizeof ((d)->d_name) > 1 ? sizeof ((d)->d_name) : _D_EXACT_NAMLEN(d) + 1)

extern DIR *opendir _OP((const char *name));

extern struct dirent *readdir _OP((DIR *dirp));

extern long telldir _OP((DIR *dirp));
extern VOID rewinddir _OP((DIR *dirp));
extern VOID seekdir _OP((DIR *dirp, long loc));
extern int closedir _OP((DIR *dirp));

#define rewinddir(a)	seekdir((a), 0L)

#endif /* _DIRENT_H_ */
