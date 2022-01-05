#include "op.h"
#include "actions.h"

direct int opsdone;
int labelnum = 0;
action *acslots[128];

static char empty[1] = {'\0'};	/* the empty string */

extern direct chain *freeins;
extern char *bratab[];

#ifdef DEBUG
static char *p1 = "<1>";
static char *p2 = "<2>";
static char *zero = "<any>";

char *
strinst (val)
char *val;
{
	switch ((int) val) {
		case 0:		return zero;
		case 1:		return p1;
		case 2:		return p2;
		default:	return val;
	}
}


dbgaction (act)
action *act;
{
    register instr *ap, *rp;

    debug("\n    %-17s  %s\n","action","replacement");
    for (ap = act->actp, rp = act->repp; ap; ap = ap->nxtins) {
        debug("    %-8s %-8s", strinst(ap->mnp), strinst(ap->opp));
        if (rp) {
            debug(", %-8s %-8s",(rp->mnp ? strinst(rp->mnp) : "<same>"),
                                (rp->opp ? strinst(rp->opp) : "<same>"));
            rp = rp->nxtins;
        }
        debug("\n");
    }
}
#endif

static instr *
mkInstr(line)
char *line;
{
	instr *i = grab(sizeof(instr));
	register char *c = line;
	char *idx, *mnem, *oper;
	int count;

	i->nxtins = NULL;
	while (*c) {

		if (idx = index(c, ' ')) {
			count = (idx - c);
		} else {	/* this is an error, no possible operand */
			error ("incomplete instruction");
		}

		if (count == 1 && *c == '*') {
			i->mnp = NULL;	/* "don't care" */
		} else if (*c == '^') switch (*(c+1)) {
			case '0':
				i->mnp = NULL;
				break;
			case '1':
				i->mnp = (char *)1;
				break;
			case '2':
				i->mnp = (char *)2;
				break;
			default:
				error ("bad opcode reference");
				break;
		} else {
			mnem = grab (count+1);
			strncpy (mnem, c, count);
			*(mnem+count) = '\0';
#if 0 && defined DEBUG
			debug("\nmnem: \"%s\"", mnem);
#endif
			i->mnp = mnem;
		}

		c += count + 1;
		if (idx = index(c, ' ')) {
			count = (idx - c);
		} else {	/* this is the last operand */
			count = strlen(c) - 1;	/* eat the newline */
		}


		if (count == 0) {
			i->opp = empty;	/* empty operands use the same string */
		} else if (count == 1 && *c == '*') {
			i->opp = NULL;	/* "don't care" */
		} else if (*c == '^') switch (*(c+1)) {
			case '0':
				i->opp = NULL;
				break;
			case '1':
				i->opp = (char *)1;
				break;
			case '2':
				i->opp = (char *)2;
				break;
			default:
				error ("bad operand reference");
				break;
		} else {
			oper = grab (count+1);
			strncpy (oper, c, count);
			*(oper+count) = '\0';
#if 0 && defined DEBUG
			debug("\noper: \"%s\"", oper);
#endif
			i->opp = oper;
		}
		c += count + 1;

		if (strlen(c) < 2) { /* we're at the end */
			return i;
		} else {	/* make another instruction to fill */
			instr *i2 = grab (sizeof (instr));

			i2->nxtins = i;
			i = i2;
		}
	}
	return i;
}

loadfile(patfile)
FILE *patfile;
{
	register action *a, **slot;
	char line[LINESIZE];
	int which = 1;

	while (fgets (line, LINESIZE, patfile)) {
		char *c = line;

		if (*c == '#') {
			continue;	/* comment line */
		}
		if (*c == '\n') {
			which = 1;
			continue;
		}

		switch (which) {
			case 1:	{
				instr *i = mkInstr (line);
				a = grab (sizeof (action)); /* new action */
				a->actp = i;
				break;
			}
			case 2:	{
				instr *i = mkInstr (line);
				a->repp = i;
#ifdef DEBUG
				dbgaction(a);
#endif
				if (!a->actp->mnp || index(a->actp->mnp, '*') || index(a->actp->mnp, '?')) {
					slot = &acslots[0];
				} else {
					slot = &acslots[hash (a->actp->mnp)];
				}
				a->nxtact = *slot;
				*slot = a;

				break;
			}
		}

		which++;
	}
}

