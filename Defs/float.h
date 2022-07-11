#ifndef _FLOAT_H_
#define _FLOAT_H_

/* Same as in <math.h> */
#ifndef FLT_EVAL_METHOD
# define FLT_EVAL_METHOD 1	/* operations and constants evaluate as double */
#endif

#define FLT_RADIX	2	/* Radix of exponent representation */
#define FLT_ROUNDS	-1	/* Rounding mode is indeterminable */

/* Number of significant decimal digits */
#define FLT_DIG	6
#define DBL_DIG	16

/* Mantissa base-RADIX digits (23+1 float, 55+1 double */
#define FLT_MANT_DIG	24
#define DBL_MANT_DIG	56

/* Minimum value which may be added to 1.0 */
#define FLT_EPSILON	2.384185791015625e-7
#define DBL_EPSILON	1.4210854715202004e-14

/* Maximum representable floating point number */
#define FLT_MAX	1.7014117331926443e+38
#define DBL_MAX	1.7014118346046923e+38

/* Minimum normalized positive floating point number */
#define FLT_MIN	2.938736e-39
#define DBL_MIN	2.938735877055719e-39

/* Minimum negative integer such that FLT_RADIX^(x-1) is normalizable */
#define FLT_MIN_EXP	-127
#define DBL_MIN_EXP	-127

/* Maximum positive integer such that FLT_RADIX^(x-1) is representable */
#define FLT_MAX_EXP	127
#define DBL_MAX_EXP	127

/* Maximum integer such that 10^x is representable */
#define FLT_MAX_10_EXP	+38
#define DBL_MAX_10_EXP	+38

/* Minimum integer such that 10^x is normalizable */
#define DBL_MIN_10_EXP	-38
#define FLT_MIN_10_EXP	-38

#endif /* _FLOAT_H_ */
