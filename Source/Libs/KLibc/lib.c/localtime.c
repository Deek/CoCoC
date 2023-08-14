#include <time.h>

#ifdef _OS9
extern unsigned		_mdays[];
extern unsigned		_myears[];
static struct tm	_tm;

extern unsigned _isleap(year);

struct tm *
gmtime (tp)
const time_t	*tp;
{
	time_t				ticks = *tp;
	unsigned			days, ly;
	register unsigned	i;

	/* split out seconds */
	_tm.tm_sec = ticks % 60;
	ticks /= 60;

	/* split out minutes */
	_tm.tm_min = ticks % 60;
	ticks /= 60;

	/* split out hours */
	_tm.tm_hour = ticks % 24;

	days = ticks / 24;	/* get days since the epoch */
	_tm.tm_wday = (days + EPOCH_DOW) % 7;	/* get the day of the week */

	/* finding the year and day in year */
	i = 0;
	while (days >= _myears[ly = _isleap (i)]) {
		days -= _myears[ly];
		i++;
	}
	_tm.tm_year = (EPOCH_YEAR - TM_YEAR_BASE) + i;
	_tm.tm_yday = days;

	/*
	 * If the day is on or after the start of month 'i', but before the start
	 * of month 'i+1', then our date is in that month.
	 */
	for (i = 0; i < 12; i++) {
		if ((_mdays[i+1] + (ly && i > 0)) > days)
			break;
	}
	_tm.tm_mday = days - (_mdays[i] + (ly && i > 1)) + 1;
	_tm.tm_mon = i;

	_tm.tm_isdst = -1;

	return &_tm;
}


struct tm *
localtime (tp)
const time_t	*tp;
{
	time_t	tt = *tp + timezone;

	return gmtime (&tt);
}
#endif

#ifdef TEST_MAIN
#include <stdio.h>
#include <string.h>

int
main (argc, argv)
int argc;
char **argv;
{
	struct tm foo;
	time_t nowm, nows;
	char npgs[26], npls[26];

	foo.tm_year = 205;
	foo.tm_mon = 1;
	foo.tm_mday = 29;
	foo.tm_hour = 1;
	foo.tm_min = 28;
	foo.tm_yday = 37;
	foo.tm_sec = 15;
	timezone = -5*3600;

	nowm = mktime (&foo);
	nows = 0xffffffff;

	printf ("%08lx %08lx\n", nowm, nows);

	strcpy (npgs, asctime (gmtime (&nowm)));
	strcpy (npls, asctime (localtime (&nowm)));

	printf ("%s%s", npgs,
	                npls);
	return 0;
#ifdef _OS9
	pflinit();
#endif
}
#endif
