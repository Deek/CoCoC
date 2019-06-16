/* xlisp - a small subset of lisp */

#ifdef AZTEC
#include "a:stdio.h"
#include "a:setjmp.h"
#else
#include <stdio.h>
#include <setjmp.h>
#endif

#include "xlisp.h"

/* global variables */
jmp_buf xljmpbuf;

/* external variables */
extern struct node *xlenv;
extern struct node *xlstack;
extern int xlpvals;

/* main - the main routine */
main(argc,argv)
  int argc; char *argv[];
{
    struct node expr;

    /* initialize the dynamic memory module (must be first) */
    xldmeminit();

    /* initialize xlisp */
    xlinit();
    xleinit(); xllinit(); xlminit();
    xloinit(); xlsinit(); xlfinit();
    xlpinit();

    /* initialize the 'Keymap' class */
#ifdef KEYMAPCLASS
    xlkinit();
#endif

    /* initialize terminal input */
    xltin(FALSE);

    /* read the input file if specified */
    if (argc > 1)
        xlfin(argv[1]);
    else
        printf("XLISP version 1.1\n");

    /* main command processing loop */
    while (TRUE) {

        /* setup the error return */
        setjmp(xljmpbuf);

        /* free any previous expression and leftover context */
        xlstack = xlenv = NULL;

        /* create a new stack frame */
        xlsave(&expr,NULL);

        /* read an expression */
        expr.n_ptr = xlread();

        /* evaluate the expression */
        expr.n_ptr = xleval(expr.n_ptr);

        /* print it if necessary */
        if (xlpvals) {
            xlprint(expr.n_ptr,TRUE);
            putchar('\n');
        }
    }
}