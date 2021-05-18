#ifndef _GETOPT_H_
#define _GETOPT_H_
/*
	Declarations for the getopt() function.

	On a Unix-like system, this function is declared in the <unistd.h>
	header. For Unix source compatibility we do declare it there, but
	getopt() is just as useful for non-Unix folks too.
*/
#ifndef _ANSI_H_
#include <ansi.h>
#endif

extern char	*optarg;
extern int	opterr, optind, optopt;

int getopt _OP((int argc, char * const argv[], const char *optstring));

#endif /* _GETOPT_H_ */
