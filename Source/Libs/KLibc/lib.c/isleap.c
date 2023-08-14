#include <time.h>

/*
	_isleap (year)

	Returns whether epoch+year is a leap year. Note that this is explicitly
	based on the Unix epoch
*/
int
_isleap (year)
register int	year;
{
	/* By rule, this is the same as adding 1970 but allows shorter calcs */
	year += (300 - (EPOCH_YEAR - TM_YEAR_BASE));
	return (!(year & 3) && ((year % 100) || !(year % 400)));
}


/*
	_leaps (year)

	Returns the number of leap days between the epoch and the start of
	(epoch + year).
*/
unsigned
_leaps (year)
register int	year;
{
	int	ld = 0;

	while (year) {
		if (_isleap (--year))
			ld++;
	}
	return ld;
}
