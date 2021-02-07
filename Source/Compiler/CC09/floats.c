/*
    Modification history for floats.c:
        24-May-83       Make better useage of register variables.
        13-Apr-84 LAC   Conversion for UNIX.
*/

#include "cj.h"
#ifdef  DOFLOATS


dload(ptr)
register expnode *ptr;
{
    trandexp(ptr);
    getadd(ptr);
}


trandexp(node)
register expnode *node;
{
    register expnode *p;
    register int op, type;

    type = node->type;
    switch (op = node->op) {
        case FREG:
        case XIND:
        case YIND:
        case UIND:
        case NAME:
            return;
        case STAR:
            tranlexp(node);
            break;
        case UTOD:
        case ITOD:
            lddexp(node->left);
            gen(DBLOP,op);
            node->op = FREG;
            break;
        case LTOD:
            lload(node->left);
            gen(DBLOP,op);
            node->op = FREG;
            break;
        case DTOF:
        case FTOD:
            dload(node->left);
            gen(DBLOP,op);
            node->op = FREG;
            break;
        case FCONST:
            gen(DBLOP,FCONST,node->val.dp);
            node->op = XIND;
            /*  should free constant storage here  */
            node->val.dp = NULL;
            break;
        case QUERY:
            doquery(node,dload);
            node->op = XIND;
            node->val.num = 0;
            break;
        case INCBEF:
        case DECBEF:
        case NEG:
            dload(node->left);
            gen(DBLOP,op,type);
            node->op = XIND;
            node->val.num = 0;
            break;
        case INCAFT:
        case DECAFT:
            gen(LOADIM,XREG,FREG);
            gen(PUSH,XREG);
            dload(node->left);
            gen(DBLOP,op,type);
            gen(DBLOP,MOVE,type);
            gen(DBLOP,op == INCAFT ? DECAFT : INCAFT,type);
            node->op = FREG;
            break;
        case CALL:
            docall(node);
            node->op = FREG;
            break;
        case EQ:
        case NEQ:
        case GEQ:
        case LEQ:
        case GT:
        case LT:
        case PLUS:
        case MINUS:
        case TIMES:
        case DIV:
            dload(node->left);
            gen(DBLOP,STACK);
            dload(node->right);
            gen(DBLOP,op);
            node->op = FREG;
            break;
        case ASSIGN:
            dload(node->left);
            gen(PUSH,XREG);
            dload(node->right);
            gen(DBLOP,MOVE,type);
            node->op = XIND;
            node->val.num = 0;
            break;
        default:
            if (op >= ASSPLUS) {
                p = node->left;
                if (type == FLOAT) {
                    dload(p->left);
                    gen(PUSH,XREG);
                    gen(DBLOP,FTOD);
                } else {
                    dload(p);
                    gen(PUSH,XREG);
                }
                node->op = op - (ASSPLUS - PLUS);
                p->op = XIND;
                trandexp(node);
                if (type == FLOAT) gen(DBLOP,DTOF);
                gen(DBLOP,MOVE,type);
                node->op = XIND;
                node->val.num = 0;
                break;
            }
            comperr(node,"floats");
    }
#ifdef REGCONTS
    clrconts();              /* clear D contents */
#endif
}
#endif