optinit()
{
	char	path[255];
	char	*dir = CONFDIR;
	char	*pat = "%s/level%d.patterns";
	char	*err = "can't open %s\n";
	FILE	*patfile;

	sprintf(path, pat, dir, 1);
	if (!(patfile = fopen (path, "r")))
		error (err, path);

	loadfile(patfile);
	fclose(patfile);

	if (l2flag) {
		sprintf(path, pat, dir, 2);
		if (!(patfile = fopen (path, "r")))
			error (err, path);

		loadfile(patfile);
		fclose(patfile);
	}
}


#define isconbra(a) (((a) & BRAMASK) > (BRANCH|SUBR))
optim(i)
register instruction *i;
{
    register instruction *i2;
    action *a;
    instr *ap, *mp;
    char labc, c, found, fallback = FALSE;

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
                    goto loop; /* TODO: use continue? */
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
        if (!(a = acslots[hash(i2->mnem)])) {
            a = acslots[0];
            fallback = TRUE;
        }

restart:
        for (found = FALSE; a ; a = a->nxtact) {
#ifdef DEBUG
            debug("  matching action %p.\n",a);
#endif
            i = i2;
            labc = c = 0;
            for (ap = a->actp; ap; i = i->pred, ap = ap->nxtins, ++c) {
                if (i == &ilist) break;
                if (!labc && i->llist) labc = c; /* has label(s) at pos c */
                if (labc && labc < c) {
#ifdef DEBUG
                    debug("    inside label prevents match.\n");
#endif
                    break;
                }
                if (ap->mnp && match(i->mnem,ap->mnp) == 0) break;
                if (ap->opp) {
                    if ((int)ap->opp == 1) {
                        if (i->pred == &ilist) break;
                        if (match(i->args,i->pred->args) == 0) break;
                    } else if (match(i->args,ap->opp) == 0) break;
                }
#ifdef DEBUG
                debug("    matched: %s %s <=> %s %s\n",i->mnem,i->args,
                               strinst(ap->mnp), strinst(ap->opp));
#endif
            }
            if (ap == NULL) {
                found = TRUE;
                break;
            }
        }

        if (!found) {
            if (!fallback) {
                fallback = TRUE;
                a = acslots[0];
                goto restart;
            }
            return;
        }
#ifdef DEBUG
        debug("  all matched action %p.\n",a);
#endif

        /* all matched - replace */
		i = i2;
		for (ap = a->repp, mp = a->actp; ap; ap = ap->nxtins,mp = mp->nxtins) {
			char *ts;
			if (ap->mnp) {
				if ((int)ap->mnp == 1) /* do nothing, same as NULL */;
				else if ((int)ap->mnp == 2) strcpy(i->mnem,i->pred->mnem);
				else if (ts = index(ap->mnp, '\\')) {	// mnp has \#
					if (*(ts+1) > '0' && *(ts+1) < '3')
						matchcpy (ap->mnp,
						          i->mnem, mp->mnp,
						          (i->pred) ? i->pred->mnem : 0,
						          (mp->nxtins) ? mp->nxtins->mnp : 0);
					else error("bad opcode reference");
				} else {
					strcpy(i->mnem,ap->mnp);
				}
            }
            if (ap->opp) {
				if ((int)ap->opp == 1) /* do nothing, same as NULL */;
				else if ((int)ap->opp == 2) strcpy(i->args,i->pred->args);
				else if (ts = index(ap->opp, '\\')) {	// opp has \#
					if (*(ts+1) > '0' && *(ts+1) < '3')
						matchcpy (ap->opp,
						          i->args, mp->opp,
						          (i->pred) ? i->pred->args : 0,
						          (mp->nxtins) ? mp->nxtins->opp : 0);
					else error("bad operand reference");
				} else {
					strcpy(i->args,ap->opp);
				}
            }
            i = i->pred;
            --c;
        }

        /*
         * delete the instructions we don't need any more, moving any labels
         * we find to the new first instruction
         */
        for (i2 = i, i = i->succ; c; --c) {
            if (i2->llist) movlab(i2,i);
            i2 = i2->pred;
            remins(i2->succ);
            ++opsdone;
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
