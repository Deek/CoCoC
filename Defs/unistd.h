#ifndef	_UNISTD_H_
#define	_UNISTD_H_

#include <ansi.h>
#include <sys/types.h>
/*
 * UNIX-type system calls and functions implemented
 */
#ifndef STDIN_FILENO
# define STDIN_FILENO	(0)	/* Standard input */
# define STDOUT_FILENO	(1)	/* Standard output */
# define STDERR_FILENO	(2)	/* Standard error output */
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
# define alarm(a)			__NIMPL__(alarm)
# define chroot(a)			__NIMPL__(chroot)
# define dup2(a,b)			__NIMPL__(dup2)
# define execl(a,b,c)		__NIMPL__(execl)
# define execle(a,b,c)		__NIMPL__(execle)
# define execlp(a,b,c)		__NIMPL__(execlp)
# define execv(a,b)			__NIMPL__(execv)
# define execve(a,b,c)		__NIMPL__(execve)
# define execvp(a,b)		__NIMPL__(execvp)
# define execvpe(a,b,c)		__NIMPL__(execvpe)
# define fdatasync(a)		__NIMPL__(fdatasync)
# define fork()				__NIMPL__(fork)
# define fpathconf(a,b)		__NIMPL__(fpathconf)
# define fsync(a)			__NIMPL__(fsync)
# define ftruncate(a,b)		__NIMPL__(ftruncate)
# define getegid()			__NIMPL__(getegid)
# define getgid()			__NIMPL__(getgid)
# define getgroups(a,b)		__NIMPL__(getgroups)
# define getlogin()			__NIMPL__(getlogin)
# define getlogin_r(a,b)	__NIMPL__(getlogin_r)
# define getpass(a)			__NIMPL__(getpass)
# define getpgid(a)			__NIMPL__(getpgid)
# define getpgrp()			__NIMPL__(getppid)
# define getppid()			__NIMPL__(getppid)
# define getsid(a)			__NIMPL__(getsid)
# define getwd(a)			__NIMPL__(getwd)
# define link(a,b)			__NIMPL__(link)
# define nice(a)			__NIMPL__(nice)
# define pathconf(a,b)		__NIMPL__(pathconf)
# define pipe(a)			__NIMPL__(pipe)
# define rmdir(a)			__NIMPL__(rmdir)
# define setgid(a)			__NIMPL__(setgid)
# define setpgid(a,b)		__NIMPL__(setpgid)
# define setpgrp(a,b)		__NIMPL__(setpgrp)
# define setsid()			__NIMPL__(setsid)
# define sysconf(a)			__NIMPL__(sysconf)
# define tcgetpgrp(a)		__NIMPL__(tcgetpgrp)
# define tcsetpgrp(a,b)		__NIMPL__(tcsetpgrp)
# define ttyname()			__NIMPL__(ttyname)
# define ttyname_r(a,b)		__NIMPL__(ttyname_r)
# define usleep(a)			__NIMPL__(usleep)
#endif

/*
 * UNIX constants for access()
 *
 * These are not the same values as on Unix!
 */
#define F_OK	(0)		/* Path exists */
#define R_OK	(1)		/* Path is readable */
#define W_OK	(2)		/* Path is writeable */
#define X_OK	(4)		/* Path is executable / searchable */

int access _OP((const char *pathname, int mode));

/* Standard constants for lseek/fseek */
#ifndef SEEK_SET
# define SEEK_SET	(0)	/* offset from file start */
# define SEEK_CUR	(1)	/* offset is from current file pointer */
# define SEEK_END	(2)	/* offset is from end of file */
#endif

/* Old BSD constants for same */
#define L_SET	SEEK_SET
#define L_INCR	SEEK_CUR
#define L_XTND	SEEK_END

off_t lseek _OP((int fd, off_t offset, int whence));

int close _OP((int fd));

ssize_t read _OP((int fd, _VOID *buf, size_t count));
ssize_t write _OP((int fd, _VOID *buf, size_t count));

clock_t sleep _OP((unsigned seconds));

int pause _OP((_VOID));

int chown _OP((const char *pathname, uid_t owner, gid_t group));
int chdir _OP((const char *pathname));
int chxdir _OP((const char *pathname));

char *getcwd _OP((char *buf, size_t size));

int dup _OP((int oldfd));

_VOID _exit _OP((int status));

pid_t getpid _OP((_VOID));

uid_t getuid _OP((_VOID));
#define geteuid() getuid()

int setuid _OP((uid_t uid));
int isatty _OP((int fd));

int unlink _OP((const char *pathname));

#ifndef _GETOPT_H_
#define _GETOPT_H_
extern char	*optarg;
extern int	opterr, optind, optopt;

int getopt _OP((int argc, char * const argv[], const char *optstring));
#endif /* _GETOPT_H_ */

_VOID sync _OP((_VOID));
int brk _OP((intptr_t size));
_VOIDPTR sbrk _OP((intptr_t increment));
_VOID swab _OP((const _VOIDPTR from, _VOIDPTR to, ssize_t n));

int asetuid _OP((uid_t uid));
int chmod _OP((char *pathname, mode_t mode));

/*
int os9fork _OP((char *, int, char *, short, short, int));
clock_t tsleep _OP((clock_t ticks));
*/

#endif	/* _UNISTD_H_ */
