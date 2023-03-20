#include "cj.h"

direct int shiftflag;
#ifdef REGCONTS
extern direct expnode *dregcont,*xregcont,*currexpr;
#endif
extern expnode *transexp();

#define swap(x,y) {expnode *t; t=x; x=y; y=t;}

/***************************************
 *                                     *
 *     expression translator           *
 *                                     *
 ***************************************/

lddexp(tree)
register expnode *tree;
{
    loadexp(tree);
    if (tree->op != DREG) {
        gen(LOAD,DREG,NODE,tree);
#ifdef  REGCONTS
        setdreg(tree);
#else
        tree->op = DREG;
#endif
    }
}


ldxexp(tree)
register expnode *tree;
{
    tranxexp(tree);
    if (tree->op != XREG || tree->val.num) {
        gen(LOAD,XREG,NODE,tree);
#ifdef  REGCONTS
        setxreg(tree);
#else
        tree->op = XREG;
#endif
    }
}


loadexp(tree)
register expnode *tree;
{
    switch(tree->type) {
        case LONG:
        case ULONG:
            lload(tree);
            break;
#ifdef  DOFLOATS
        case FLOAT:
        case DOUBLE:
            dload(tree);
            break;
#endif
        default:
            tranexp(tree);
            doload(tree);
    }
}


doload(tree)
register expnode *tree;
{
    switch (tree->op) {
        case STRING:
            gen(LOAD,XREG,NODE,tree);
#ifdef REGCONTS
            setxreg(NULL);
#endif
            tree->op = XREG;
            tree->val.num = 0;
            break;
        case AMPER:
            gen(LOADIM,XREG,NODE,tree->left);
#ifdef REGCONTS
            setxreg(tree);
#else
            tree->op = XREG;
#endif
            tree->val.num = 0;
            break;
        default:
#ifdef REGCONTS // yeah, yeah, I know...
/*          if (cmptrees(tree,dregcont)) {
                rplcnode(tree,DREG);
            } else
*/
#endif
#ifdef DEBUG
/*              prtree(tree,"doload (before gen)");*/
#endif
                gen(LOAD,DREG,NODE,tree);
#ifdef DEBUG
/*              prtree(tree,"doload (after gen)");  */
#endif
#ifdef REGCONTS
            if (tree->op == CTOI || tree->op == UTOI) {
                setdreg(tree->left);
            } else {
                setdreg(tree);
            }
#else
            tree->op=DREG;
#endif
            break;
        case XREG:
        case DREG:
        case UREG:
#ifdef USE_YREG
        case YREG:
#endif
            ;
    }
}


expnode *tranexp(tree)
register expnode *tree;
{
    register int op;

#ifdef DEBUG
    prtree(tree,"tranexp (before):");
#endif

#ifdef  REGCONTS
    setcurr(NULL);      /* clear current expression */
    if (cmptrees(tree,dregcont)) {
        rplcnode(tree,DREG);
        shiftflag = 1;
    } else if (cmptrees(tree,xregcont)) {
        rplcnode(tree,XREG);
        shiftflag = 1;
    } else
#endif  /* REGCONTS */
    if ((op=tree->op) == COMMA) {
        expnode *p = tree->right;

        reltree(tranexp(tree->left));
        nodecopy(p,tree);
        release(p);
        tranexp(tree);
    } else if (islong(tree)) tranlexp(tree);
#ifdef  DOFLOATS
    else if (isfloat(tree)) trandexp(tree);
#endif
    else if (isbin(op)) tranbinop(op,tree);
    else switch(op) {
            case STRING:
            case NAME:
#ifdef USE_YREG
            case YREG:
#endif
            case UREG:
            case AMPER:
            case CONST:
                break;
            case ASSIGN :
                doass(tree);
                break;
            case CTOI:
            case UTOI:
                tranexp(tree->left);
                break;
            case LTOI:
                tranlexp(tree->left);
                gen(LTOI,NODE,tree->left);
                tree->op = DREG;
                break;
#ifdef  DOFLOATS
            case DTOI:
                dload(tree->left);
                gen(DBLOP,DTOI);
                tree->op = DREG;
                break;
#endif

            case STAR :
                dostar(tree);
                break;
            case NOT:
            case DBLAND:
            case DBLOR:
                dobool(tree);
                break;
            case COMPL:
            case NEG:
                lddexp(tree->left);
                gen(op);
#ifdef REGCONTS
                setdreg(tree);      /* set D contents */
#else
                tree->op = DREG;
#endif
                break;
            case QUERY:
                doquery(tree,lddexp);
                tree->op = DREG;
                break;
            case INCBEF:
            case INCAFT:
            case DECBEF:
            case DECAFT:
                dotoggle(tree,DREG);
                break;
            case CALL:
                docall(tree);
                break;
            default:
                if (op >= ASSPLUS) {
                    assop(tree,op);
                    break;
                }
                comperr(tree,"translation");
                printf("%x\n",op);
        }
#ifdef DEBUG
        prtree(tree,"tranexp (after)");
#endif
    return tree;
}


