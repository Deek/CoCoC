	/* this file has the location of the parser, and the size of the progam desired */
	/* It may also contain definitions to override various defaults: for example,
	/* WORD32 tells yacc that there are at least 32 bits per int */
	/* on some systems, notably IBM, the names for the output files and tempfiles must
	/* also be changed  */

/*#define WORD32*/
	/* location of the parser text file */
#if defined(_OS9) || defined(_OSK) || defined(_OS9000)
# define PARSER "/dd/lib/yaccpar"
#else
# define PARSER "/usr/lib/yaccpar"
#endif

	/* basic size of the Yacc implementation */
#if defined(_OS9)
# define SMALL
#elif defined(_OSK) || defined(__pdp11__)
# define MEDIUM
#else
# define HUGE
#endif
