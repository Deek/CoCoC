#ifndef _FCNTL_H_
#define _FCNTL_H_
/*
	fcntl.h - definitions for open() and creat() system calls

	OS-9 does not have a fcntl() call, but POSIX says that the flags for
	open() are declared here, so for ease in porting... :)

	The open() system call is not totally equivalent to Unix open(). It
	can't create files, or open a file for appending, or truncate a file
	on open. For those, you need to use additional system calls (see
	lseek(), _ss_size())

	The I$Create system call is used to create and open files.
*/
#ifndef _ANSI_H_
# include <ansi.h>
#endif
/*
	This macro causes allows missing functions to generate compiler
	errors instead of silently appearing to work until link time.

	To use it, define the symbol MISSING_CHECK in your source or in the
	command line you use to compile.
*/
#ifdef MISSING_CHECK
# ifndef __NIMPL__
#  define __NIMPL__(__FUNCNAME__) 0="__FUNCNAME__() missing"
# endif
# define fcntl(a,b)	__NIMPL__(fcntl)
# define flock(a,b)	__NIMPL__(flock)
#endif

/* Non- or semi-standard symbols */
#define O_PATH	0	/* Get a fd, but can't read or write */
#define O_READ	1	/* read mode available */
#define O_WRITE	2	/* write mode available */
#define O_EXEC	4	/* open for exec (uses exec directory) */

/* Standard symbols */
#define O_RDONLY	1
#define O_WRONLY	2
#define O_RDWR		3

/* masks off all but read/write */
#define O_ACCMODE	O_RDWR

/* Needed to open() a directory file. Causes creat() to fail. */
#define O_DIRECTORY	128

/* These modes are incompatible and not supported. */
#undef O_APPEND
#undef O_CREATE
#undef O_TRUNC
#undef O_EXCL

/* Unix-style nonblocking I/O not currently supported. */
#undef O_NONBLOCK
#undef O_NDELAY

/* These Unix modes are harmless, so we can put them in. */
#define O_DSYNC		0
#define O_NOCTTY	0
#define O_NOFOLLOW	0
#define O_SYNC		0

int open _OP((const char *pathname, int flags));
int creat _OP((const char *pathname, int mode));

#endif	/* _FCNTL_H_ */
