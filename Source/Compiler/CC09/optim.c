/*
     Modification history for optim.c:
        22-May-83       Install better tree folder.
        25-May-83       Make better use of register variables.
        11-Apr-84       Conversion for UNIX.
        11-Feb-86 LAC   fixed array subscript ordering bug.
*/

#include "cj.h"
#define swap(type,x,y)  {type t;t = x;x = y;y = t;}


expnode *
optim(tree)
register expnode *tree;
{
    register expnode *lhs,*rhs,*tptr;
    int op;

#ifdef DEBUG
/*    fprintf(stderr,"**optim: entry\n");*/
#endif
    if (tree) {
#ifdef PTREE
        if (dflag && tcount++==0) prtree(tree,"optim (before):");
#endif
        tree->left=optim(tree->left);
        tree->right=optim(tree->right);

        tree = chtype(tree);

        if (isleaf(tree)) goto exit;

        tree = fold(tree);
        lhs = tree->left;
        rhs = tree->right;

        if ((op = tree->op) == QUERY && lhs->op == CONST) {
            if (lhs->val.num) {
                tptr = rhs->left;
                reltree(rhs->right);
            } else {
                tptr = rhs->right;
                reltree(rhs->left);
            }
            release(rhs);
            release(lhs);
            release(tree);
            tree = tptr;
        } else if ((op == STAR && lhs->op == AMPER)
                            || (op == AMPER && lhs->op == STAR)) {
            tptr = lhs->left;
            tptr->type = tree->type;
            tptr->size = tree->size;
            release(tree);
            release(lhs);
            tree=tptr;
        }
exit:   ;
#ifdef PTREE
        if (dflag && --tcount==0) prtree(tree,"optim (after):");
#endif
    }
#ifdef DEBUG
/*    fprintf(stderr,"**optim: exit\n");*/
#endif
    return tree;
}


expnode *
fold(tree)
register expnode *tree;
{
    register expnode *lhs,*rhs,*ptr;
    register int op,opl,opr;

    if (!tree) return NULL;

    lhs = tree->left;
    rhs = tree->right;

    if (isbin(op = tree->op) || op == DBLAND || op == DBLOR) {
        if ((opl = (lhs->op == CONST)) & (opr = (rhs->op == CONST)))
            goto doop;
        /* here, either lhs or rhs (or both) is not a constant */
        if (opl)
            switch (op) {
                /* comutative operators */
                case PLUS:
                case TIMES:
                case AND:
                case OR:
                case XOR:
                    /* the right node is not a constant - see above
                     * we are free to swap left and right
                     * to make life easier below.
                     */
                    ptr = lhs;
                    tree->left = lhs = rhs;
                    tree->right = rhs = ptr;
                    opr = 1;
                    /* now the left node is not a constant but the
                     * right node is.
                     */
            }

        switch (op) {
            case OR:
            case XOR:
                if (opr && rhs->val.num == 0) goto justleft;
                break;
            case PLUS:
                if (opr) {
                    /* right node constant - three cases can be collapsed */
                    if (rhs->val.num) {
                        switch (lhs->op) {
                            /* two cases here */
                            case AMPER:
                                lhs->left->modifier += rhs->val.num;
justleft:                       /* delete tree and rhs */
                                ptr=lhs;
                                release(tree);
                                release(rhs);
                                tree=ptr;
                                break;
                            case PLUS:
                                /* the constant subtractions lower
                                 * in the tree have gone
                                 */
                                if (lhs->right->op == CONST) {
                                    lhs->right->val.num += rhs->val.num;
                                    goto justleft;
                                }
                        }
                    } else
                        /* adding zero */
                        goto justleft;
                        /* no other cases can be collapsed */
                }
                break;
            case MINUS:
                if (opr) {
                    rhs->val.num = -rhs->val.num;
                    tree->op = PLUS;
                    return fold(tree);
                } else if (opl && lhs->val.num == 0) {
                    tree->op=NEG;
                    tree->left=rhs;
                    tree->right=NULL;
                    release(lhs);
                }
                break;
            case AND:
                if (opr && rhs->val.num == 0) goto makezero;
                break;
            case TIMES:
                if (opr)
                    switch (rhs->val.num) {
                        case 0:   goto makezero;
                        case 1:   goto justleft;
                        default:
                            if (lhs->op == TIMES
                                        && lhs->right->op == CONST) {
                                lhs->right->val.num *= rhs->val.num;
                                goto justleft;
                            }
                    }
                break;
            case DIV:
                if (opr && rhs->val.num == 1) goto justleft;
                else if (opl && lhs->val.num == 0) {
makezero:           tree->op=CONST;
                    tree->val.num=0;
                    tree->left=tree->right=NULL;
                    release(lhs);
                    release(rhs);
                }
                break;
        }
    } else
        switch (op) {
            case NOT:
            case COMPL:
            case NEG:
                if(lhs->op==CONST) {
doop:               tree->op=CONST;
                    tree->sux = 0;
                    tree->val.num=constop(op,lhs->val.num,rhs ? rhs->val.num : 0);
                    tree->left=tree->right=NULL;
                    release(lhs);
                    release(rhs);
                } else if(lhs->op == LCONST) {
                    long *p;

                    if ((p = lhs->val.lp) == NULL)
                        p = (long *) grab(sizeof(long));

                    switch (op) {
                        case NEG:      *p = -*p; break;
                        case COMPL:    *p = ~*p; break;
                    }
                    lhs->val.lp = p;
                    release(tree);
                    tree = lhs;
                }
#ifdef DOFLOATS
                else if (lhs->op == FCONST) {
                    double *p;

                    if (p = lhs->val.dp)  /* don't bother if zero */
                        *p = -*p;      /* can only be NEG */
                    release(tree);
                    tree = lhs;
                }
#endif
        }
    return tree;
}


