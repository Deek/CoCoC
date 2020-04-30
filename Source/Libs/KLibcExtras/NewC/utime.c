#include <utime.h>
#include <time.h>

/*
** function to return Unix style time as a long.
**  fetches time from the system and converts it.
*/


long
time(arg)
long  *arg;
   {
   char  tbuf[6];
   long  result, o2utime();

   getime(tbuf);
   result = o2utime(tbuf);
   if (arg)
      *arg = result;
   return (result);
   }


/*
** function to change os9 style time to unix style long
**  converts passed char buffer to a long
*/

static int   monsiz[] = {31,28,31,30,31,30,31,31,30,31,30,31};

long
o2utime(tp)
register char  *tp;                       /* remember - buf is char[6] */
   {
   int   j, mon;
   long  accum = 0;

   for (j = 70; j < *tp; ++j)           /* total up days in years past */
      accum += ((j & 3) ? 365 : 366);       /* don't forget leap years */

   monsiz[1] = (*tp++ & 3) ? 28 : 29;             /* fix for leap year */
   mon = *tp++ - 1;                    /* get local copy and 0 base it */

   for (j = 0; j < mon; ++j)               /* add days for months past */
      accum += monsiz[j];

   accum += (*tp++ - 1);                        /* add days this month */
   accum *= 24;                                    /* convert to hours */

   accum += *tp++;                                  /* add hours today */
   accum *= 60;                                  /* convert to minutes */

   accum += *tp++;                                /* add minutes today */
   accum *= 60;                                  /* convert to seconds */

   accum += *tp;                                  /* add seconds today */

   return (accum);
   }
/*page*/
/*
** function to convert a 'tm' style time to os9 time
*/

u2otime(otime, tmp)
struct sgtbuf  *otime;
struct tm      *tmp;
   {
   otime->t_year = tmp->tm_year;
   otime->t_month = tmp->tm_mon + 1;           /* basing difference */
   otime->t_day   = tmp->tm_mday;
   otime->t_hour  = tmp->tm_hour;
   otime->t_minute = tmp->tm_min;
   otime->t_second = tmp->tm_sec;
   }


/*
** function to return a broken down time from a Unix style long.
*/

long           timezone = 0;
int            daylight = 0;
static struct  tm _tm;

struct tm *
localtime(utime)
long  *utime;
   {
   register struct tm *tmp = &_tm;
   long  ticks = *utime;
   int   days;
   int   fac;

   tmp->tm_sec = ticks % 60;                      /* split out seconds */
   ticks /= 60;

   tmp->tm_min = ticks % 60;                      /* split out minutes */
   ticks /= 60;

   tmp->tm_hour = ticks %24;                        /* split out hours */
   days = ticks / 24;

   tmp->tm_wday = (days + 4) % 7;           /* extract the day of week */

   for (tmp->tm_year = 70; days >= 0; ++tmp->tm_year)
      days -= (fac = ((tmp->tm_year & 3) ? 365 : 366));

   tmp->tm_year -= 1;
   tmp->tm_yday = (days += fac);               /* adjust for overshoot */

   monsiz[1] = (tmp->tm_year & 3) ? 28 : 29;
   for (tmp->tm_mon = 0; days >= monsiz[tmp->tm_mon]; ++tmp->tm_mon)
      days -= monsiz[tmp->tm_mon];

   tmp->tm_mday = days + 1;

   tmp->tm_isdst = 0;                               /* just in case */

   return (tmp);
   }
/*page*/
/*
** two functions to return Unix style date and time strings.
**  same 26 char format is used.
*/


static   char  xx[26];
static   char  *days[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static   char  *months[] = {
         "Jan","Feb","Mar","Apr","May","Jun",
         "Jul","Aug","Sep","Oct","Nov","Dec"};


char   *
asctime(tmp)
struct tm *tmp;
   {
   sprintf(xx, "%s %s %2d %02d:%02d:%02d 19%02d\n",
      days[tmp->tm_wday], months[tmp->tm_mon], tmp->tm_mday,
      tmp->tm_hour, tmp->tm_min, tmp->tm_sec, tmp->tm_year);
   return (xx);
   }


char  *
ctime(ticks)
long   *ticks;
   {
   struct tm *tmp;

   return (asctime(localtime(ticks)));
   }
