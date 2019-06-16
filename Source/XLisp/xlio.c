/* xlio - xlisp i/o routines */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* global variables */
int (*xlgetc)();
int xlpvals;
int xlplevel;

/* local variables */
static int prompt;
static FILE *ifp;

/* tgetc - get a character from the terminal */
static int tgetc()
{
    int ch;

    /* prompt if necessary */
    if (prompt) {
        if (xlplevel > 0)
            printf("%d> ",xlplevel);
        else
            printf("> ");
        prompt = FALSE;
    }

    /* get the character */
    if ((ch = getc(stdin)) == '\n')
        prompt = TRUE;

    /* return the character */
    return (ch);
}

/* xltin - setup terminal input */
int xltin(flag)
  int flag;
{
    /* flush line if flag is set */
    if (flag & !prompt)
        while (tgetc() != '\n')
            ;

    /* initialize */
    prompt = TRUE;
    xlplevel = 0;
    xlgetc = tgetc;
    xlpvals = TRUE;
}

/* fgetcx - get a character from a file */
static int fgetcx()
{
    int ch;

    /* get a character */
    if ((ch = getc(ifp)) <= 0) {
        xlgetc = tgetc;
        xlpvals = TRUE;
        return (tgetc());
    }

    /* return it */
    return (ch);
}

/* xlfin - setup file input */
xlfin(str)
  char *str;
{
#ifdef DEFEXT
    char fname[100];

    /* create the file name */
    strcpy(fname,str);

    /* check for extension */
    if (strchr(fname,'.') == 0)
        strcat(fname,".lsp");
#else
#define fname str
#endif

    /* open the input file */
    if ((ifp = (FILE *) fopen(fname,"r")) == (FILE *) NULL) {
        printf("can't open \"%s\" for input\n",fname);
        return;
    }

    /* setup input from the file */
    xlgetc = fgetcx;
    xlpvals = FALSE;
}