isleaf(node)
register expnode *node;
{
    if (node)
        switch (node->op) {
#ifdef USE_YREG
            case YREG:
#endif
            case UREG:
            case CONST:
            case LCONST:
#ifdef  DOFLOATS
            case FCONST:
#endif
            case NAME:
            case STRING:
                return 1;
        }
    return 0;
}


diverr()
{
    error("divide by zero");
}


/* mammoth tree type checker.
 * checks for correct descendant nodes,
 * rewrites tree and converts types where neccessary,
 * fills in various node fields according to types and operators.
 * tree should not present any problems to the translator after this.
 */
expnode *
chtype(node)
expnode *node;
{
    register expnode *lhs,*rhs,*eptr;
    register symnode *sptr;
    register dimnode *dptr;
    register int op,size,xtype;
    int t,tl,tr,mosar;

    lhs = node->left;
    rhs = node->right;

    /* defaults */
    xtype = size = INTSIZE;
    dptr = NULL;
    t = INT;

#ifdef PTREE
    if (dflag) {
        putchar('\n');
        pnode(node);
    }
#endif
    switch (op = node->op) {
        case NAME:
            sptr = node->val.sp;
            if (sptr->storage == TYPEDEF) {
                terror(node,"typedef - not a variable");
                makedummy(node);
                break;
            }
            t = sptr->type;
            dptr = sptr->dimptr;
            if (btype(t) == USTRUCT) {
                register symnode *p = (symnode *) sptr->size;
                t = sptr->type = modtype(t,p->type);
                sizeup(sptr,dptr,p->size);
            }
            size = sptr->size;
            if (isary(t) || isftn(t)) {
                lhs = node;
                lhs->size = size;
                node = newnode(AMPER,lhs,0,0,node->lno,node->pnt);
                if (isary(t)) {
                    dptr = dimwalk(dptr);
                    t = incref(lhs->type = decref(t));
                } else t = incref(lhs->type = t);
                lhs->sux = 1;
            } else
                switch (tl = sptr->storage) {
#ifdef USE_YREG
                    case YREG:
#endif
                    case UREG:
                        node->op = tl;
                        node->val.num = 0;
                }
            xtype = 1;
            break;
        case CONST:
            xtype = 0;
            break;
        case LCONST:
            t = LONG;
            xtype = 1;
            size = LONGSIZE;
            break;
#ifdef DOFLOATS
        case FCONST:
            t = DOUBLE;
            xtype = 1;
            size = DOUBLESIZE;
            break;
#endif
        case STRING:
            t = POINTER | CHAR;
            size = 1;
            break;
        case CAST:
            chkdecl(lhs);
            if (isftn(t = node->type) || isary(t)) {
                terror(node,"cannot cast");
                t = incref(t);
            }
            if (isptr(t)) cvt(lhs,INT);
            else t = cvt(lhs,t);
            dptr = node->dimptr;
            size = node->size;
            release(node);
            node = lhs;
            break;

        case AMPER:
            needlval(lhs,1);
#ifdef USE_YREG
            if (lhs->op == YREG || lhs->op == UREG) {
#else
            if (lhs->op == UREG) {
#endif
                terror(node,"can't take address");
                makedummy(lhs);
            }
            xtype = lhs->sux;
            size = lhs->size;
            t = incref(lhs->type);
            dptr = lhs->dimptr;
            node->val.num = 0;
            break;
        case STAR:
            dptr = lhs->dimptr;
            if (isptr(t = lhs->type)) {
                t = decref(t);
                if (isary(t)) {
                    t = incref(decref(t));
                    release(node);
                    node = lhs;
                    dptr = dptr ? dptr->dptr : 0;
                } else node->val.num = 0;
            } else {
                terror(lhs,"pointer required");
                makedummy(lhs);
                lhs->type = POINTER | INT;
                t = INT;
                dptr = NULL;
            }
            xtype = lhs->sux;
            size = lhs->size;
            break;
        case DOT:
            size = needmos(rhs,&t,&mosar);
            dptr = rhs->dimptr;
            needlval(lhs,1);
            if (rhs->val.num == 0 && !mosar) {
                release(rhs);
                release(node);
                node = lhs;
                xtype = node->sux;
            } else {
                xtype = lhs->sux;
                if (lhs->op == STAR) {
                    eptr = lhs->left;
                    release(lhs);
                    node->left = lhs = eptr;
                } else {
                    tl = lhs->type;
                    lhs = node->left =
                            newnode(AMPER,lhs,0,0,lhs->lno,lhs->pnt);
                    lhs->type = incref(tl);
                }
                lhs->sux = xtype;
                goto strcommon;
            }
            break;
        case ARROW:
            size = needmos(rhs,&t,&mosar);
            dptr = rhs->dimptr;
            if ((tl = lhs->type) != INT && isptr(tl) == 0) {
                terror(lhs,"pointer or integer required");
                lhs->op = CONST;
                lhs->val.num = 0;
                lhs->sux = 0;
            }
            xtype = lhs->sux;
strcommon:
            node->op = PLUS;
            node = fold(node);
            if (!mosar) {
                node->type = incref(t);
                node->sux = xtype;
                node->size = size;
                node=newnode(STAR,node,0,0,node->lno,node->pnt);
            }
            break;
        case CALL:
            t = lhs->type;
            if (lhs->op == AMPER && isftn(decref(t))){
                eptr = node->left = lhs->left;
                release (lhs);
                t = decref(eptr->type);
            } else if (isftn(t)) t = decref(t);
            else if (t == UNDECL) {
                sptr = lhs->val.sp;
                sptr->type = INT | FUNCTION;
                sptr->size = INTSIZE;
                sptr->dimptr = NULL;
                sptr->storage = EXTERN;
                sptr->blklev = 0;
                move(sptr,lhs,6);
                t = INT;
            } else {
                terror(lhs,"not a function");
                t = btype(t);
            }
            dptr = lhs->dimptr;
            size = lhs->size;
        case ARG:
            chkdecl(lhs);
            chkstrct(lhs);
            switch (lhs->type) {
                case CHAR: cvt(lhs,INT); break;
                case UCHAR: cvt(lhs,UNSIGN); break;
#ifdef  DOFLOATS
                case FLOAT: cvt(lhs,DOUBLE); break;
#endif
            }
            break;
        case COMMA:
            size = rhs->size;
            t = rhs->type;
            dptr = rhs->dimptr;
            break;
        case NOT:
            if (!isptr(lhs->type)) usual(lhs);
            t = INT;
            break;
        case NEG:
            t = usual(lhs);
            break;
        case COMPL:
#ifdef  DOFLOATS
            if ((t = usual(lhs)) == DOUBLE) {
                integerr(lhs);
                makedummy(lhs);
            }
#else
            t = usual(lhs);
#endif
            break;
        case INCBEF:
        case INCAFT:
        case DECBEF:
        case DECAFT:
            needlval(lhs,0);
            size = lhs->size;
            node->val.num = (t = lhs->type) & XTYPE ?
                    getsize(decref(t),lhs->size,lhs->dimptr) : 1;
            xtype = lhs->sux;
            if (lhs->op == STAR) ++xtype;
            break;
        case DBLAND:
        case DBLOR:
            if (!isptr(lhs->type)) usual(lhs);
            if (!isptr(rhs->type)) usual(rhs);
            break;
        case DIV:
        case TIMES:
            t = tymatch(lhs,rhs);
            node->val.num = 0;
            break;
        case AND:
        case OR:
        case XOR:
        case MOD:
#ifdef  DOFLOATS
            if ((t = tymatch(lhs,rhs)) == DOUBLE) goto dblerr;
#else
            t = tymatch(lhs,rhs);
#endif
            node->val.num = 0;
            break;
        case SHL:
        case SHR:
#ifdef  DOFLOATS
            if ((t = usual(lhs)) == DOUBLE || usual(rhs) == DOUBLE) {
dblerr:         terror(node,"both must be integral");
                makedummy(node);
                break;
            }
#else
            t = usual(lhs);
            usual(rhs);
#endif
            cvt(rhs,INT);
            break;
        case LT:
        case GT:
        case LEQ:
        case GEQ:
            if ((isptr(lhs->type) && isptr(rhs->type))
                || tymatch(lhs,rhs) == UNSIGN || tymatch(lhs,rhs) == ULONG)
                node->op = op + (ULEQ-LEQ);
            break;
        case EQ:
        case NEQ:
            if (isptr(lhs->type)) {
                if (isptr(rhs->type) == 0) {
                    usual(rhs);
                    cvt(rhs,INT);
                }
            } else tymatch(lhs,rhs);
            break;
        case PLUS:
            if (isptr(rhs->type)) {
                swap(expnode *,lhs,rhs);
                node->left = lhs;
            }
            size = lhs->size;
            if (isptr(t = lhs->type)) {
                dptr = lhs->dimptr;
                goto dofix;
            } else t = tymatch(lhs,rhs);
            xtype = lhs->sux + rhs->sux;
            node->val.num = 0;
            break;
        case MINUS:
            xtype = lhs->sux + rhs->sux;
            if (isptr(t = lhs->type)) {
                size = lhs->size;
                dptr = lhs->dimptr;
                if (isptr(rhs->type)) {
                    node->sux = xtype;
                    if (t != rhs->type || size != rhs->size)
                        terror(node,"pointer mismatch");
                    else {
                        t = decref(t);
                        size = getsize(t,size,dptr);
                        if (size != 1) {
                            xtype = 2;
                            eptr = newnode(CONST,0,0,size,0,0);
                            node = newnode(DIV,node,eptr,0,
                                                        node->lno,node->pnt);
                        }
                    }
                    size = POINTSIZE;
                    dptr = NULL;
                    t = INT;
                } else {
dofix:
                    usual(rhs);
                    if (!isint(rhs->type)) {
                        integerr(rhs);
                        makedummy(rhs);
                    }
                    cvt(rhs,INT);
                    node->right = fixup(size,t,dptr,rhs);
                    xtype = lhs->sux + rhs->sux;
                }
            } else t = tymatch(lhs,rhs);
            break;
        case ASSIGN:
            needlval(lhs,0);
            chkdecl(rhs);
            tr = chkstrct(rhs);
            if ((isptr(t = lhs->type) && !isptr(tr) && !isint(tr))
                            || (isptr(tr) && !isptr(t) && !isint(t)))
                goto asserr;
            cvt(rhs,t);
            goto asscommon;
chass:
            needlval(lhs,0);
            usual(rhs);
            tr = chkstrct(rhs);
            if (isptr(t = lhs->type))
                switch(op) {
                    case ASSMIN:
                    case ASSPLUS:
                        cvt(rhs,INT);
                        goto do_cht;
                    default:
                        goto asserr;
                }
            else switch (op) {
                    default:
#ifdef  DOFLOATS
                        if (t == FLOAT) cvt(rhs,DOUBLE);
                        else
#endif
                            cvt(rhs, t);
                    case ASSSHL:
                    case ASSSHR:
do_cht:
                        node->op = op + (PLUS - ASSPLUS);
                        node = chtype(node);
                        if (t == CHAR || t == UCHAR) {
                            node->left = lhs->left;
                            release(lhs);
                            lhs = node->left;
                            if (t == UCHAR) t = UNSIGN;
                            else t = INT;
                        }
                        if (node->op == op + (PLUS - ASSPLUS)) node->op = op;
                }
asscommon:
            size = lhs->size;
            xtype = lhs->sux + rhs->sux;
            dptr = lhs->dimptr;
            break;
asserr:
            terror(node,"type mismatch");
            break;
        case COLON:
            if (isptr(t = lhs->type)) {
                size = lhs->size;
                xtype = lhs->sux;
                dptr = lhs->dimptr;
                if (isptr(rhs->type)) {
                    if (t != rhs->type || lhs->size != rhs->size)
                        terror(node,"pointer mismatch");
                    else dptr = NULL;
                } else chknull(rhs);
            } else if (isptr(t = rhs->type)) {
                size = rhs->size;
                xtype = rhs->sux;
                dptr = rhs->dimptr;
                chknull(lhs);
            } else t = tymatch(lhs,rhs);
            break;
        case QUERY:
            t = rhs->type;
            size = rhs->size;
            dptr = rhs->dimptr;
            break;
        default:
            if (op >= ASSPLUS) goto chass;
            comperr(node,"type check");
    }
    node->type = t;
    node->size = size;
    node->sux = xtype;
    node->dimptr = dptr;

#ifdef PTREE
    if (dflag) pnode(node);
#endif

    return node;
}


chknull(node)
register expnode *node;
{
    if (node->op != CONST || node->val.num != 0) {
        terror(node,"should be NULL");
        node->op = CONST;
        node->val.num = 0;
    }
}


usual(node)
register expnode *node;
{
    register int t;

    chkdecl(node);
    switch (t = node->type) {
        case CHAR:  cvt(node,t = INT); break;
        case UCHAR: cvt(node,t = UNSIGN); break;
#ifdef  DOFLOATS
        case FLOAT: cvt(node,t = DOUBLE); break;
        case DOUBLE:
#endif
        case LONG:
        case ULONG:
        case INT:
        case UNSIGN: break;
        default: terror(node,"type error");
            t = INT;
   }
   return (node->type = t);
}


cvt(node,t)
register expnode *node;
{
    expnode *ptr;
    DBLETYPE *dblptr;
    int op;

    op=0;
#ifdef DEBUG
/*    fprintf(stderr,"**cvt: entry type=%d\n",node->type);*/
#endif
    switch (node->type) {
        case UCHAR:
        case CHAR:
            switch (t) {
                case INT:
                case UNSIGN:
                    if (node->type == CHAR) op=CTOI;
                    else if (node->type == UCHAR) op=UTOI;
                    break;
                case LONG:
                    cvt(node,INT);
                    op=ITOL;
                    break;
                case ULONG:
                    cvt(node,UNSIGN);
                    op=UTOL;
#ifdef  DOFLOATS
                    break;
                case DOUBLE:
                case FLOAT:
                    cvt(node,INT);
                    cvt(node,t);
#endif
            }
            break;
        default:
            if (!isptr(node->type)) break;
        case INT:
            switch(t) {
                case LONG:
                case ULONG:
                    if (node->op == CONST) {
                        register long *ptr = (long *)grab(LONGSIZE);
                        *ptr = node->val.num;
                        node->val.lp = ptr;
fixl:
                        node->op = LCONST;
                        node->size = LONGSIZE;
                    } else op = ITOL;
                    break;
                case CHAR:
                    t=INT;
                    break;
                case UCHAR:
                    t=UNSIGN;
                    break;
#ifdef  DOFLOATS
                case FLOAT:
                    cvt(node,DOUBLE);
                    op = DTOF;
                    break;
                case DOUBLE:
                    if (node->op == CONST) {
                        dblptr = (DBLETYPE *) grab(DOUBLESIZE);
                        *dblptr = node->val.num;
fixf:
                        node->val.dp = dblptr;
                        node->op = FCONST;
                        node->size = DOUBLESIZE;
                        break;
                    }
                    op = ITOD;
                    break;
#endif
            }
            break;
        case UNSIGN:
            switch (t) {
                case LONG:
                case ULONG:
                    op=UTOL;
                    break;
#ifdef  DOFLOATS
                case FLOAT:
                    cvt(node,DOUBLE);
                    op=DTOF;
                    break;
                case DOUBLE:
                    op=UTOD;
                    break;
#endif
                case CHAR:
                case UCHAR:
                    t = INT;
                    break;
            }
            break;
        case LONG:
        case ULONG:
            switch (t) {
                default:
                    if (!isptr(t)) break;
                case INT:
                case UNSIGN:
                case CHAR:
                case UCHAR:
                    if (node->op==LCONST) {
                        node->val.num = *node->val.lp;
fixint:
                        node->op=CONST;
                        node->size = INTSIZE;
                    } else op=LTOI;
                    break;
#ifdef  DOFLOATS
                case FLOAT:
                    cvt(node,DOUBLE);
                    op=DTOF;
                    break;
                case DOUBLE:
                    if (node->op == LCONST) {
                        dblptr = grab(DOUBLESIZE);
                        *dblptr = *node->val.lp;
                        goto fixf;
                    }
                    op=LTOD;
                    break;
#endif
            }
            break;
#ifdef  DOFLOATS
        case FLOAT:
            switch (t) {
                case LONG:
                case ULONG:
                case UNSIGN:
                case CHAR:
                case UCHAR:
                case INT:
                    cvt(node,DOUBLE);
                    cvt(node,t);
                    break;
                case DOUBLE:
                    op=FTOD;
                    break;
            }
            break;
        case DOUBLE:
            switch (t) {
                case CHAR:
                case UCHAR:
                case UNSIGN:
                case INT:
                    if (node->op==FCONST) {
                        node->val.num = *(double *)(node->val.sp);
                        goto fixint;
                    }
                    op=DTOI;
                    break;
                case LONG:
                case ULONG:
                    if (node->op == FCONST) {
#ifdef DEBUG
/*                        fprintf(stderr,"**cvt: dtol start\n");*/
#endif
                        *node->val.lp = *node->val.dp;
#ifdef DEBUG
/*                        fprintf(stderr,"**cvt: dtol end\n");*/
#endif
                        goto fixl;
                    } else op=DTOL;
                    break;
                case FLOAT:
                    op=DTOF;
                    break;
            }
            break;
#endif
    }
    if (op) {
        ptr=newnode(0,0,0,0,0,0);
        nodecopy(node,ptr);
        node->op=op;
        node->left=ptr;
        node->right=NULL;
    }
#ifdef DEBUG
/*    fprintf(stderr,"**cvt: exit type=%d\n",t);*/
#endif
    return node->type = t;
}


tymatch(lhs,rhs)
register expnode *lhs,*rhs;
{
    register int tl,tr;

    tl = usual(lhs);
    tr = usual(rhs);
#ifdef  DOFLOATS
    if(tl == DOUBLE) return cvt(rhs,DOUBLE);
    else if (tr == DOUBLE) return cvt(lhs,DOUBLE);
    else
#endif
        if (tl == LONG) return cvt(rhs,LONG);
    else if (tr == LONG) return cvt(lhs,LONG);
    else if (tl == ULONG) return cvt(rhs,ULONG);
    else if (tr == ULONG) return cvt(lhs,ULONG);
    else if (tl == UNSIGN || tr == UNSIGN)
        return (lhs->type = rhs->type = UNSIGN);
    else return INT;
}


expnode *
fixup(size,t,dptr,rhs)
register expnode *dptr,*rhs;
{
    register expnode *ptr;

    if ((size = getsize(decref(t),size,dptr)) == 1) return rhs;

    ptr = newnode(CONST,0,0,size,rhs->lno,rhs->pnt);
    ptr->type = INT;
    ptr = fold(newnode(TIMES,rhs,ptr,0,rhs->lno,rhs->pnt));
    ptr->sux = (ptr->op == CONST) ? 0 : 2;
    ptr->type = INT;
    ptr->size = INTSIZE;

    return ptr;
}


needlval(node,flag)
register expnode *node;
{
    int op;

    switch (op=node->op) {
#ifdef USE_YREG
        case YREG:
#endif
        case UREG:
        case STAR:
            return;
    }

    if (op == NAME) {
        chkdecl(node);
        if (flag || (isary(node->type)==0 && node->type!=STRUCT))
            return;
    }
    terror(node,"lvalue required");
    makedummy(node);
}


chkdecl(node)
register expnode *node;
{
    if (node->op == NAME && node->type == UNDECL) {
        terror(node,"undeclared variable");
        makedummy(node);
    }
}


needmos(node,t,mosar)
register expnode *node;
int *t,*mosar;
{
    register expnode *eptr;
    register symnode *sptr;

    *t = node->type;
    if (node->op == AMPER) {
        *mosar = 1;
        eptr = node->left;
        eptr->dimptr = node->dimptr;
        nodecopy(eptr,node);
        release(eptr);
    } else *mosar = 0;
    chkdecl(node);
    if (node->op == NAME) {
        sptr = node->val.sp;
        if (sptr->storage != MOS) goto err;
        node->op = CONST;
        node->val.num = sptr->offset;
        node->sux = 0;
        node->type = INT;
        return sptr->size;
    }
err:
    terror(node,"struct member required");
    makedummy(node);
    node->op = CONST;
    node->val.num = 0;
    node->sux = 0;
    *t = INT;
    return INTSIZE;
}


chkstrct(node)
register expnode *node;
{
    if ((node->type) == STRUCT || node->type == UNION) {
        terror(node,"structure or union inappropriate");
        makedummy(node);
    }
    return node->type;
}


makedummy(node)
register expnode *node;
{
    move(&sdummy,node,6);
    node->sux = 1;
    reltree(node->left);
    reltree(node->right);
    node->left = node->right = 0;
    node->op = NAME;
    node->val.sp = &sdummy;
}


isint(t)
{
    switch(t){
        case INT:
        case CHAR:
        case UCHAR:
        case UNSIGN:
        case LONG:
        case ULONG:
            return 1;
    }
    return 0;
}


integerr(node)
{
    terror(node,"must be integral");
}
