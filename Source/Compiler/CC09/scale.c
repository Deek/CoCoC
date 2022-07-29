#include <math.h>

/*
	These functions are modified from the new version of the C library,
	because the new version of the code is too big for the compiler.
*/
#ifdef _OS9
static int atoftbl[] = {
	0, 0, 0, 0x81,				/* 5^0 (1) */
	0x2000, 0, 0, 0x83,			/* 5^1 (5) */
	0x4800, 0, 0, 0x85,			/* 5^2 (25) */
	0x7a00, 0, 0, 0x87,			/* 5^3 (125) */
	0x1c40, 0, 0, 0x8a,			/* 5^4 (625) */
	0x4350, 0, 0, 0x8c,			/* 5^5 (3125) */
	0x7424, 0, 0, 0x8e,			/* 5^6 (15625) */
	0x1896, 0x8000, 0, 0x91,	/* 5^7 */
	0x3ebc, 0x2000, 0, 0x93,	/* 5^8 */
	0x6e6b, 0x2800, 0, 0x95,	/* 5^9 */
	0x1502, 0xf900, 0, 0x98,	/* 5^10 */
	0x2d78, 0xebc5, 0xac62, 0x00af,	/* 5^20 */
	0x49f2, 0xc9cd, 0x0467, 0x4fc6,	/* 5^30 */
	0x6b19, 0x4f8e, 0x1ae5, 0x26dd,	/* 5^40 */
	0x08d8, 0x762b, 0xf324, 0xcdf5	/* 5^50 */
};
static double *dtbl = atoftbl;


static double
exp5 (power)
register int power;
{
	if (power > 9)
		return dtbl[(power%10)] * dtbl[((power/10))+9];

	return dtbl[power];
}
#endif


static double
scale0 (d, cnt, pos)
double	d;
register int	cnt, pos;
{
#ifdef _OS9
	return (pos ? (d * exp5 (cnt)) : (d / exp5 (cnt)));
#else
	return (pos ? (d * pow(5.0, (double) cnt)) : (d / pow (5.0, (double) cnt)));
#endif
}


double
scale(d, cnt, sign)
double	d;
int		cnt, sign;
{
#ifdef _OS9
	register unsigned char *dx = &d;

	if (cnt) {
		d = scale0 (d, cnt, sign);

		/* poor man's ldexp() */
		if (sign) *(dx+7) += cnt;
		else *(dx+7) -= cnt;
	}

	return d;
#else
	if (!cnt) return d;

	return ldexp (scale0 (d, cnt, sign), sign ? cnt : -cnt);
#endif
}
