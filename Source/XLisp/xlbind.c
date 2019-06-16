/* xlbind - xlisp symbol binding routines */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* global variables */
struct node *xlenv;

/* xlunbind - unbind symbols bound in this environment */
xlunbind(env)
  struct node *env;
{
    struct node *bnd;

    /* unbind each symbol in the environment chain */
    for (; xlenv != env; xlenv = xlenv->n_listnext) {
        bnd = xlenv->n_listvalue;
        bnd->n_bndsym->n_symvalue = bnd->n_bndvalue;
    }
}

/* xlbind - bind a symbol to a value */
xlbind(sym,val)
  struct node *sym,*val;
{
    struct node *lptr,*bptr;

    /* create a new environment list entry */
    lptr = newnode(LIST);
    lptr->n_listnext = xlenv;
    xlenv = lptr;

    /* create a new variable binding */
    lptr->n_listvalue = bptr = newnode(LIST);
    bptr->n_bndsym = sym;
    bptr->n_bndvalue = val;
}

/* xlfixbindings - make a new set of bindings visible */
xlfixbindings(env)
  struct node *env;
{
    struct node *eptr,*bnd,*sym,*oldvalue;

    /* fix the bound value of each symbol in the environment chain */
    for (eptr = xlenv; eptr != env; eptr = eptr->n_listnext) {
        bnd = eptr->n_listvalue;
        sym = bnd->n_bndsym;
        oldvalue = sym->n_symvalue;
        sym->n_symvalue = bnd->n_bndvalue;
        bnd->n_bndvalue = oldvalue;
    }
}