tranbinop(op,node)
int op;
expnode *node;
{
    register expnode *lhs,*rhs;
    int s;

    lhs = node->left;
    rhs = node->right;

    if (isuint(lhs) && op >= DIV && op <= SHR)
		op -= (DIV-UDIV);
    else switch(op) {
            case PLUS:
            case MINUS:
                if (lhs->op == AMPER && rhs->op != CONST) {
                    tranxexp(node);
                    shiftflag = 0;
                    return;
                }
        }

    switch (op) {
        case MINUS:
            if (!isaleaf(rhs) ) {
                loadexp(rhs);
                gen(PUSH,rhs->op);
                rhs->op=STACK;
                lddexp(lhs);
            } else {
                lddexp(lhs);
                transexp(rhs);
            }
            gen(MINUS,DREG,NODE,rhs);
            break;

        case AND:
        case OR:
        case XOR:
        case PLUS:
            if ((isaleaf(lhs) && !isaleaf(rhs)) || lhs->op==CONST)
                swap(lhs,rhs);

            if (isreg(lhs->op)) {
                gen(PUSH,lhs->op);
                lddexp(rhs);
                rhs->op = STACK;
            } else {
                lddexp(lhs);
                if (!isaleaf(rhs)) {
                    gen(PUSH,DREG);
                    lddexp(rhs);
                    rhs->op=STACK;
                } else {
                    transexp(rhs);
                    if (op != PLUS) getinx(rhs);
                }
            }
            gen(op,DREG,NODE,rhs);
#ifdef  REGCONTS
            setdreg(node);
            shiftflag = 0;
            return;
#else
            break;
#endif
        case EQ:
        case NEQ:
        case GT:
        case LT:
        case LEQ:
        case GEQ:
        case UGT:
        case ULT:
        case ULEQ:
        case UGEQ:
            dobool(node);
            break;

        case USHR:
        case SHR:
        case SHL:
            if (rhs->op == CONST) {
                s = rhs->val.num;
doshift:        if ((unsigned)s <= 4) {
                    /* bytes of code in-line <= setting up for
                     * call of library routine
                     */
                    lddexp(lhs);

                    while (s--)
                        switch (op) {
                            case SHL:
                                gen(IDOUBLE);
                                break;
                            case SHR:
                                gen ((lhs->size == 1) ? CHALVE : HALVE);
                                break;
                            case USHR:
                                gen ((lhs->size == 1) ? UCHALVE : UHALVE);
                        }
                    shiftflag = 1;
                    goto out1;
                }
            }
            goto rest;

        case TIMES:
            if (lhs->op == CONST) swap(lhs,rhs);
        case DIV:
        case UDIV:
            if (rhs->op == CONST && (s = isashift(rhs->val.num))) {
                /* quicker by shifting for a power of two */
                rhs->val.num = s;
                /* fiddle operator for shifting */
                if (op == TIMES) op = SHL;
                else if (op == UDIV) op = USHR;
                else op = SHR;

                goto doshift;
            }
        case UMOD:
        case MOD:
rest:       loadexp(lhs);
            gen(PUSH,lhs->op);
            lddexp(rhs);
            gen(op);
#ifdef  REGCONTS
            setxreg(NULL);  /* clear X contents */
#endif
            break;

        default:
            comperr(node,"binary op.");
    }

out: shiftflag = 0;

out1: node->op = DREG;
#ifdef  REGCONTS
    setdreg(NULL);  /* clear D contents */
#endif
}


