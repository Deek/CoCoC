#include "cj.h"

#include <assert.h>

/*
	This function accepts an IEEE 754 format double-precision number and
	converts it to our compiler's special format, which is almost the same as
	the PDP-11/VAX "D" format used on old PDP-11 Unix. All our bits are the
	same, but they are in a different order, probably because this allows at
	least the exponent to be held in just one contiguous byte.

	IEEE double: [sign][11 bits exponent][52 bits mantissa], exponent bias 1022
	PDP double : [sign][8 bits exponent][55 bits mantissa], exponent bias 128
	6809 double: [sign][55 bits mantissa][8 bits exponent], exponent bias 128

	PDP numbers of all kinds are stored in a weird way -- mixed-endian. The
	PDP-11 is natively a 16-bit little-endian machine without byte addressing,
	so any number large enough to need more than one 16-bit memory cell is
	stored little-endian (going from least significant to most significant).
	However, each individual word is stored big-endian just like a byte on a
	byte-addressable machine. So a PDP-11 will store a 64-bit double like this:

	1 0 3 2 5 4 7 6 (0 is the least significant byte, 7 the most)
	for comparison, this is how an x86 machine stores a 64-bit number:
	0 1 2 3 4 5 6 7
	And a big-endian architecture like 68000 or 6809:
	7 6 5 4 3 2 1 0

	Note: since the converted double's endianness is native for the machine
	the compiler is running on, the bytes will still need to be reordered on
	output.
*/

#define EXPN_BIAS	128
#define EXPN_BITS	8
#define EXPN_MASK	0xFF
#define MANT_BITS	55
#define SIGN_MASK	(0x1LL << 63)

#define PDP_SIGN_MASK	SIGN_MASK
#define	PDP_MANT_BITS	MANT_BITS
#define	PDP_MANT_MASK	MANT_BITS
#define PDP_EXPN_MASK	(0xFFLL << PDP_MANT_BITS)
#define PDP_EXPN_BIAS	EXPN_BIAS
#define PDP_EXPN_BITS	8

#define	IEEE_SIGN_MASK	SIGN_MASK
#define IEEE_MANT_BITS	52
#define IEEE_EXPN_MASK	(0xFFFLL << IEEE_MANT_BITS)
#define IEEE_EXPN_BIAS	1022

double
dadjust (n)
double	n;
{
	union {
		double		d;
		uint64_t	l;
	} a;
	int			exponent;
	uint64_t	sign;
	uint64_t	mantissa;
	uint8_t		uexp;

	assert (sizeof (a) == 8);

	/* Special case for zero */
	if (n == 0.0) {
		a.l = 0LL;
		return a.d;
	}
	a.d = n;

	sign = a.l & IEEE_SIGN_MASK;		/* high bit is sign, 1=negative */
	exponent = ((a.l & ~sign) >> IEEE_MANT_BITS) - IEEE_EXPN_BIAS;
	mantissa = a.l & ~IEEE_EXPN_MASK;

	if ((exponent > EXPN_BIAS-1) || (exponent < -EXPN_BIAS))
		fprintf (stderr, "floating point exponent out of range\n");

#if 0 && defined(DEBUG)
	fprintf(stderr, "exponent: 2^%d, mantissa: 0x%013llx\n", exponent, mantissa);
#endif

	uexp = (exponent + EXPN_BIAS);	/* rebias 6809 exponent */

#if 0 && defined(DEBUG)
	fprintf(stderr, "new exponent: %d, mantissa: %014llx\n", uexp, mantissa >> 8);
#endif

	a.l = sign | (mantissa << (EXPN_BITS + (MANT_BITS - IEEE_MANT_BITS))) | uexp;
	return a.d;
}
