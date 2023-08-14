#include <time.h>

extern unsigned _mdays[];

extern int _isleap();
extern unsigned _leaps();

time_t
mktime (tp)
struct tm	*tp;
{
	time_t				accum;
	register unsigned	d;
	int					y;

	/* get days between epoch and start of year */
	y = tp->tm_year - (EPOCH_YEAR - TM_YEAR_BASE);
	if (y < 0 || y > 136) goto err;	/* Sanity check: no dates outside epoch */

	d = y * 365 + _leaps (y);	/* add 1 for each leap day */

	/* add the days for completed months "this" year */
	d += _mdays[tp->tm_mon] + ((tp->tm_mon > 1) && _isleap (y));

	/* add days passed this month, convert to hours (and long) */
	d += (tp->tm_mday - 1);
	accum = (time_t) d * 24;

	/* add hours passed today, convert to minutes */
	accum += tp->tm_hour;
	accum *= 60;

	/* minutes passed this hour, convert to seconds */
	accum += tp->tm_min;
	accum *= 60;

	/* add seconds passed this minute */
	accum += tp->tm_sec;

	if (timezone) accum -= timezone;

	return accum;

err:return -1L;
}


#if 0
int
main (argc, argv)
int argc;
char **argv;
{
	time_t now = time (NULL);
	struct tm *np = localtime (&now);
	time_t mine = mytime(np);
	time_t sys = mktime(np);

	printf ("%d %d %d %d\n", now, sys, mine, now - mine);
}
#endif