isashift(x)
{
    /* returns whether x is a suitable candidate for
    * shifting code in multiplication or division
    */
    register int i;
    static int masks[] = {
        2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384
    };

    for (i=0; i < 14 ;)
        if (x == masks[i++]) return i;

    return 0;
}


dobool(tree)
register expnode *tree;
{
    /*  to evaluate expressions of the form "e1 <rel> e2",
        other than in tests */
    labstruc l1,l2;

    l1.labnum = l2.labnum = 0;
#ifdef  REGCONTS
    l1.labdreg = l1.labxreg = l2.labdreg = l2.labxreg = NULL;
#endif

    /*  use jumping code to evaluate */
    tranbool(tree,&l1,&l2,TRUE);

    /*  put in 'true' condition */
    uselabel(&l1);
    gen(LOAD,DREG,CONST,1);
    gen(JMP,l1.labnum=getlabel(),1);

    /* put in 'false' condition */
    uselabel(&l2);
    gen(LOAD,DREG,CONST,0);

    /* finally the end label */
    label(l1.labnum);
    tree->op = DREG;
#ifdef  REGCONTS
    setdreg(NULL);         /* clear D contents */
#endif
}


doquery(tree,loadfunc)
register expnode *tree;
int (*loadfunc)();
{
    labstruc l1,l2;

    l1.labnum = l2.labnum = 0;
#ifdef  REGCONTS
    l1.labdreg = l1.labxreg = l2.labdreg = l2.labxreg = NULL;
#endif

    /* emit code for test */
    tranbool(tree->left,&l1,&l2,TRUE);

    /* move to ':' */
    tree = tree->right;

    /* put in the 'true' label */
    uselabel(&l1);
    /* and load the 'true' expression */
    (*loadfunc)(tree->left);
    /* then jump round the 'false' expression */
    gen(JMP,l1.labnum=getlabel(),0);

    /* put in the 'false' label */
    uselabel(&l2);
    /* and load the 'false' expression */
    (*loadfunc)(tree->right);

    /* finally the end label */
    label(l1.labnum);
#ifdef  REGCONTS
    clrconts();         /* clear register contents */
#endif
}


docall(node)
expnode *node;
{
    register expnode *lhs,*rhs;
    int oldsp;

    oldsp = sp;

    for (rhs = node; rhs = rhs->right;) {
        lhs = rhs->left;
        if (islong(lhs)) {
            if (lhs->op == LCONST) pushcon(lhs);
            else {
                lload(lhs);
                gen(LONGOP,STACK);
            }
        }
#ifdef DOFLOATS
        else if (isfloat(lhs)) {
            dload(lhs);
            gen(DBLOP,STACK);
        }
#endif
        else {
#ifdef REGPARMS
            if (rhs->right == NULL) break;  // don't push the first argument
#endif
            tranexp(lhs);
            switch (lhs->op) {
                default:
                    doload(lhs);
                case DREG:
                case XREG:
                case UREG:
#ifdef USE_YREG
                case YREG:
#endif
                    ;
            }
            gen(PUSH,lhs->op);
        }
    }

    tranexp(node->left);
#ifdef REGPARMS
    if (rhs) {
        lhs = rhs->left;
        if (isdleaf(lhs)) lddexp(lhs);
        else {
            op = node->left->op & NOTIND;
            if (op != YIND && op != UIND) stackx(node->left);
            lddexp(lhs);
        }
    }
#endif
    gen(CALL,NODE,node->left);

    sp = modstk(oldsp);

    node->op = DREG;
#ifdef REGCONTS
    clrconts();         /* clear register contents */
#endif
}


