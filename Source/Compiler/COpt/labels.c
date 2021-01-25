#include "op.h"

chain ltable[128];
direct label *lfree;

labelinit()
{
    register int i;
    register chain *p;

    for(p = ltable,i = 0; i < 128; i++) {
        p->succ = p->pred = p;
        p++;
    }
}

label *inslabel(s)
char *s;
{
    register chain *l;
    register label *p;

    p = newlabel();

    l = &ltable[hash(s)];
    strcpy(p->lname,s);

    /* put at back of list */
    p->succ = l;
    p->pred = l->pred;
    l->pred->succ = p;
    l->pred = p;

    return p;
}

label *findlabel(s)
char *s;
{
    label *p;
    chain *l;

    l = &ltable[hash(s)];


    for(p = l->succ; p != l;) {
        if(strcmp(s,p->lname) == 0)
            return p;
        p = p->succ;
    }
    return NULL;
}

hash(s)
register char *s;
{
    int i = 0;

    while (s && *s)
        i += *s++;

    return i & 127;
}

label *
newlabel()
{
    register label *p;

    if(p = lfree) {
        lfree = p->succ;
        p->dest = p->rlist = p->nextl = p->lflags = NULL;
    } else p = grab(sizeof(*p));

    return p;
}

freelabel(p)
register label *p;
{
    if (p->dest == NULL && p->rlist == NULL) {
        /* remove from ltable */
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        /* add to free list */
        p->succ = lfree;
        lfree = p;
    }
}

char *newbra(lblp)
char *lblp;
{
    static int labnum = 0;

    sprintf(lblp,"_$%d",++labnum);
    return lblp;
}

