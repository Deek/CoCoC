#include "op.h"
#include "actions.h"
#include "../rma/ctype.h"

direct int opsdone;
int labelnum = 0;
action *acslots[128];

extern direct chain *freeins;
extern char *bratab[];

optinit()
{
    register action *a,**slot;
#ifdef DEBUG
    instr *ap,*rp;
#endif

    for(a = actions; a->nxtact; a++) {
#ifdef DEBUG
        debug("    %-17s  %s\n","action","replacement");
        rp = a->repp;
        for (ap = a->actp; ap; ap = ap->nxtins) {
          debug("    %-8s %-8s",(ap->mnp ? ap->mnp : "<any>"),
                                (ap->opp ? ap->opp : "<any>"));
          if (rp) {
            debug(", %-8s %-8s",(rp->mnp ? rp->mnp : "<same>"),
                                (rp->opp ? rp->opp : "<same>"));
            rp = rp->nxtins;
          }
          debug("\n");
        }
#endif
        slot = &acslots[hash(a->actp->mnp)];
        a->nxtact = *slot;
        *slot = a;
    }
}

#define isconbra(a) (((a) & BRAMASK) > (BRANCH|SUBR))

optim(i)
register instruction *i;
{
    register instruction *i2;
    action *a;
    instr *ap;
    label *l;
    int found,c;
    char *m;

    i = i->pred;
    for (;;) {
loop:   if (i == &ilist || (i2 = i->succ) == &ilist)
            return;
        if (i2->llist) {    /* look for jump to next instruction */
            if ((i->itype & BRANCH)
                        && (i->itype & BRAMASK) != (BRANCH|SUBR)) {
                /* does it jump to one of i2's labels ? */
                if (i->lab->dest == i2) {
                    /* transfer i's labels to i2 */
                    movlab(i,i2);
                    remins(i);
                    i = i2->pred;
                    ++opsdone;
                    goto loop;
                } else if ((i->itype & CODEBRK)
                            && i->llist == NULL
                            && (i = i->pred) != &ilist
                            && isconbra(i->itype)
                            && i->lab->dest == i2) {
                    fixcond(i);
                }
            }
            return;
        }
#ifdef DEBUG
        debug("actions search:\n");
#endif
        a = acslots[hash(i2->mnem)];
        for (found = FALSE; a ; a = a->nxtact) {
#ifdef DEBUG
            debug("  matching action %04x.\n",a);
#endif
            i = i2;
            c = 0;
            for (ap = a->actp; ap; ap = ap->nxtins) {
                if (i == &ilist || i->llist) break;
                if (ap->mnp && match(i->mnem,ap->mnp) == 0) break;
                if (ap->opp) {
                    if (ap->opp == 1) {
                        if (i->pred == &ilist) break;
                        if (match(i->args,i->pred->args) == 0) break;
                    } else if (match(i->args,ap->opp) == 0) break;
                }
#ifdef DEBUG
                debug("    matched: %-8s %-8s, %-8s %-8s\n",i->mnem,i->args,
                               (ap->mnp ? ap->mnp : "<any>"),
                               (ap->opp ? ap->opp : "<any>"));
#endif
                i = i->pred;
                ++c;
            }
            if (ap == NULL) {
                found = TRUE;
                break;
            }
        }

        if (!found) return;
#ifdef DEBUG
        debug("  all matched action %04x.\n",a);
#endif
        /* all matched - replace */
        i = i2;
        for (ap = a->repp; ap; ap = ap->nxtins) {
            if (ap->mnp) strcpy(i->mnem,ap->mnp);
            if (ap->opp) strcpy(i->args,ap->opp);
            i = i->pred;
            --c;
        }
        while (c > 0) {
            i2 = i->pred;
            remins(i);
            ++opsdone;
            i = i2;
            --c;
        }
        return;
    }
}

fixcond(i)
register instruction *i;
{
    i->itype = ((i->itype & BRAMASK) ^ 1)
                    | (i->succ->itype & ~(CODEBRK|BRAMASK));
    strcpy(i->mnem,bratab[i->itype & BRATYPE]);
    remref(i,i->lab);
    insref(i,i->lab = i->succ->lab);
    remins(i->succ);
    ++opsdone;
}

match(s,p)
register char *s,*p;
{
    if(p == NULL)
        return 1;

    while(*p) {
        if(*p == '<') {
            if(isdigit(*s)) {
                unsigned n =0 ;
                do {
                    n = n * 10 + *s++ - '0';
                } while(isdigit(*s));

                if(n > 255)
                    return 0;
            }
            else return 0;
            p++;
        }
        else if(*s++ != *p++)
            return 0;
    }
    return (*s == '\0');
}

