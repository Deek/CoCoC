#ifndef _MATH_H_
#define _MATH_H_
#ifndef _ANSI_H_
# include <ansi.h>
#endif

typedef double float_t;
typedef double double_t;

/* Same as in <float.h> */
#ifndef FLT_EVAL_METHOD
# define FLT_EVAL_METHOD	1	/* operations and constants evaluate as double */
#endif

/* These may change if we can use 'undefined' numbers */
#define HUGE_VAL	1.7014118346046923e+38
#define HUGE_VALF	1.7014117331926443e+38

/* Useful math constants */
#define M_E			2.7182818284590452	/* e */
#define M_LOG2E		1.4426950408889634	/* log base 2 of e */
#define M_LOG10E	0.43429448190325183	/* log base 10 of e */
#define M_LN2		0.69314718055994531	/* natural log of 2 */
#define M_LN10		2.3025850929940457	/* natural log of 10 */
#define M_PI		3.1415926535897932	/* pi */
#define M_PI_2		1.5707963267948966	/* pi/2 */
#define M_PI_4		0.7853981633974483	/* pi/4 */
#define M_1_PI		0.31830988618379067	/* 1/pi */
#define M_2_PI		0.63661977236758134	/* 2/pi */
#define M_2_SQRTPI	1.1283791670955126	/* 2/sqrt(pi) */
#define M_SQRT2		1.4142135623730950	/* sqrt(2) */
#define M_SQRT1_2	0.70710678118654752	/* 1/sqrt(2) */

/* Raise X to the Y power */
double pow _OP((double x, double y));

/* Exponential function of X (e^X) */
double exp _OP((double x));

/* Returns 10 to the power X, the base-10 "antilog" */
double exp10 _OP((double x));
double antilg _OP((double x));

/* Returns the natural log of x */
double log _OP((double x));

/* Returns the base-10 log of x */
double log10 _OP((double x));

/* chop off the fractional part of x, "rounding" towards zero */
double trunc _OP((double x));

/* Return the positive square root of x */
double sqrt _OP((double x));

/* Return the square of x */
double sqr _OP((double x));

/* Return the inverse of x (1/x) */
double inv _OP((double x));

/* Take the absolute value of X */
double fabs _OP((double x));
double dabs _OP((double x));

/* Return X * 2^I */
double dexp _OP((double x, double i));

extern char _deg;	/* If nonzero, trig functions use degrees */

/* Sine, cosine, tangent */
double sin _OP((double x));
double cos _OP((double x));
double tan _OP((double x));

/* Arcsine, arc cosine, arc tangent */
double asin _OP((double x));
double acos _OP((double x));
double atan _OP((double x));

/* Switch trigonometry functions to use degrees */
_VOID deg _OP((void));

/* Switch trigonometry functions to use radians */
_VOID rad _OP((void));

/* Hyperbolic equivalents of sine, cosine, tangent */
double sinh _OP((double x));
double cosh _OP((double x));
double tanh _OP((double x));

/* Hyperbolic area sine, cosine, tangent */
double asinh _OP((double x));
double acosh _OP((double x));
double atanh _OP((double x));

/* convert X into fraction and inegral components */
double frexp _OP((double x, int *exp));

/* Multiply X by integral power of 2 */
double ldexp _OP((double x, int exp));

/* Multiply X by integral power of radix */
double scalbn _OP((double x, int exp));

#endif /* _MATH_H_ */