isdleaf(node)
register expnode *node;
{
    expnode *p;

    switch(node->op){
        case NAME:
        case CONST:
            return 1;
        case STAR:
            switch ((p = node->left)->op) {
                case CONST:
                case NAME:
#ifdef USE_YREG
                case YREG:
#endif
                case UREG:
                    return 1;
                default:
                    return regandcon(p);
            }
    }
    return 0;
}


regandcon(p)
register expnode *p;
{
    switch (p->op) {
        case PLUS:
        case MINUS:
            if (isreg(p->left->op) && p->right->op == CONST) return 1;
    }
    return 0;
}


isxleaf(node)
register expnode *node;
{
    return (node->sux<2);
}


isaddress(p)
register expnode *p;
{
    switch (p->op) {
        case PLUS:
        case MINUS:
            if (p->left->op != AMPER) break;
        case STRING:
        case AMPER:
            return 1;
    }
    return 0;
}


getinx(node)
register expnode *node;
{
    if (node->op & INDIRECT) {
        node->op&= NOTIND;
        gen(LOAD,XREG,NODE,node);
        node->op=XIND;
        node->val.num=0;
    }
}


doass(node)
register expnode *node;
{
    register expnode *lhs,*rhs,*savlhs;

    rhs = node->right;
    lhs = node->left;
    savlhs = NULL;
    if (isreg(lhs->op)) {
        if (regandcon(rhs)) tranxexp(rhs);
        else tranexp(rhs);
        switch (rhs->op) {
            case AMPER:
                gen(LOADIM,lhs->op,NODE,rhs->left);
                break;
            case CTOI:
            case UTOI:
                gen(LOAD,DREG,NODE,rhs);
            default:
                gen(LOAD,lhs->op,NODE,rhs);
        }
#ifdef REGCONTS
        if (treecont(dregcont,lhs->op)) setdreg(NULL);
        if (treecont(xregcont,lhs->op)) setxreg(NULL);
#endif
        node->op=lhs->op;
        node->val.num=0;
        return;
    } else {
#ifdef REGCONTS
        savlhs = treecopy(lhs);
#endif
        if (isdleaf(lhs) && lhs->type != CHAR && lhs->type != UCHAR
                        && (regandcon(rhs) || isaddress(rhs))){
            transexp(lhs);  /* this is tranexp in other source */
            loadxexp(rhs);
        } else if (isreg(rhs->op)) {
            transexp(lhs);  /* this is tranexp in other source */
            if (lhs->type==CHAR || lhs->type==UCHAR) lddexp(rhs);
        } else if (isxleaf(lhs)) {
            lddexp(rhs);
            transexp(lhs);  /* this is tranexp in other source */
        } else {
            tranexp(lhs);
            if (!isdleaf(rhs))
                switch(lhs->op & NOTIND) {
                    case UIND:
                    case YIND:
                        break;
                    default:
                        stackx(lhs);
                }
            loadexp(rhs);
        }
    }

#ifdef REGCONTS
    if (dregcont) {
        if (!(rhs->op == DREG && cmptrees(savlhs,dregcont)))
            if (subtree(savlhs,dregcont)) setdreg(NULL);
    }
    if (xregcont) {
        if (!(rhs->op == XREG && cmptrees(savlhs,dregcont)))
            if (subtree(savlhs,xregcont)) setxreg(NULL);
    }
    reltree(savlhs);
#endif

    gen(STORE,rhs->op,NODE,lhs);
    node->op=rhs->op;
    node->val.num=0;
}


expnode *
transexp(tree)
register expnode *tree;
{
    switch (tree->op) {
        case STAR:
            dostar(tree);
            break;
        default:
            tranexp(tree);
            break;
        case AMPER:
        case CONST:
        case NAME:
        case STRING:
        case UREG:
#ifdef USE_YREG
        case YREG:
#endif
            break;
    }
    return tree;
}