finddupl(i)
register instruction *i;
{
    register instruction *ip1;
    label *lbp;
    chain *rlp1,*rlp2,*lmovp;

    if (i->pred == &ilist) return;
    /* move labels on "bra"s to here */
    lmovp = NULL;
    /* travel down label list */
    for (lbp = i->llist; lbp; lbp = lbp->nextl)
        /* travel down reference list */
        for (rlp1 = lbp->rlist; rlp1; rlp1 = rlp1->succ) {
            ip1 = rlp1->pred;
            /* is reference labeled code-break, not blocked ? */
            if (ip1->llist
                    && (ip1->itype & CODEBRK)
                    && !(ip1->itype & BLOCKDUP)) {
#ifdef DEBUG
                debug("labeled branch adjusted: ");
                prtins(ip1);
#endif
                rlp2 = newchain();
                rlp2->succ = lmovp;
                lmovp = rlp2;
                lmovp->pred = ip1;
            }
        }
    while (lmovp) {
        movlab(ip1 = lmovp->pred,i);
        if ((ip1 = ip1->pred) != &ilist && (ip1->itype & BRANCH)) {
            if (ip1->itype & CODEBRK)
                remins(ip1->succ);
            else if (isconbra(ip1->itype)
                        && ip1->lab->dest == ip1->succ->succ)
                fixcond(ip1);
        }
        rlp2 = lmovp->succ;
        freechain(lmovp);
        lmovp = rlp2;
    }
    if (!(i->pred->itype & CODEBRK)) {
        /* travel down label list */
        for (lbp = i->llist; lbp; lbp = lbp->nextl)
            /* travel down reference list */
            for (rlp1 = lbp->rlist; rlp1; rlp1 = rlp1->succ)
                if ((ip1 = rlp1->pred)->itype & CODEBRK)
                    /* match code before branches to another label */
                    /* of instruction with code before this branch */
                    duplrefs(lbp->nextl,ip1);
        /* match code before label with code before branches */
        duplrefs(i->llist,i);
    }
    if (i->itype & CODEBRK) {
        if (i->itype & BRANCH) {
            /* does branch label know its destination ? */
            if (ip1 = i->lab->dest) {
                /* code before branch match code before label ? */
                if (!duplsrch(i->pred,ip1->pred))
                    /* match code before other branches with */
                    /* code before this branch */
                    duplrefs(ip1->llist,i);
            } else
                /* match code before other branches to same */
                /* label with code before this branch */
                duplrefs(i->lab,i);
        } else {
            /* travel down instruction list */
            for (ip1 = i->pred; ip1 != &ilist; ip1 = ip1->pred) {
                if (!(ip1->itype & CODEBRK)
                        || (ip1->itype & (BLOCKDUP|BRANCH)))
                    continue;
                if (strcmp(i->mnem,ip1->mnem) == 0
                        && strcmp(i->args,ip1->args) == 0
                        && duplsrch(ip1->pred,i->pred))
                    break;
            }
        }
    }
}

duplsrch(ip1,ip2)
register instruction *ip1,*ip2;
{
    char bralbl[LABELSIZE+1];
    instruction *ip1b,*ip2b;
    label *lbp;
    int count;

    count = 0;
    if ((ip1b = ip1) == (ip2b = ip2)) return 0;
    while (ip1 != &ilist  &&  ip2 != &ilist
            &&  ip1 != ip2b  &&  ip2 != ip1b
            && (ip1->itype & BRAMASK) == (ip2->itype & BRAMASK)
            && !(ip1->itype & BLOCKDUP)
            && (ip1->itype & BRANCH
                    ? (ip1->lab->dest
                            ? ip1->lab->dest == ip2->lab->dest
                            : ip1->lab == ip2->lab)
                    : strcmp(ip1->mnem,ip2->mnem) == 0
                            && strcmp(ip1->args,ip2->args) == 0)) {
        ip1 = ip1->pred;
        ip2 = ip2->pred;
        ++count;
    }
#ifdef DEBUG
    debug("duplsrch: count = %d\n",count);
    debug("    %04x ",ip1->itype); prtins(ip1);
    debug("    %04x ",ip2->itype); prtins(ip2);
#endif
    ip1 = ip1->succ;
    if (count) {
        ip2b = ip2 = ip2->succ;
        /* save first instruction's label list */
        if ((lbp = ip1->llist) == NULL)
            if ((lbp = ip2->llist) == NULL) {
                lbp = inslabel(newbra(bralbl));
                lbp->dest = ip2;
                ip2->llist = lbp;
            }
        while (count-- >= 0) {
            movlab(ip1,ip2);
            ip1 = ip1->succ;
            remins(ip1->pred);
            ++opsdone;
            ip2 = ip2->succ;
        }
        insins(ip1,"lbra",lbp->lname,NULL);
        optim(ip1);
        /* travel down first instruction's label list */
        duplrefs(ip2b->llist,ip2b);
        return 1;
    }
    return 0;
}

duplrefs(lbp,ip)
label *lbp;
instruction *ip;
{
    register instruction *ip1;
    chain *rlp;

    /* travel down label list */
    for ( ; lbp; lbp = lbp->nextl)
        /* travel down reference list */
        for (rlp = lbp->rlist; rlp; rlp = rlp->succ)
            if (((ip1 = rlp->pred)->itype & CODEBRK)
                   && (ip1->itype & BLOCKDUP) == 0
                   && duplsrch(ip1->pred,ip->pred))
                break;
}
