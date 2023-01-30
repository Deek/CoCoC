/*
     Modification history for bool.c:
          25-May-83      Rearranged register variable usage.
          29-Aug-83      Test on comma-separated expressions where
                         the right hand one was a float or a long
                         did not work.
          13-Sep-83 LAC  add register contents maintanance code
          17-Apr-84 LAC  Conversion for UNIX.
*/

#include "cj.h"

extern direct int shiftflag;
#ifdef REGCONTS
extern direct expnode *dregcont,*xregcont;
#endif


expnode *
tranbool(node,tlab,flab,nj)
register expnode *node;
labstruc *tlab,*flab;
{
    int op;
    labstruc second;

    second.labnum = second.labsp = 0;
#ifdef REGCONTS
    second.labdreg = second.labxreg = NULL;
#endif
    switch(op=node->op){
        case DBLAND:
            tranbool(node->left,&second,flab,TRUE);
            goto l1;
        case DBLOR:
            tranbool(node->left,tlab,&second,FALSE);
l1:         uselabel(&second);
            tranbool(node->right,tlab,flab,nj);
            break;
        case NOT:
            tranbool(node->left,flab,tlab,TRUE-nj);
            break;
        case EQ:
        case NEQ:
        case LEQ:
        case LT:
        case GEQ:
        case GT:
        case ULEQ:
        case ULT:
        case UGEQ:
        case UGT:
            tranrel(op,node,tlab,flab,nj);
            break;
        case CONST:
#ifdef  DOFLOATS
        case FCONST:
#endif
        case LCONST:
            if (node->val.num && nj == FALSE) gen(JMP,setlabel(tlab),0);
            else if (nj && node->val.num == 0) gen(JMP,setlabel(flab),0);
            break;
        case COMMA:
            tranexp(node->left);
            tranbool(node->right,tlab,flab,nj);
            break;
        default:
            if(islong(node)) {
                lload(node);
                gen(LONGOP,TEST);
            }
#ifdef  DOFLOATS
            else if(isfloat(node)) {
                if(node->op==FTOD) node=node->left;
                dload(node);
                gen(DBLOP,TEST,node->type);
            }
#endif
            else checkop(node);
usual:      gen(CNDJMP,(nj ? EQ:NEQ),setlabel(nj ? flab : tlab));
     }
}


tranrel(op,node,tlab,flab,nj)
expnode *node;
labstruc *tlab,*flab;
{
    labstruc *destin;
    register expnode *rhs,*lhs,*t;
    register int temp;

    lhs = node->left;
    rhs = node->right;
    destin = nj ? flab : tlab;

    op = nj ? invrel(op) : op;

    if (islong(lhs)) {
        tranlexp(node);
        goto usual;
    }
#ifdef DOFLOATS
    else if (isfloat(lhs)) {
        trandexp(node);
        goto usual;
    }
#endif
    if (zeroconst(lhs) || (isaleaf(lhs) && !isaleaf(rhs))
                       || (isreg(rhs->op) && !isreg(lhs->op))) {
        t = lhs; lhs = rhs; rhs = t;
        op = revrel(op);
    }

    if (isreg(temp=lhs->op)) {
        tranexp(rhs);
        switch (rhs->op) {
            case AMPER:
                loadexp(rhs);
                goto ok;
            case CTOI:
                gen(LOAD,DREG,NODE,rhs);
#ifdef REGCONTS
                setdreg(rhs);
#else
                rhs->op = DREG;
#endif
            case XREG:
#ifdef USE_YREG
            case YREG:
#endif
            case UREG:
            case DREG:
ok:
                gen(PUSH,rhs->op);
                rhs->op=STACK;
        }
        gen(COMPARE,temp,NODE,rhs);
    } else if (zeroconst(rhs) && op<ULEQ ){
        checkop(lhs);
    } else {
        loadexp(lhs);
        temp = lhs->op;

        if (isdleaf(rhs) || (temp == DREG && isaleaf(rhs))) {
            tranexp(rhs);
        } else {
            gen(PUSH,temp);
            lhs->op=STACK;
            loadexp(rhs);
            temp = rhs->op;
            rhs = lhs;
            op=revrel(op);
        }

        gen(COMPARE,temp,NODE,rhs);
    }
usual:
    gen(CNDJMP,op,setlabel(destin));
}


setlabel(lab)
register labstruc *lab;
{
    if (lab) {
        if (lab->labnum == 0) {
            lab->labnum = getlabel();
#ifdef REGCONTS
            lab->labdreg = treecopy(dregcont);
            lab->labxreg = treecopy(xregcont);
        } else {
            if (!cmptrees(lab->labdreg,dregcont)) {
                reltree(lab->labdreg);
                lab->labdreg = NULL;
            }
            if (!cmptrees(lab->labxreg,xregcont)) {
                reltree(lab->labxreg);
                lab->labxreg = NULL;
            }
#endif
        }
        return lab->labnum;
    }
    return 0;
}


uselabel(lab)
register labstruc *lab;
{
    if (lab->labnum) {
#ifdef REGCONTS
        if (!cmptrees(lab->labdreg,dregcont)) setdreg(NULL);
        if (!cmptrees(lab->labxreg,xregcont)) setxreg(NULL);
        reltree(lab->labdreg);
        reltree(lab->labxreg);
        lab->labdreg = lab->labxreg = NULL;
#endif
        label(lab->labnum);
    }
}


isaleaf(node)
expnode *node;
{
     switch(node->op) {
          case NAME:case CONST:return 1;
          case STAR: return isxleaf(node);
     }
     return 0;
}


isauto(p)
expnode *p;
{
    return p->op == NAME && p->val.sp->storage == AUTO;
}


checkop(node)
register expnode *node;
{
    register expnode *lhs;
    int flag;

    flag=0;
    switch(node->op){
        case AND:
            if((lhs = node->right)->op != CONST
                        || (unsigned)lhs->val.num > 255) flag = 1;
            break;
        case ASSIGN:
            if (isreg(node->left->op)
                            && !isaleaf(node->right)) flag = 1;
            break;
        case INCAFT:
        case DECAFT:
        case DECBEF:
        case INCBEF:
        case ASSPLUS:
        case ASSMIN:
        case ASSAND:
        case ASSOR:
        case ASSXOR:
            if(!isreg(node->left->op)) break;
#ifdef USE_YREG
        case YREG:
#endif
        case UREG:
        case OR:
        case XOR:
        case COMPL:
        case NEG:
        case CALL:
            flag=1;
    }

    shiftflag = 0;
    tranexp(node);

    switch(node->op) {
        case XREG:
        case DREG:
#ifdef USE_YREG
        case YREG:
#endif
        case UREG:
            if (flag || shiftflag) gen(COMPARE,node->op,CONST,0);
            break;
        case CTOI:
            node=node->left;
#ifdef REGCONTS
            if (node->op == DREG) {
                if (shiftflag) gen(COMPARE,node->op,CONST,0);
                break;
            }
#endif  // otherwise fall through, because of what's below
        default:
            gen(LOAD,DREG,NODE,node);
#ifdef REGCONTS
            setdreg(node);
#endif
    }
}


invrel(op)
{
     switch(op){
          case EQ: return NEQ;
          case NEQ:return EQ;
          default: return ((op>GT) ? ULEQ+UGT : LEQ+GT) - op ;
     }
}


revrel(op)
{
      switch(op){
           case EQ:
           case NEQ: return op;
           case LEQ:case LT:case ULEQ:case ULT:
                     return op+2;
           default:  return op-2;
      }
}


zeroconst(node)
register expnode *node;
{
     return node->op == CONST && !node->val.num;
}
