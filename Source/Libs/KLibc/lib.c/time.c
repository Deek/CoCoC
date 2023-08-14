#include <time.h>

extern unsigned _mdays[];
extern int _isleap();
extern unsigned _leaps();

time_t
time (arg)
time_t  *arg;
{
#ifdef ASMOPT
#asm
 leas -6,s allocate time buffer
 leau ,s point to time buffer
 pshs u
 lbsr getime get system time
 stu ,s still time buffer
 lbsr cvtime convert to C time_t
 ldu 12,s check arg
 beq noarg
 ldd ,x so copy over
 std ,u
 ldd 2,x
 std 2,u
noarg leas 8,s
#endasm
#else
	time_t          result;
	struct sgtbuf   tbuf;
	register struct sgtbuf *tptr = &tbuf;

	getime (tptr);
	result = cvtime (tptr);
	if (arg)
		*arg = result;
	return result;
#endif	/* ! ASMOPT */
}

#ifdef _OS9
#asm
o2utime: 
#endasm
#endif
time_t
cvtime (bp)
struct sgtbuf	*bp;
{
	time_t				accum;
	register unsigned	d;
	int					y;

	/* get days between epoch and start of year */
	y = bp->t_year - (EPOCH_YEAR - TM_YEAR_BASE);
	if (y < 0 || y > 136) goto err;	/* Sanity check: no dates outside epoch */

	d = y * 365 + _leaps (y);	/* add 1 for each leap day */

	/* add the days for completed months "this" year */
	d += _mdays[bp->t_month - 1] + ((bp->t_month > 2) && _isleap (y));

	/* add days passed this month, convert to hours (and long) */
	d += (bp->t_day - 1);
	accum = (time_t) d * 24 ;

	/* add hours passed today, convert to minutes */
	accum += bp->t_hour;
	accum *= 60;

	/* minutes passed this hour, convert to seconds */
	accum += bp->t_minute;
	accum *= 60;

	/* add seconds passed this minute */
	accum += bp->t_second;

	if (timezone) accum -= timezone;

	return accum;

err:return -1L;
}
