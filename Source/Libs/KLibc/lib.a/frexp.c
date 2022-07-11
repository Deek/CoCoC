/*
	x = frexp (arg, &i);
	returns a double fp quantity x which is a fraction < 1.0
	and a corresponding binary exponent "i" such that

		arg = x * 2^i
*/

double
frexp (x, i)
double	x;
int		*i;
{
	register int	j = 0, neg;
	unsigned char	*cx = &x;

	neg = (*cx & 0x80);		/* check sign bit */
	if (neg) *cx &= ~0x80;	/* get rid of sign bit */

	if (x >= 1.) while (x >= 1.) {
		j++;
		(*(cx+7))--;	/* decrement exponent */
	} else while (x < .5) {
		j--;
		(*(cx+7))++;	/* increment exponent */
	}
	*i = j;	/* set argument */

	if (neg) *cx |= 0x80;	/* re-set sign bit */

	return x;
}
