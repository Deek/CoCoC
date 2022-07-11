#include <math.h>
#include <float.h>

#define DBL_BIAS 128

#ifndef __DCC__
extern double _huge;

#asm
 vsect
_huge fdb 32767,-1,-1,-1 largest possible double
 endsect
#endasm
#endif	/* ! __DCC__ */

#asm
scalbn:
#endasm
double
ldexp (x, exp)
double x;
int exp;
{
	int i, neg;
	register unsigned char *cx = &x;

	neg = ((*cx & 0x80));	/* sign bit */
	cx += 7;				/* skip to exponent byte */
	i = *cx - DBL_BIAS;		/* get the unbiased exponent */
	i += exp;

	if (i < DBL_MIN_EXP) return 0.;
#ifndef __DCC__
	if (i > DBL_MAX_EXP) return neg ? -_huge : _huge;
#else
	if (i > DBL_MAX_EXP) return neg ? -HUGE_VAL : HUGE_VAL;
#endif

	*cx += exp;				/* set the new exponent */
	return x;
}
