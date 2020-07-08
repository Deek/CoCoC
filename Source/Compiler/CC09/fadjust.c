#include <stdio.h>

/* This function accepts a GCC double and converts it to a coco double
	 gcc double: seeeeeeeeeeeffffffffffffffffffffffffffffffffffffffffffffffffffff, bias 1023
	coco double: sfffffffffffffffffffffffffffffffffffffffffffffffffffffffeeeeeeee, bias 128
	             0123456701234567012345670123456701234567012345670123456701234567
   Only positive double are passed to this function */
   
double fadjust( double n )
{
	union
	{
		double d;
		long long lp;
	} a;
	
	int exponent;
	unsigned char ue;
	
	if( sizeof a > 8 )
		fprintf( stderr, "Floating Point Conversion Error\n" );
	
	/* Special case zero */
	if( n == 0.0 )
	{
		a.lp = 0ll;
		return a.d;
	}
		
	a.d = n;

	exponent = (a.lp >> 52 ); /* Shift exponent */
	exponent -= 1024; /* De bias GCC exponent */
	ue = (exponent + 130); /* Bias CoCo exponent */

	a.lp <<= 11; /* Shift fraction into proper location */
	a.lp &= 0x7fffffffffffffffll; /* clear sign bit (always positive) */
	a.lp |= ue; /* Or in 8 bit exponent */
	
	return a.d;
}
