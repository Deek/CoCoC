#include <time.h>

extern unsigned _mdays[];
extern unsigned _isleap();

/*
** function to convert a 'tm' style time to os9 time
*/
#ifdef _OS9
#asm
u2otime: 
#endasm
#endif
systime(ot, tm)
struct sgtbuf		*ot;
register struct tm	*tm;
{
#ifdef ASMOPT
#asm
 ldx 4,S *ot
 leax 6,X get to end
 lda #6 byte to copy
u2o1 ldb 1,U get data
 stb ,-X stash away
 leau 2,U skip ahead
 deca
 bne u2o1
 inc 1,x increment the month
#endasm
#else
	ot->t_year = tm->tm_year;
	ot->t_month = tm->tm_mon + 1;
	ot->t_day = tm->tm_mday;
	ot->t_hour = tm->tm_hour;
	ot->t_minute = tm->tm_min;
	ot->t_second = tm->tm_sec;
#endif
}
