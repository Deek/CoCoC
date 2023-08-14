#include <time.h>

static char	xx[26];

#ifdef ASMOPT
#asm
days fcc "Sun"
 fcb 0
 fcc "Mon"
 fcb 0
 fcc "Tue"
 fcb 0
 fcc "Wed"
 fcb 0
 fcc "Thu"
 fcb 0
 fcc "Fri"
 fcb 0
 fcc "Sat"
 fcb 0

months fcc "Jan"
 fcb 0
 fcc "Feb"
 fcb 0
 fcc "Mar"
 fcb 0
 fcc "Apr"
 fcb 0
 fcc "May"
 fcb 0
 fcc "Jun"
 fcb 0
 fcc "Jul"
 fcb 0
 fcc "Aug"
 fcb 0
 fcc "Sep"
 fcb 0
 fcc "Oct"
 fcb 0
 fcc "Nov"
 fcb 0
 fcc "Dec"
 fcb 0

datfmt fcc "%s %s %2d %02d:%02d:%02d %4d"
 fcb $d,0

#endasm
#else	/* ! ASMOPT */
static char	*days[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char	*months[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
#endif

char *
asctime(tmp)
struct tm	*tmp;
{
#ifdef ASMOPT
#asm
 ldu 4,S
 ldd 10,U tm_year
 addd #1900
 pshs D
 ldd ,U tm_sec
 pshs D
 ldd 2,U tm_min
 pshs D
 ldd 4,U tm_hour
 pshs D
 ldd 6,U tm_day
 pshs D
 ldd 8,U tm_mon
 aslb * Shift left twice to skip 4 bytes for each entry (3 chars and nul)
 rola
 aslb
 rola
 leax months,PCR
 leax D,X
 pshs X month string
 ldd 12,U tm_wday
 aslb * Same thing here, day names are all 3 chars long so index * 4
 rola
 aslb
 rola
 leax days,PCR
 leax D,X
 pshs X day of week string
 leax datfmt,PCR
 pshs X sprintf spec string
 leax xx,Y
 pshs X dest buffer
 lbsr sprintf
 leas 18,S
 leax xx,Y
 tfr X,D
#endasm
#else	/* ! ASMOPT */
	sprintf (xx, "%s %s %2d %02d:%02d:%02d %4d\n",
	         days[tmp->tm_wday], months[tmp->tm_mon], tmp->tm_mday,
	         tmp->tm_hour, tmp->tm_min, tmp->tm_sec,
	         TM_YEAR_BASE + tmp->tm_year);
	return xx;
#endif
}
