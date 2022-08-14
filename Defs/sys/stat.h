#ifndef _SYS_STAT_H_
#define _SYS_STAT_H_
/*
	data returned by the stat() function
 */
#ifndef _ANSI_H_
# include <ansi.h>
#endif
#ifndef _SYS_TYPES_H_
# include <sys/types.h>
#endif

#ifdef MISSING_CHECK
# ifndef __NIMPL__
#  define __NIMPL__(__FUNCNAME__) 0="__FUNCNAME__() missing"
# endif
# define fchmod(a,b)	__NIMPL__(fchmod)
# define fchown(a,b)	__NIMPL__(fchown)
# define fstat(a,b)		__NIMPL__(fstat)
# define lstat(a,b)		__NIMPL__(lstat)
# define mkfifo(a,b)	__NIMPL__(mkfifo)
# define umask(a)		__NIMPL__(umask)
#endif

union _fstime {
	struct {
		char	t_year,
				t_month,
				t_day,
				t_hour,
				t_minute;
	};
	char	t_bytes[5];
};

union _fsdate {
	struct {
		char	t_year,
				t_month,
				t_day;
	};
	char	t_bytes[3];
};

struct stat {
	mode_t			st_mode;	/* attributes */
	uid_t			st_uid;		/* owner */
	union _fstime	st_mtim;	/* modification time */
	nlink_t			st_nlink;	/* link count */
	off_t			st_size;	/* file size */
	union _fsdate	st_cdat;	/* creation time */
};

#define S_IFMT		0200
#define S_IPRM		0077

#define S_IFBLK		0077		/* these are impossible format masks */
#define S_IFCHR		0077
#define S_IFIFO		0077
#define S_IFLNK		0077
#define S_IFSOCK	0077

#define S_IFREG		0000		/* no bits set in format mask */
#define S_IFDIR		0200
#define S_ISHARE	0100

/* S_ISREG basically means "isn't a directory" here... */
#define S_ISDIR(mode)	(((mode) & S_IFMT) == S_IFDIR)
#define S_ISREG(mode)	(((mode) & S_IFMT) == S_IFREG)

#define S_ISBLK(mode)	0
#define S_ISCHR(mode)	0
#define S_ISFIFO(mode)	0
#define S_ISLNK(mode)	0
#define S_ISSOCK(mode)	0

#define __S_ISTYPE(mode, mask)  (((mode) & __S_IFMT) == (mask))

#define S_ISUID		0			/* Set user ID on execution.  */
#define S_ISGID		0			/* Set group ID on execution.  */

/* Protection bits.  */

#define S_IREAD		001
#define S_IWRITE	002
#define S_IEXEC		004

#define S_IOREAD	010
#define S_IOWRITE	020
#define S_IOEXEC	040

#define S_IRUSR		S_IREAD		/* Read by owner.  */
#define S_IWUSR		S_IWRITE	/* Write by owner.  */
#define S_IXUSR		S_IEXEC		/* Execute by owner.  */
#define S_IRWXU		(S_IRUSR|S_IWUSR|S_IXUSR)

#define S_IRGRP		0			/* Read by group.  */
#define S_IWGRP		0			/* Write by group.  */
#define S_IXGRP		0			/* Execute by group.  */
#define S_IRWXG		0

#define S_IROTH		S_IOREAD	/* Read by others.  */
#define S_IWOTH		S_IOWRITE	/* Write by others.  */
#define S_IXOTH		S_IOEXEC	/* Execute by others.  */
#define S_IRWXO		(S_IROTH|S_IWOTH|S_IXOTH)

/* Macros for common mode bit masks.  */
# define ACCESSPERMS (S_IRWXU|S_IRWXG|S_IRWXO) /* 0777 */
# define ALLPERMS (S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO)/* 07777 */
# define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)/* 0666*/

/*
	The OS-9 standard library does not have the mkdir() function. It has a
	compatible function called mknod(), which will at some point be exported
	under both names. Until then, we can define mkdir to call mknod
*/
#define mkdir(path,mode) mknod((path),(mode)&S_IPRM)
/*
	This function is not strictly compatible with UNIX mknod(); the original
	Unix mknod() was capable of creating any possible type of file, including
	device special files and directories, and it takes a third argument
	specifying the attributes of a device special file, provided that 
	mode & (S_IFCHR|S_IFBLK) is nonzero. None of this applies to RBF disks, 
	because OS-9 mknod() only creates directories (not even regular files).
*/
int mknod _OP((const char *pathname, mode_t mode /*, dev_t dev */));

int stat _OP((const char *pathname, struct stat *statbuf));

int chmod _OP((const char *pathname, mode_t mode));

int chown _OP((const char *pathname, uid_t owner /*, gid_t group */));

#endif /* _SYS_STAT_H_ */
