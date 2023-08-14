#ifndef _TIME_H_
#define _TIME_H_

#ifndef _ANSI_H_
# include <ansi.h>
#endif

#ifndef NULL
# define NULL	0
#endif

/*
 *	clock_t is a C standard representing a count of clock ticks.
 */
#ifndef _CLOCK_T_
# define _CLOCK_T_
typedef unsigned clock_t;
#endif

/*
 *	time_t is a C standard, but C doesn't require any particular
 *	form or format. Our time_t currently represents seconds since the Unix
 *	epoch (1/1/1970 00:00:00 UTC). We declare time_t to be unsigned because
 *	that makes it representable until 7 Feb 2106, instead of 19 Jan 2038.
 *	This beats using an unsigned 1900 epoch though, that one rolls over
 *	on 7 Feb 2036.
 */
#ifndef _TIME_T_
# define _TIME_T_
# if defined (__STDC__) || defined (__DCC__)
typedef unsigned long time_t;
# else
typedef long time_t;
# endif
#endif

#define EPOCH_YEAR		1970	/* The Unix epoch started 1/1/1970 00:00:00 */
#define EPOCH_DOW		4		/* It was a Thursday */
#define SECS_PER_DAY	86400L
#define SECS_PER_HOUR	3600
#define SECS_PER_MIN	60

#define TM_YEAR_BASE	1900
#define FIRST_GOOD_YEAR	EPOCH_YEAR	/* First full year we can represent */
#define LAST_GOOD_YEAR	EPOCH_YEAR+135	/* Last full year in our time_t */

/*
 *	This calls a fast internal function that determines and returns the
 *	number of clock ticks per second.
 */
#define CLOCKS_PER_SEC _stps()
#define tps _stps()	/* This is an old OS-9-specific name for ticks/second. */

clock_t _stps _OP((void));

/* OS-9 doesn't know/care how much CPU time has been used */
#define clock() ((clock_t)-1)

/*
 *	This macro causes missing functions to generate compiler errors instead
 *	of silently appearing to work until link time. To use it, define the
 *	symbol MISSING_CHECK in your source or in the command line you use to
 *	compile.
 */
#ifdef MISSING_CHECK
# ifndef __NIMPL__
#  define __NIMPL__(__FUNCNAME__) (0=#__FUNCNAME__ "() missing")
# endif
# define strftime(d,m,f,t) __NIMPL__(strftime)
# define strptime(b,f,t)   __NIMPL__(strptime)
#endif	/* MISSING_CHECK */

/* This structure is used by the 'setime()' and 'getime()' calls */
#ifndef _BYTE
# if defined(__STDC__) || defined(__DCC__)
#  define _BYTE unsigned char
# else
#  define _BYTE char
# endif
#endif
struct sgtbuf {
	_BYTE	t_year,		/* year - 1900 */
			t_month,	/* month (1..12) */
			t_day,		/* day of month (1..31) */
			t_hour,		/* hour (0..23) */
			t_minute,	/* minutes (0..59) */
			t_second;	/* seconds (0..59) */
};
int getime _OP((struct sgtbuf *timebuf));
int setime _OP((struct sgtbuf *timebuf));

/*
 *	struct tm, also known as "broken down time", is also a C standard, but
 *	it's a lot more specified. For example, a minute may contain up to 61
 *	seconds in it, to account for leap seconds. Unofficially, this will
 *	probably cause trouble, so maybe don't do it.
 */
#ifndef _STRUCT_TM_
# define _STRUCT_TM_
struct tm {
	int	tm_sec;		/* seconds (0 - 61*) */
	int	tm_min;		/* minutes (0 - 59) */
	int	tm_hour;	/* hours (0 - 23) */
	int	tm_mday;	/* day of month (1 - 31) */
	int	tm_mon;		/* month of year (0 - 11) */
	int	tm_year;	/* years since 1900 */
	int	tm_wday;	/* day of week (Sunday = 0) */
	int	tm_yday;	/* day of year (0 - 365) */
	int	tm_isdst;	/* daylight saving time active */
};
#endif

extern int daylight;		/* nonzero if DST is ever in use */
extern long int timezone;	/* signed offset from GMT in seconds */

/* Return the current time and put it in timer if !NULL */
time_t time _OP((time_t *timer));

/* Return the difference between time1 and time0 */
long difftime _OP((time_t time1, time_t time0));
#define difftime(t1,t0) ((long) ((long)(t1) - (long)(t0)))

/* Return the 'struct tm' rep of *timer in "the local time zone" */
struct tm *localtime _OP((const time_t *timer));

/* Return the 'struct tm' rep of *timer in GMT */
struct tm *gmtime _OP((const time_t *timer));

/* Return a string of the form "Day Mon dd hh:mm:ss yyyy\n"
	equivalent to tp */
char *asctime _OP((const struct tm *tp));

/* Equivalent to asctime (localtime (timep)) */
char *ctime _OP((const time_t *timep));
# define ctime(t) asctime(localtime((t)))

/* Converts a 'struct tm' to 'time_t' */
time_t mktime _OP((struct tm *));

/* Converts a setime/getime buffer to 'time_t' */
time_t cvtime _OP((struct sgtbuf *));

/* Converts a C broken-down time to OS-9 setime/getime format */
_VOID systime _OP((struct sgtbuf *, struct tm *));

#endif /* ! _TIME_H_ */
