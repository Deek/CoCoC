#include <time.h>

/* Array shared by mktime(), localtime(), and cvtime() */

int	_mdays[] = {0,31,59,90,120,151,181,212,243,273,304,334};
int	_myears[] = {365, 366};

int	daylight = -1;
long int	timezone = 0;