assop(node,op)
expnode *node;
{
    register expnode *lhs,*rhs;
#ifdef REGCONTS
    int inreg;
#endif

    lhs = node->left;
    rhs = node->right;

#ifdef REGCONTS
    inreg = 0;
    if (cmptrees(lhs,dregcont)) inreg = DREG;
    else if (cmptrees(lhs,xregcont)) inreg = XREG;
    else 
#endif
    tranexp(lhs);

    op += (PLUS - ASSPLUS);

    if (isuint (lhs) && op >= DIV && op <= SHR)
        op -= (DIV - UDIV);

    if (isreg(lhs->op)) {
        switch (op) {
            case PLUS:
            case MINUS:
                if (rhs->op == CONST) {
                    gen(LEA,lhs->op,CONST,(op == PLUS ? rhs->val.num
                                                      : -rhs->val.num));
                } else {
                    lddexp(rhs);
                    if (op == MINUS) {
                        gen(NEG);
#ifdef REGCONTS
                        setdreg(NULL);
#endif
                    }
                    gen(LEA,lhs->op,DREG);
                }
                node->op = lhs->op;
                node->val.num = 0;
                return;
            case AND:
            case OR:
            case XOR:
                goto ord;
            default:
                gen(PUSH,lhs->op);
                lddexp(rhs);
                gen(op);
        }
    } else {
#ifdef REGCONTS
        if (inreg == XREG) {
            node->op = XREG;
            node->val.num = 0;
            gen(LOAD,DREG,NODE,node);
            setdreg(NULL);
        } else if (inreg == 0) {
#endif
ord:
            gen(LOAD,DREG,NODE,lhs);
#ifdef REGCONTS
            setdreg(NULL);
        }
#endif
        if (lhs->type == CHAR) gen(CTOI);
        switch (op) {
            case AND:
            case OR:
            case XOR:
            case PLUS:
            case MINUS:
                if (isdleaf(rhs)) {
                    tranexp(rhs);
                    switch (op) {
                        case AND:
                        case OR:
                        case XOR:
                            getinx(rhs);
                    }
                    gen(op,DREG,NODE,rhs);
                    break;
                }
            default:
                if (
#ifdef REGCONTS
                    inreg == 0 && 
#endif
                    (lhs->op & NOTIND) != YIND
                        && (lhs->op & NOTIND) != UIND) stackx(lhs);
                gen(PUSH,DREG);
                lddexp(rhs);
                if (op == MINUS) op = RSUB;
                gen(op,DREG,STACK);
        }
    }
#ifdef REGCONTS
    if (inreg) {
        if (!isaleaf(lhs)) {
            gen(PUSH,DREG);
            transexp(lhs);
            gen(LOAD,DREG,STACK);
        } else transexp(lhs);
    }
#endif
    gen(STORE,DREG,NODE,lhs);
    node->op=DREG;
#ifdef REGCONTS
    setdreg(lhs);   /* set D contents */
#endif

}


stackx(node)
register expnode *node;
{
    if ((node->op & NOTIND) == NAME) return;
    getinx(node);
    if(node->val.num) gen(LEA,XREG,CONST,node->val.num);
    gen(PUSH,XREG);
    node->op=STACK | INDIRECT;
}


