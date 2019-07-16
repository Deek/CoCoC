#include "cj.h"

direct int shiftflag;
extern direct expnode *dregcont,*xregcont,*currexpr;
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
        setdreg(tree);
    }
}


ldxexp(tree)
register expnode *tree;
{
    tranxexp(tree);
    if (tree->op != XREG) {
        gen(LOAD,XREG,NODE,tree);
        setxreg(tree);
    }
}


loadexp(tree)
register expnode *tree;
{
    switch(tree->type) {
        case LONG:
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
            setxreg(NULL);
            tree->op = XREG;
            tree->val.num = 0;
            break;
        case AMPER:
            gen(LOADIM,XREG,NODE,tree->left);
            setxreg(tree);
            tree->val.num = 0;
            break;
        default:
/*          if (cmptrees(tree,dregcont)) {
                rplcnode(tree,DREG);
            } else
*/
            if (tree->op == CTOI) {
                gen(LOAD,DREG,NODE,tree);
                setdreg(tree->left);
            } else {
#ifdef DEBUG
/*              prtree(tree,"doload (before gen)");*/
#endif
                gen(LOAD,DREG,NODE,tree);
#ifdef DEBUG
/*              prtree(tree,"doload (after gen)");  */
#endif
                setdreg(tree);
            }
            break;
        case XREG:
        case DREG:
        case UREG:
        case YREG:
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
    setcurr(NULL);      /* clear current expression */
    if (cmptrees(tree,dregcont)) {
        rplcnode(tree,DREG);
        shiftflag = 1;
    } else if (cmptrees(tree,xregcont)) {
        rplcnode(tree,XREG);
        shiftflag = 1;
    } else if ((op=tree->op) == COMMA) {
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
            case YREG:
            case UREG:
            case AMPER:
            case CONST:
                break;
            case ASSIGN :
                doass(tree);
                break;
            case CTOI:
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
                setdreg(tree);      /* set D contents */
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
    register expnode *lhs,*rhs,*temp;
    int s;

    lhs = node->left;
    rhs = node->right;

    if (lhs->type == UNSIGN)
        switch(op) {
            case DIV:
                op = UDIV;
                break;
            case MOD:
                op = UMOD;
                break;
            case SHR:
                op = USHR;
        }
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
            setdreg(node);
            shiftflag = 0;
            return;

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
                                gen(HALVE);
                                break;
                            case USHR:
                                gen(UHALVE);
                        }
                    shiftflag = 1;
                    goto out1;
                }
            }
            goto rest;

        case TIMES:
            if (lhs->op == CONST) swap(lhs,rhs);
        case UDIV:
            if (rhs->op == CONST && (s = isashift(rhs->val.num))) {
                /* quicker by shifting for a power of two */
                rhs->val.num = s;
                /* fiddle operator for shifting */
                op = (op == TIMES) ? SHL : USHR;
                goto doshift;
            }
        case DIV:
        case UMOD:
        case MOD:
rest:       loadexp(lhs);
            gen(PUSH,lhs->op);
            lddexp(rhs);
            gen(op);
            setxreg(NULL);  /* clear X contents */
            break;

        default:
            comperr(node,"binary op.");
    }

out: shiftflag = 0;

out1: node->op = DREG;

    setdreg(NULL);  /* clear D contents */
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
    l1.labdreg = l1.labxreg = l2.labdreg = l2.labxreg = NULL;

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
    setdreg(NULL);         /* clear D contents */
}


doquery(tree,loadfunc)
register expnode *tree;
int (*loadfunc)();
{
    labstruc l1,l2;

    l1.labnum = l2.labnum = 0;
    l1.labdreg = l1.labxreg = l2.labdreg = l2.labxreg = NULL;

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
    clrconts();         /* clear register contents */
}


docall(node)
expnode *node;
{
    register expnode *lhs,*rhs;
    int oldsp,op;

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
            if (rhs->right == NULL) break;
            tranexp(lhs);
            switch (lhs->op) {
                default:
                    doload(lhs);
                case DREG:
                case XREG:
                case UREG:
                case YREG:
                    ;
            }
            gen(PUSH,lhs->op);
        }
    }

    tranexp(node->left);
    if (rhs) {
        lhs = rhs->left;
        if (isdleaf(lhs)) lddexp(lhs);
        else {
            op = node->left->op & NOTIND;
            if (op != YIND && op != UIND) stackx(node->left);
            lddexp(lhs);
        }
    }
    gen(CALL,NODE,node->left);

    sp = modstk(oldsp);

    node->op = DREG;
    clrconts();         /* clear register contents */
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
                case YREG:
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
                gen(LOAD,DREG,NODE,rhs);
            default:
                gen(LOAD,lhs->op,NODE,rhs);
        }
        if (treecont(dregcont,lhs->op)) setdreg(NULL);
        if (treecont(xregcont,lhs->op)) setxreg(NULL);
        node->op=lhs->op;
        node->val.num=0;
        return;
    } else {
        savlhs = treecopy(lhs);
        if (isdleaf(lhs) && lhs->type != CHAR
                        && (regandcon(rhs) || isaddress(rhs))){
            transexp(lhs);
            loadxexp(rhs);
        } else if (isreg(rhs->op)) {
            transexp(lhs);
            if (lhs->type==CHAR) lddexp(rhs);
        } else if (isxleaf(lhs)) {
            lddexp(rhs);
            transexp(lhs);
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
    if (dregcont) {
        if (!(rhs->op == DREG && cmptrees(savlhs,dregcont)))
            if (subtree(savlhs,dregcont)) setdreg(NULL);
    }
    if (xregcont) {
        if (!(rhs->op == XREG && cmptrees(savlhs,dregcont)))
            if (subtree(savlhs,xregcont)) setxreg(NULL);
    }
    reltree(savlhs);
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
        case YREG:
            break;
    }
    return tree;
}


