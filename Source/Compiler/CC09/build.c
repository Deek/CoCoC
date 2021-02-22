/*
     Modification history for build.c:
          22-May-83      Fixed sizeof problem with undefined name.
          25-May-83      Made better use of register variables.
          19-Sep-83      Fixed assops so their associativity is the
                         same as that of assignment itself.
          26-Nov-83      Look for undefined struct tags in casts.
          10-Apr-84      Conversion for UNIX.
*/

#include "cj.h"

static elsize(prim);


expnode *
parsexp(priority)
int priority;
{
    register expnode *rhs,*lhs,*temp;
    register int op,priop,lno,rprec;
    char *errpnt;

    if ((lhs = primary()) == NULL) return NULL;

    while (isop() && priority <= symval) {
        op = sym;
        rprec = priop = symval;
        lno = symline;
        errpnt = symptr;
        getsym();

        switch(op) {
            default:
                if (op < ASSPLUS || op > ASSXOR) rprec = priop+1;
            case QUERY :
            case ASSIGN :
                ;
        }

        if (rhs = parsexp(rprec)) {
            if (op == QUERY) {
                if (need(COLON)) {
                    reltree(rhs);
                    goto exit;
                } else {
                    errpnt = symptr;
                    lno = symline;
                    if (temp = parsexp(LEV_3)) {
                        rhs = newnode(COLON,rhs,temp,LEV_3,lno,errpnt);
                    } else {
                        error("third expression missing");
                        reltree(rhs);
                        goto exit;
                    }
                }
            }
            lhs = newnode(op,lhs,rhs,priop,lno,errpnt);
        } else error("operand expected");
    }

exit:
    return lhs;
}


expnode *
primary()
{
    register expnode *nodep,*prim;
    expnode *temp;
    register int op,lno;
    register char *errpnt;

    nodep = NULL;

    switch (sym) {
        case STRING:
        case NAME:
        case LCONST:
#ifdef  DOFLOATS
        case FCONST:
#endif
        case CONST:
            nodep = newnode(sym,0,0,symval,symline,symptr);
            getsym();
            break;
        case LPAREN:
            getsym();
            if (istype()) {
                nodep = getcast();
                need(RPAREN);
                if (nodep->left = primary()) ;
                else {
                    release(nodep);
                    nodep=NULL;
                }
                break;
            } else if ((nodep = parsexp(LEV_0)) == NULL) {
noexp:
                experr();
                nodep = newnode(CONST,0,0,0,symline,symptr);
            }
            need(RPAREN);
            break;
        case NOT:
        case NEG:
        case COMPL:
        case STAR:
        case DECBEF:
        case INCBEF:
        case AMPER:
            op=sym;
            lno=symline;
            errpnt=symptr;
            getsym();
            if (prim = primary())
                nodep = newnode(op,prim,0,LEV_14,lno,errpnt);
            else error("primary expected");
            break;
        case SIZEOF:
            lno=symline;
            errpnt=symptr;
            getsym();

            if(sym==LPAREN){
                getsym();
                if (istype()) prim = getcast();
                else if ((prim = parsexp(LEV_0)) == NULL) goto noexp;
                need(RPAREN);
            } else prim = primary();
            temp = prim;
            nodep = newnode(CONST,0,0,elsize(&temp),lno,errpnt);
            reltree(temp);
            break;
    }
    if (nodep == NULL) return NULL;

    for ( ; ; ) {
        switch(sym) {
            case LPAREN:
                errpnt=symptr;
                lno=symline;
                getsym();
                nodep=newnode(CALL,nodep,explist(),LEV_15,lno,errpnt);
                need(RPAREN);
                continue;

            case LBRACK:
                getsym();
                if ((prim = parsexp(LEV_2)) == NULL) {
                    experr();
                    prim = newnode(CONST,0,0,0,symline,symptr);
                }
                nodep=newnode(PLUS,nodep,prim,LEV_12,symline,symptr);
                nodep=newnode(STAR,nodep,0,LEV_15,symline,symptr);
                need(RBRACK);
                continue;

            case DOT:
            case ARROW:
                op=sym;
                lno=symline;
                errpnt=symptr;
                ++mosflg;
                getsym();
                --mosflg;
                if (sym!=NAME) {
                    identerr();
                    break;
                }
                prim = newnode(sym,0,0,symval,symline,symptr);
                nodep = newnode(op,nodep,prim,LEV_15,lno,errpnt);
                getsym();
                continue;
        }
        break;
    }

    switch (sym){
        case INCBEF:
            sym=INCAFT;
            goto dblop;
        case DECBEF:
            sym=DECAFT;
dblop:      nodep = newnode(sym,nodep,0,LEV_14,symline,symptr);
            getsym();
    }

    return nodep;
}


