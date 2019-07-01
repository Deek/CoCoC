/*
	@(#)history.c	2.2.4	10/28/87
*/
/*
	Modification History:

		04	Allowed @ as "pass rest of line to assembler"	09-19-84	kkk
		05	Checked for output errors periodically			11/08/84	kkk
		******** V1.3 ********
		06	Changed UNIX version to look for /dd ins. /user 11-20-84	kkk
			Fixed 68K detach(-1) problem (crashed system).
		07	Added "-o=" option for vms mainly				02-04-85	kkk
		08	Don't output asmline comments if #asm in effect	03-04-85	kkk
		09	The -v= options may appear many times to give	25-Jun-85	kkk
			additional directories to search for < > include
			files.  Also, if no -v= options appears, the environment
			is checked for a DDEV variable, the value of which is used
			for the default.  If no DDEV appears in the directory,
			/dd/defs is used.
		******** V2.0 ********
05-Nov-85
	10	Fixed problem where cpp would forget defined names if arguments
		were given and no expansion.
	11	Allow long macro names.  Current limit is now 256 characters.
		Redefined macro names now draw an error.  The __LINE__ and __FILE__
		macros are now available.
17-Dec-85
	12	Comments on include directives spanning multiple lines are now
		handled properly.
19-Sep-86
	13	Now look for CDEF environement variable for default include directory
		if no -v is given and not run under cc.
30-Sep-86
	14	Final compile for 2.1 pre-release
27-Oct-86
	15	Final compile for 2.1 release
13-Mar-87
	16	Addition of directives and facilities as described in ANSI X3J11,
		i.e. #if, #elif, the defined operator, and unsigned constants in
		preprocessor expressions
17-Mar-87
	17	Reworked eval.c, lex.c and eval.h for consistency.  Added VMS
		conditionals.
 30-Mar-87
 	18	Made scan() stack lbase as well as lptr for correct error message
 		reporting.
 	19	Modified scan() and the macro structure to support X3J11 3.8.3.4,
 		which forbids replacement of macros being replaced (avoiding
 		endless recursion).
 05-May-87
 	20	Modified scan(), its callers, and added a different flavor of
 		gch() (which, of course, calls gch()) so that invocations of
 		macros can cross line boundaries.
 13-May-87
 	21	Modified addmac() so that bogus formal parameters don't send
 		it into an infinite loop!
***** 2.2 Release 87/05/29 *****
87/09/15
	22	Reversed order of PSECT and NEWFNAME compiler directives and added
		OLDFNAME to indicate the end of an include file so the compiler can
		pass the info on to the source debugger (main(), putesc()). [lac]
87/10/05
	23	Modified nextname() and doinclude() to conform to K&R description
		of handling of #include "woof.h" (i.e. look in current (data for OS-9)
		directory, *then in same sequence of places as <> header files!*)
		NOTE: watch the ANSI Standard closely; if it diverges from K&R,
		then we'll follow it. [jej]
	24	Made exit from #included file stomp heavily on nxtlno (now global,
		darn it) to keep cpp from being fooled about what line it's on in
		some <expletive> obscure cases. [jej]
*/
#ifdef OSK
@_sysedit: equ 24
#else
static char *_xx = "@_sysedit=24";
#endif