assop(node,op)
expnode *node;
{
    register expnode *lhs,*rhs;
    int inreg;

    lhs = node->left;
    rhs = node->right;

    inreg = 0;
    if (cmptrees(lhs,dregcont)) inreg = DREG;
    else if (cmptrees(lhs,xregcont)) inreg = XREG;
    else tranexp(lhs);

    op += (PLUS - ASSPLUS);
    if (lhs->type == UNSIGN)
        switch(op) {
            case DIV:
                op = UDIV;
                break;
            case SHR:
                op = USHR;
                break;
            case MOD:
                op = UMOD;
        }

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
                        setdreg(NULL);
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
        if (inreg == XREG) {
            node->op = XREG;
            node->val.num = 0;
            gen(LOAD,DREG,NODE,node);
            setdreg(NULL);
        } else if (inreg == 0) {
ord:
            gen(LOAD,DREG,NODE,lhs);
            setdreg(NULL);
        }
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
                if (inreg == 0 && (lhs->op & NOTIND) != YIND
                        && (lhs->op & NOTIND) != UIND) stackx(lhs);
                gen(PUSH,DREG);
                lddexp(rhs);
                if (op == MINUS) op = RSUB;
                gen(op,DREG,STACK);
        }
    }
    if (inreg) {
        if (!isaleaf(lhs)) {
            gen(PUSH,DREG);
            transexp(lhs);
            gen(LOAD,DREG,STACK);
        } else transexp(lhs);
    }
    gen(STORE,DREG,NODE,lhs);
    node->op=DREG;
    setdreg(lhs);   /* set D contents */

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
            case NAME :
            case YIND:
            case UIND:
                if (currexpr) {
                    currexpr = newnode(STAR,currexpr,0,0,0,0);
#ifdef DEBUG
                    prtree(currexpr,"current expression (by dostar):");
#endif
                }
                setxreg(currexpr);
                gen(LOAD,XREG,op,lhs->val.num);
                break;
            case XIND:
                if (xregcont) {
                    xregcont = newnode(STAR,xregcont,0,0,0,0);
#ifdef DEBUG
                    prtree(xregcont,"X given (by dostar):");
#endif
                }
                gen(LOAD,XREG,op,lhs->val.num);
                break;
            default:
            comperr(lhs,"indirection");
        }
        op = XIND;
    } else {
        switch (op) {
            case UREG:
                op = UIND;
                goto fix;
            case YREG:
                op = YIND;
                goto fix;
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
                setxreg(NULL);
                op = XIND;
                break;
            case CONST:
                op = NAME;
                node->modifier = lhs->val.num;
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
    register expnode *lhs, *temp;
    int op, size, reg, inreg;

#ifdef DEBUG
/*  prtree(node,"dotoggle (before):");*/
#endif
    temp = treecopy(lhs = node->left);
    tranexp(lhs);
    if (lhs->op != DREG && lhs->op != XREG) {
        reltree(temp);
        temp = NULL;
    }
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
        if (reg = XREG) setxreg(NULL);
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

    if (temp) {
        nodecopy(temp,lhs);
        release(temp);
        transexp(lhs);
    }
    gen(STORE,reg,NODE,lhs);

    switch (op) {
        case INCAFT:
        case DECAFT:
            if (reg == DREG) {
                gen(MINUS,DREG,CONST,size);
                setdreg(NULL);
            } else node->val.num = -size;
            break;
        default:
            if (reg == DREG) setdreg(lhs);
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
        setxreg(NULL);
    }
    p->op = XREG;
}

tranxexp(node)
expnode *node;
{
    register expnode *lhs,*rhs;
    int lhsval,rhsval,op,newop;

    setcurr(NULL);
    if (cmptrees(node,xregcont)) {
        rplcnode(node,XREG);
        return;
    }
    lhs = node->left;
    rhs = node->right;
    newop = XREG;
    switch (op = node->op) {
        case STAR:
            dostar(node);
        case NAME:
        case YREG:
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
            setxreg(node);      /* set X contents */
            node->val.num = 0;
            break;
        case CTOI:
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
                    setcurr(node);
                    newop = lhs->op;
                    rhsval = rhs->val.num;
                } else {
                    lddexp(rhs);
                    tranxexp(lhs);
                    if (op == MINUS) {
                        gen(NEG);
                        setdreg(NULL);
                    }
                    gen(LEAX,DREG,lhs->op);
                    setxreg(node);
                    rhsval = 0;
                }
                lhsval = lhs->val.num;
            } else {
                if (op == PLUS && lhs->sux < rhs->sux) swap(lhs,rhs);
                if(!(isdleaf(rhs) || isreg(rhs->op))) goto cant;
#ifdef DEBUG
                prtree(node,"tranxexp (before):");
#endif
                if (cmptrees(lhs,xregcont)) {
                    lhsval = 0;
                } else {
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
                            setxreg(lhs);
                            lhsval = 0;
                        case XREG:
                            break;
                        case YREG:
                        case UREG:
                            newop = lhs->op;
                            break;
                        default:
                            comperr(lhs,"x translate");
                    }
                }
                if (rhs->op == CONST) rhsval = rhs->val.num;
                else {
                    lddexp(rhs);
                    if (op == MINUS) {
                        gen(NEG,0,0);
                        setdreg(NULL);
                    }
done:               gen(LEAX,DREG,newop);
                    setxreg(node);
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
    return (r==UREG || r==YREG);
}