expnode *
explist()
{
    register expnode *list = NULL, *ptr;

    for ( ; ; ) {
        if (sym == RPAREN) break;
        if (ptr = parsexp(LEV_2)){
            ptr = newnode(ARG,ptr,list,0,ptr->lno,ptr->pnt);
            list = ptr;
        }
        if (sym != COMMA) break;
        getsym();
    }
    return list;
}


constexp(p)
int p;
{
    register expnode *ptr;
    register int v;

    if ((ptr = optim(parsexp(p))) && ptr->op == CONST) v = ptr->val.num;
    else {
        v = 0;
        error("constant required");
    }
    if (ptr) reltree(ptr);
    return v;
}


isop()
{
    switch (sym) {
        case AMPER:
            sym=AND;
            symval=LEV_8;
            return 1;
        case STAR:
            sym=TIMES;
            symval=LEV_13;
            return 1;
        case NEG:
            sym=MINUS;
            symval=LEV_12;
        case COMMA:
        case ASSIGN:
        case QUERY:
            return 1;
        case COLON:
            return 0;
        default:
            return ((sym >= DBLAND && sym <= GT)
                            || (sym >= ASSPLUS && sym <= ASSXOR));
    }
}


constop(op,r,l)
{
    switch(op) {
        case PLUS:      return r + l;
        case MINUS:     return r - l;
        case TIMES:     return r * l;
        case DIV:       if (l) return r / l;
                        diverr();
                        return 0;
        case MOD:       if(l) return r % l;
                        diverr();
                        return 0;
        case AND:       return r & l;
        case OR:        return r | l;
        case XOR:       return r ^ l;
        case SHL:       return r << l;
        case SHR:       return r >> l;
        case EQ:        return r == l;
        case NEQ:       return r != l;
        case GT:        return r > l;
        case LT:        return r < l;
        case GEQ:       return r >= l;
        case LEQ:       return r <= l;
        case NEG:       return -r;
        case NOT:       return !r;
        case COMPL:     return ~r;
        case DBLAND:    return r && l;
        case DBLOR:     return r || l;
        case ULEQ:
        case ULT:
        case UGEQ:
        case UGT:
            {
                char *lp,*rp;
                lp = (char *) l;  rp = (char *) r;
                switch (op) {
                    case ULEQ:      return rp <= lp;
                    case ULT:       return rp < lp;
                    case UGEQ:      return rp >= lp;
                    default:        return rp > lp;
                }
            }
        default:        error("constant operator");
                        return 0;
    }
}


expnode *
getcast()
{
    expnode *dptr,*ptr;
    int lno,dummy,size,type;
    register char *errpnt;

    lno = symline;
    errpnt = symptr;
    type = settype(&size,&dptr,&dummy);
    if (type == USTRUCT) {
        error("undefined struct");
        ptr = NULL;
    } else {
        type = declarator(&ptr,&dptr,type);
        clear(&arglist);
        if (ptr) error("name in a cast");
        ptr = newnode(CAST,0,0,0,lno,errpnt);
        ptr->type = type;
        sizeup(ptr,dptr,size);
    }
    return ptr;
}


expnode *
newnode(op,left,right,value,lno,errpnt)
expnode *left,*right;
char *errpnt;
{
    register expnode *node;

    if (freenode) {
        node = freenode;
        freenode = node->left;
    } else node = (expnode *) grab(NODESIZE);
    node->op = op;
    node->left = left;
    node->right = right;
    node->val.num = value;
    node->lno = lno;
    node->pnt = errpnt;
    node->modifier = 0;

    return node;
}


experr()
{
    error("expression missing");
}


static
elsize(prim)
expnode **prim;
{
    register expnode *p;

    switch((p = *prim)->op) {
        default:
            p = *prim = optim(p);
            goto usual;
        case NAME:
            p->type = p->val.sp->type;
            chkdecl(p);
            p = (expnode *) p->val.sp;
        case CAST:
usual:      return getsize(p->type,p->size,p->dimptr);
        case DOT:
        case ARROW:
            return elsize(&p->right);
    }
}

