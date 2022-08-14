#ifndef _LIMITS_H_
#define _LIMITS_H_

#define ARG_MAX		2048

/* Maximum number of child processes "per user' (total) */
#define CHILD_MAX	31

/* Maximum length of a file */
#define FILESIZEBITS 32

/* Maximum number of hard links to a file */
#define LINK_MAX	255

/* Maximum length of a file name */
#ifndef NAME_MAX
# define NAME_MAX	29
#endif

/* Size of a memory page, in bytes */
#define PAGESIZE	256

/* Maximum number of open files, fopen'ed files, streams */
#define OPEN_MAX	16
#define FOPEN_MAX	OPEN_MAX
#define STREAM_MAX	OPEN_MAX

/* Maximum number of timers per process */
#define TIMER_MAX	1

/* Max length of a tty name */
#define TTY_NAME_MAX	NAME_MAX

/* "runtime increasable values" (not really) */
#define LINE_MAX	256		/* maximum "guaranteed" length of a line */

/* Numerical limits */
#define CHAR_BIT	8
#define SCHAR_MIN	-128
#define SCHAR_MAX	127
#define UCHAR_MAX	255
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX

#define WORD_BIT	16
#define SHRT_MIN	(-32767-1)
#define SHRT_MAX	32767
#define USHRT_MAX	65535
#define INT_MIN		(-32767-1)
#define INT_MAX		32767
#define UINT_MAX	65535

#define LONG_BIT	32
#define LONG_MIN	(-2147483647L-1)
#define LONG_MAX	2147483647L
#if defined(__STDC__) || defined(__DCC__)
#define ULONG_MAX	(4294967295L)
#endif

/* Maximum value of "digit" in calls to printf() and scanf() */
#define NL_ARGMAX	9

/* Default process priority */
#define NZERO	128

#endif /* _LIMITS_H_ */