dostar(node)
register expnode *node;
{
    register expnode *lhs;
    register int op;

#ifdef DEBUG
/*  prtree(node,"dostar (before):");    */
#endif
    tranxexp(lhs = node->left);
    op = lhs->op;
    if (op & INDIRECT) {
        switch(op & NOTIND) {
#ifdef REGCONTS
            case NAME:
            case YIND:
            case UIND:
                if (currexpr) {
                    currexpr = newnode(STAR,currexpr,0,0,0,0);
# ifdef DEBUG
                    prtree(currexpr,"current expression (by dostar):");
# endif
                }
                setxreg(currexpr);
                gen(LOAD,XREG,op,lhs->val.num);
                break;
            case XIND:
                if (xregcont) {
                    xregcont = newnode(STAR,xregcont,0,0,0,0);
# ifdef DEBUG
                    prtree(xregcont,"X given (by dostar):");
# endif
                }
                gen(LOAD,XREG,op,lhs->val.num);
                break;
            default:
                comperr(lhs,"indirection");
#else /* !REGCONTS */
            case NAME:
            case XIND:
            case YIND:
            case UIND:
                gen(LOAD,XREG,op&NOTIND,lhs->val.num);
                break;
            default:
                comperr(lhs,"indirection");
#endif
        }
        op = XIND;
    } else {
        switch (op) {
            case UREG:
                op = UIND;
                goto fix;
#ifdef USE_YREG
            case YREG:
                op = YIND;
                goto fix;
#endif
            case XREG:
                op = XIND;
                goto fix;
            case XIND:
            case YIND:
            case UIND:
                op |= INDIRECT;
                goto fix;
            case NAME:
                op = NAME | INDIRECT;
                node->modifier = lhs->modifier;
fix:            node->val.sp = lhs->val.sp;
                break;
            case DREG:
                gen(LOAD,XREG,NODE,lhs);
#ifdef REGCONTS
                setxreg(NULL);
#else
                node->val.num = 0;
#endif
                op = XIND;
                break;
            case CONST:
                op = NAME;
                node->modifier = lhs->val.num;
//                node->val.num = 0;
                break;
            default:
                comperr(node,"indirection");
                op = XIND;
        }
    }
    node->op=op;
#ifdef DEBUG
/*  prtree(node,"dostar (after):");*/
#endif
}


dotoggle(node,dest)
register expnode *node;
register int dest;
{
    register expnode *lhs;
    int op, size, reg;
#ifdef REGCONTS
    register expnode *temp;
#endif

#ifdef DEBUG
/*  prtree(node,"dotoggle (before):");*/
#endif

    lhs = node->left;
#ifdef REGCONTS
    temp = treecopy(lhs);
    tranexp(lhs);
    if (lhs->op != DREG && lhs->op != XREG) {
        reltree(temp);
        temp = NULL;
    }
#else
    tranexp(lhs);
#endif

    op = node->op;
    if (isreg(reg = lhs->op)) {
        switch (op) {
            case INCAFT:
            case DECAFT:
                if (dest == DREG) gen(LOAD,dest,NODE,lhs);
                else {
            default:
                    dest = reg;
                }
        }
    } else {
        if (dest == XREG && (reg & NOTIND) != NAME) dest = DREG;
#ifdef REGCONTS
        if (reg = XREG) setxreg(NULL);
#endif
        gen(LOAD,dest,NODE,lhs);
        reg = dest;
    }
#ifdef DEBUG
#if 0
    printf("dotoggle: reg %s(%x) dest %s(%x)\n",kw[reg],reg,kw[dest],dest);
#endif
#endif

    size = node->val.num;

    switch (op) {
        case DECBEF:
        case DECAFT:
            size = -size;
        default:
            reg == DREG ? gen(PLUS,reg,CONST,size)
                        : gen(LEA,reg,CONST,size);
    }

#ifdef REGCONTS
    if (temp) {
        nodecopy(temp,lhs);
        release(temp);
        transexp(lhs);
    }
#endif
    gen(STORE,reg,NODE,lhs);

    switch (op) {
        case INCAFT:
        case DECAFT:
            if (reg == DREG) {
                gen(MINUS,DREG,CONST,size);
#ifdef REGCONTS
                setdreg(NULL);
#endif
            } else node->val.num = -size;
            break;
        default:
#ifdef REGCONTS
            if (reg == DREG) setdreg(lhs);
#endif
            node->val.num=0;
            break;
    }

    node->op = dest;
#ifdef DEBUG
/*  prtree(node,"dotoggle (after):");*/
#endif
    shiftflag = 0;
}

loadxexp(p)
register expnode *p;
{
    tranxexp(p);
    if (p->op != XREG || p->val.num) {
        gen(LOAD,XREG,NODE,p);
#ifdef REGCONTS
        setxreg(NULL);
#endif
    }
    p->op = XREG;
}

tranxexp(node)
expnode *node;
{
    register expnode *lhs,*rhs;
    int lhsval,rhsval,op,newop;

#ifdef REGCONTS
    setcurr(NULL);
    if (cmptrees(node,xregcont)) {
        rplcnode(node,XREG);
        return;
    }
#endif

    lhs = node->left;
    rhs = node->right;
    newop = XREG;
    switch (op = node->op) {
        case STAR:
            dostar(node);
        case NAME:
#ifdef USE_YREG
        case YREG:
#endif
        case UREG:
        case CONST:
        case STRING:
            return;
        case INCAFT: case INCBEF:case DECBEF:case DECAFT:
            dotoggle(node,XREG);
            return;
        case AMPER:
#ifdef DEBUG
            prtree(node,"tranxexp (before AMPER):");
#endif
            gen(LOADIM,XREG,NODE,lhs);
#ifdef REGCONTS
            setxreg(node);      /* set X contents */
#endif
            node->val.num = 0;
            break;
        case CTOI:
        case UTOI:
            lddexp(node);
            return;
        case MINUS:
            if (lhs->op != AMPER && rhs->sux) goto cant;
        case PLUS:
            if (isreg(lhs->op) || lhs->op == AMPER) {
#ifdef DEBUG
                prtree(node,"tranxexp (before):");
#endif
                if (rhs->op == CONST) {
                    tranxexp(lhs);
#ifdef REGCONTS
                    setcurr(node);
#endif
                    newop = lhs->op;
                    rhsval = rhs->val.num;
                } else {
                    lddexp(rhs);
                    tranxexp(lhs);
                    if (op == MINUS) {
                        gen(NEG);
#ifdef REGCONTS
                        setdreg(NULL);
#endif
                    }
                    gen(LEAX,DREG,lhs->op);
#ifdef REGCONTS
                    setxreg(node);
#endif
                    rhsval = 0;
                }
                lhsval = lhs->val.num;
            } else {
                if (op == PLUS && lhs->sux < rhs->sux) swap(lhs,rhs);
                if(!(isdleaf(rhs) || isreg(rhs->op))) goto cant;
#ifdef DEBUG
                prtree(node,"tranxexp (before):");
#endif
#ifdef REGCONTS
                if (cmptrees(lhs,xregcont)) {
                    lhsval = 0;
                } else {
#endif
                    tranxexp(lhs);
                    lhsval = lhs->val.num;
                    switch(lhs->op & NOTIND) {
                        case DREG:
                            if (op == PLUS && rhs->op != CONST) {
                                ldxexp(rhs);
                                lhsval = 0;
                                goto done;
                            }
                        case NAME:
                        case STRING:
                        case XIND:
                        case YIND:
                        case UIND:
                            gen(LOAD,XREG,NODE,lhs);
#ifdef REGCONTS
                            setxreg(lhs);
#endif
                            lhsval = 0;
                        case XREG:
                            break;
#ifdef USE_YREG
                        case YREG:
#endif
                        case UREG:
                            newop = lhs->op;
                            break;
                        default:
                            comperr(lhs,"x translate");
                    }
#ifdef REGCONTS
                }
#endif
                if (rhs->op == CONST) rhsval = rhs->val.num;
                else {
                    lddexp(rhs);
                    if (op == MINUS) {
                        gen(NEG,0,0);
#ifdef REGCONTS
                        setdreg(NULL);
#endif
                    }
done:               gen(LEAX,DREG,newop);
#ifdef REGCONTS
                    setxreg(node);
#endif
                    newop=XREG;
                    rhsval = 0;
                }
            }
            node->val.num = lhsval + (op == PLUS ? rhsval : -rhsval);
            break;
        default:
cant:       tranexp(node);
            return;
    }
    node->op=newop;
#ifdef DEBUG
    prtree(node,"tranxexp (after):");
#endif
}


isreg(r)
{
#ifdef USE_YREG
    return (r==UREG || r==YREG);
#else
    return (r==UREG);
#endif
}

