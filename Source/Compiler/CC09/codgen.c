/*
    Modification history for codgen.c:
        24-May-83       Fixed (once and for all!!) the illegal memory
                        reference involving FREG and/or direct-sc offsets.
                        (Originally goto'ed (sic) dooff: to add the offset
                        causeing illegal reference to node->type.) Made
                        addoff() function to handle this nasty bit.
        25-May-83       Changed around the register variables.
        07-Jun-83       Fixed Non-indirect lea-type references to direct page
                        to look like "leax >a,y" instead of "leax <a".
        29-Aug-83       Function calls in the form of (*((int(*)())0xfffe))();
                        did not defreference properly.
        26-Dec-83       Moved profexit code from gen(RETURN....) to
                        allow returning local storage after call to eprof.
        17-Apr-84 LAC   Conversion for UNIX.
        01-Apr-86 LAC   clear knowledge of D contents for long and float ops.
        17-Jun-86 LAC   fix LOADIM of ptr to static direct variables.
*/

#include "cj.h"

static addoff();
static outlea();

direct int datflag;

/* no advantage putting the following ptrs on dp (ptrs always access indexed)*/
static char *yind = ",y";
static char *spind = ",s";
static char *lbsr = "lbsr ";
static char *lbra = "lbra ";
static char *clra = "clra";
static char *unkopr = "unknown operator: ";

extern char *getrel(),regname();


gen(op,rtype,arg,val)
register int op,rtype;
register expnode *val;
register int arg;
{
    register expnode *ptr;
    register symnode *sptr;
    int reg;
    int temp,value;

    if (op==LONGOP) {
        dolongs(rtype,arg);
        return;
    }
#ifdef  DOFLOATS
    else if(op==DBLOP) {
        dofloats(rtype,arg);
        return;
    }
#endif
    switch(op) {
        case PUSH:
            fprintf(code," pshs %c\n",regname(rtype));
            if((sp -= 2) < maxpush) maxpush=sp;
            return;
        case JMPEQ:
            gen(COMPARE,XREG,CONST,arg);
            arg=rtype;
            rtype=EQ;
        case CNDJMP:
            ot("lb");
            os(getrel(rtype));
            label(arg);
            return;
        case RETURN:
#ifdef USE_YREG
            ol("puls u,y,pc\n");
#else
            ol("puls u,pc\n");
#endif
            return;
        case AND:
        case OR:
        case XOR:
            trouble(op,arg,val);
            return;
        case TIMES:     mwsyscall("ccmult"); return;
        case UDIV:      mwsyscall("ccudiv"); return;
        case DIV:       mwsyscall("ccdiv");  return;
        case SHL:       mwsyscall("ccasl");  return;
        case SHR:       mwsyscall("ccasr");  return;
        case USHR:      mwsyscall("cclsr");  return;
        case UMOD:      mwsyscall("ccumod"); return;
        case MOD:       mwsyscall("ccmod");  return;
        case NEG:       ol("nega\n negb\n sbca #0"); return;
        case COMPL:     ol("coma\n comb"); return;
        case GOTO:      outlea('x'); od(-sp); os(spind); nl();

/* all branches are long; assembly code optimizer fixes them up */
        case JMP:       ot(lbra); label(rtype); return;
        case LABEL:
            ot(lbra);
            label(arg = getlabel());
            label(rtype);
            outlea('s');
            od(sp);
            os(",x\n");
            label(arg);
            return;
        case CALL:
            callflag = 4;   /* basic amount of stack needed to push U,PC */

#ifdef DEBUG
#if 0
        fflush(stdout);
        fprintf(stderr,"arg=%04x op=%d val=%04x\n",
                arg,((int*)arg)->op,((int*)arg)->value);
#endif
#endif
            if ((ptr = (expnode *) arg)->op == NAME
                                    && (sptr = ptr->val.sp)) {
                ot(lbsr);
                nlabel(sptr->sname,0);
                return;
            }
            ot("jsr ");
            deref(rtype,arg,0);
            nl();
            return;
        case CTOI:      ol("sex");return;
        case UTOI:      ol(clra);return;
        case LTOI:      ot("ld"); doref('d',rtype,arg,2); return;
        case IDOUBLE:   ol("aslb\n rola"); return;
        case HALVE:     ol("asra\n rorb"); return;
        case UHALVE:    ol("lsra\n rorb"); return;
        case CHALVE:    ol("asrb"); return;
        case UCHALVE:   ol("lsrb"); return;

#ifdef USE_YREG
        case YREG:
            ot("ldy ");
            goto dooff;
#endif
        case UREG:
            ot("ldu ");
dooff:
            od(rtype);
            os(spind);nl();
            return;
        case LEAX:
            outlea('x');
            switch (rtype) {
                case NODE:
                    reg = (reg = ((expnode *)arg)->op) == YIND
                                            ? 'y' : reg == UIND
                                            ? 'u' : 'x';
                    fprintf(code,"%d,%c\n",((expnode *)arg)->val.num,reg);
                    break;
                case DREG:
                    fprintf(code,"d,%c\n",regname(arg));
                    break;
            }
            return;
    }

    reg=regname(rtype);
    if (arg == NODE) {
        if (val->op == UTOI) { /* uchar, clear A and then get value */
            ol (clra);
            gen (op, DREG, NODE, val->left);
            val->op = DREG;
            return;
        } else if (val->op == CTOI) { /* signed char, get int value */
            gen (op, DREG, NODE, val->left);
            switch (op) {
                case LOAD: ol("sex");break;
                case RSUB:
                case PLUS: ol("adca #0");break;
                case MINUS:ol("sbca #0");break;
            }
            val->op = DREG;
            return;
        }

#ifdef DEBUG
#if 0
        fprintf(stderr,
             "val=%04x val->type=%04x op=%04x reg=%04x val->op=%04x rhs=%04x\n",
             val,val->type,op,reg,val->op,((int*)val->value)->type);
#endif
#endif
        if ((val->type==CHAR || val->type==UCHAR) && op!=LOADIM && reg != 'x')
            reg='b';
    }

    switch(op) {
        case LOAD:
            if (arg==NODE) {
                value = val->val.num;
                switch (temp=val->op) {
                    case XREG:
#ifdef USE_YREG
                    case YREG:
#endif
                    case UREG:
                        if (rtype!=DREG) {
                            outlea(reg);
                            fprintf(code,"%d,%c\n",value,regname(temp));
                        } else {
                            transfer(regname(temp),'d');
                            if (value) gen(PLUS,DREG,CONST,value);
                        }
                        return;
                    case DREG:
                        if(rtype!=DREG) transfer('d',reg);
                        return;
                    case STRING:
                        outstr(reg,val->val.num);
                        return;
                    case CONST:
                        arg = CONST;
                        val = (expnode *)value;
                }
            }
            if(rtype==DREG && arg==CONST && val==0) {
                ol("clra\n clrb");
                return;
            }
            ot("ld");
            goto simple;
        case COMPARE:
            if(arg == CONST) {
                if(val == 0) {
                    fprintf(code," st%c -2,s\n",reg);
                    return;
                }
            }
            ot("cmp");
            if (reg=='b') reg='d';
            goto simple;
        case STORE:
            if (arg==NODE && isreg(temp=val->op)) {
                if(temp!=rtype) transfer(reg,regname(temp));
                return;
            }
            ot("st");
            goto simple;
        case MINUS:
            ot("sub");
            goto simple;
        case RSUB:
            gen(NEG);
        case PLUS:
            ot("add");
simple:
            doref(reg,arg,val,0);
            return;
        case LOADIM:
            if (arg == NODE) {
                switch (temp = val->val.sp->storage) {
                    case DIRECT:
                    case EXTERND:
                    case STATICD:
                        outlea(reg);
                        ob('>');
                        if (temp == STATICD) olbl(val->val.sp->offset);
                        else on(val->val.sp->sname);
                        addoff(val->modifier);
#ifndef USE_YREG
                        os(yind);
#endif
                        nl();
                        return;
                }
            }
            ot("lea");
            doref(reg,arg,val,0);
            return;
        case EXG:
            fprintf(code," exg %c,%c\n",reg,regname(arg));
            return;
        case LEA:
            outlea(reg);
            switch (arg) {
                case DREG:
                    os("d,");
                    goto leaexit;
                case CONST:
                    od(val);
                    ob(',');
leaexit:
                    ob(reg);
                    nl();
                    return;
                default:
                    error("LEA arg");
                    break;
            }
            return;
        default:
            error(unkopr);
    }
}


char
regname(r)
{
    switch (r) {
        case DREG:  return 'd';
        case XREG:  return 'x';
#ifdef USE_YREG
        case YREG:  return 'y';
#endif
        case UREG:  return 'u';
        default:    return ' ';
    }
}


transfer(r1,r2)
{
    fprintf(code," tfr %c,%c\n",r1,r2);
}


dolongs(op,arg)
int *arg;
{
    switch (op) {
        case STACK:
            gen(LOAD,DREG,XIND,2);
            gen(PUSH,DREG);
            gen(LOAD,DREG,XIND,0);
            gen(PUSH,DREG);
            break;
        case TEST:
            ol("lda 0,x\n ora 1,x\n ora 2,x\n ora 3,x");
            break;
        case MOVE:      lcall("_lmove"); sp-=2; break;
        case PLUS:      lcall("_ladd"); break;
        case MINUS:     lcall("_lsub"); break;
        case TIMES:     lcall("_lmul"); break;
        case DIV:       lcall("_ldiv"); break;
        case MOD:       lcall("_lmod"); break;
        case AND:       lcall("_land"); break;
        case OR :       lcall("_lor"); break;
        case XOR:       lcall("_lxor"); break;
        case UDIV:      lcall("_ludiv"); break;
        case UMOD:      lcall("_lumod"); break;
        case SHL:       lcall("_lshl"); sp -= 2; break;
        case SHR:       lcall("_lshr"); sp -= 2; break;
        case USHR:      lcall("_lushr"); sp -= 2; break;
        case EQ:
        case NEQ:
        case GEQ:
        case LEQ:
        case GT:
        case LT:
        case UGEQ:
        case ULEQ:
        case UGT:
        case ULT:       lcall("_lcmpr"); break;
        case NEG:       lcall("_lneg"); sp -= LONGSIZE; break;
        case COMPL:     lcall("_lcompl"); sp -= LONGSIZE; break;
        case ITOL:      lcall("_litol"); sp -= LONGSIZE; break;
        case UTOL:      lcall("_lutol"); sp -= LONGSIZE; break;
        case INCBEF:
        case INCAFT:    lcall("_linc"); sp -= LONGSIZE; break;
        case DECBEF:
        case DECAFT:    lcall("_ldec"); sp -= LONGSIZE; break;
        case LCONST:    getcon(arg,LONGSIZE/2,FALSE); break;

        default:
            error("codgen - longs");
            os(unkopr);
            od(op);
            nl();
            break;
    }
#ifdef REGCONTS
    setdreg(NULL);
#endif
}

/*
    In the decompiled source (which doesn't use register contents), all of
    these breaks are returns, because setdreg() isn't used.
*/
#ifdef  DOFLOATS
dofloats(op,arg)
register int arg;
{
    switch (op) {
        case FCONST:    getcon(arg,DOUBLESIZE/2,TRUE); break;
        case STACK:     fcall("_dstack"); sp -= DOUBLESIZE; break;
        case TEST:
            fprintf(code," lda %c,x\n",arg == FLOAT ? '3' : '7');
            break;
        case MOVE:
            fcall(arg == FLOAT ? "_fmove" : "_dmove");
            sp += 2;
            break;
        case PLUS:      fcall("_dadd"); sp += DOUBLESIZE; break;
        case MINUS:     fcall("_dsub"); sp += DOUBLESIZE; break;
        case TIMES:     fcall("_dmul"); sp += DOUBLESIZE; break;
        case DIV:       fcall("_ddiv"); sp += DOUBLESIZE; break;
        case EQ:
        case NEQ:
        case GEQ:
        case LEQ:
        case GT:
        case LT:        fcall("_dcmpr"); sp += DOUBLESIZE; break;
        case NEG:       fcall("_dneg"); break;
        case INCBEF:
        case INCAFT:    fcall(arg == FLOAT ? "_finc" : "_dinc"); break;
        case DECBEF:
        case DECAFT:    fcall(arg == FLOAT ? "_fdec" : "_ddec"); break;
        case DTOF:      fcall("_dtof"); break;
        case FTOD:      fcall("_ftod"); break;
        case LTOD:      fcall("_ltod"); break;
        case ITOD:      fcall("_itod"); break;
        case UTOD:      fcall("_utod"); break;
        case DTOL:      fcall("_dtol"); break;
        case DTOI:      fcall("_dtoi"); break;
        default:
            error("codgen - floats");
            os(unkopr);
            od(op);
            nl();
    }
#ifdef REGCONTS
    setdreg(NULL);
#endif
}
#endif


getcon(p,n,f)
int *p;
{
    int temp;

    ot("bsr ");
    label(temp = getlabel());
    defcon(p,n,f);
    olbl(temp);
    ol("puls x");
}

#ifdef IEEE_FLOATS
double dadjust();
#endif

defcon(p,n,f)
register int16_t *p;
int n, f;
{
    register int i;

    defword();
    if (n == 1) od(p);
    else if (p == NULL) {
        for (i = 1; i++ < n; ) os("0,");
        ob('0');
#ifdef IEEE_FLOATS
    } else if (f == 1) { /* f = float format */
        int16_t *temp = alloca(n * 2); /* n is in words, so we double it */

# ifdef DEBUG
        fprintf(stderr, "converting float: %g (%016llx)\n", *(double *)p, *(int64_t *)p);
# endif

        switch (n) {
            case 4: /* double */
                *(double *)temp = dadjust(*(double *)p);
                break;
            case 2: /* float */
            default:
                error("codgen - float conv");
        }

# ifdef DEBUG
        fprintf(stderr, "after conversion: %016llx\n", *(int64_t *)temp);
# endif

# ifdef _LIL_END
        /*
         * We're only swapping the order in which we output the words, the
         * individual words will format correctly since they're native.
         */
        temp += (n - 1);
        for (i = n; i > 0; --i){
            od(*temp--);
            if (i > 1) ob(',');
        }
# else	/* assume bigendian */
        for (i = 0; i < n; ++i){
            od(*temp++);
            if (i != n - 1) ob(',');
        }
# endif /* ! _LIL_END */
#endif /* IEEE_FLOATS */
	} else {
# ifdef _LIL_END
        /*
         * We're only swapping the order in which we output the words, the
         * individual words will format correctly since they're native.
         */
        p += (n - 1);
        for (i = n; i > 0; --i){
            od(*p--);
            if (i > 1) ob(',');
        }
#else	/* assume bigendian */
        for (i = 0; i < n; ++i){
            od(*p++);
            if (i != n - 1) ob(',');
        }
#endif	/* ! _LIL_END */
    }
    nl();
}


mwsyscall(s)
{
    ot(lbsr);
    ol(s);
    sp += 2;
}


lcall(s)
{
    ot(lbsr);
    ol(s);
    sp += 4;
}


fcall(s)
{
    ot(lbsr);
    ol(s);
}


trouble(op,arg,val)
register int op,arg;
register expnode *val;
{
    char *s;
    int cflag,temp;

    cflag = 0;
    if (arg == NODE) {
        cflag = (val->type == CHAR || val->type == UCHAR);
        arg = val->op;
        val = (expnode *) val->val.sp;
    }

    switch (op) {
        case AND:   s = "and"; break;
        case OR:    s = "or"; break;
        case XOR:   s = "eor"; break;
    }
    switch (arg) {
        case NAME:
        case YIND:
        case UIND:
        case XIND:
            if (cflag) {
                if(op == AND) ol(clra);
                ot(s);
                doref('b',arg,val,0);
            } else {
                ot(s);
                doref('a',arg,val,0);
                ot(s);
                doref('b',arg,val,1);
            }
            break;
        case CONST:
            switch (temp = ((int)val >> 8) & 0xff) {
                case 0:
                    if (op == AND) ol(clra);
                    break;
                case 0xff:
                    if (op == AND) break;
                    if (op == XOR) {
                        ol("coma");
                        break;
                    }
                default:
                    ot(s);
                    doref('a',CONST,temp,0);
            }
            switch (temp = (int)val & 255) {
                case 0:
                    if (op == AND) ol("clrb");
                    break;
                case 0xff:
                    if (op == AND) break;
                    if(op == XOR) {
                        ol("comb");
                        break;
                    }
                default:
                    ot(s);
                    doref('b',CONST,temp,0);
            }
            break;
        case STACK:
            fprintf(code," %sa ,s+\n %sb ,s+\n",s,s);
            sp += 2;	/* s++, explicitly 2 bytes */
            break;
        default:
            error("compiler trouble");
            break;
    }
}


doref(reg,arg,val,offset)
{
    ob(reg);
    ob(' ');
    deref(arg,val,offset);
    nl();
}


static
addoff(offset)
register int offset;
{
    if(offset) {
        if(offset > 0) ob('+');
        od(offset);
    }
}


deref(arg,val,offset)
{
    register expnode *node;
    register symnode *sn;
    int sc;

    if (arg & INDIRECT) ob('[');

    switch (arg & NOTIND) {
        case NODE:
            node = (expnode *)val;
            if (node->op == AMPER) {
                ob('#');
                deref(NODE,node->left,offset);
                return;
            }
            deref(node->op,node->val.sp,offset+node->modifier);
            return;
        case CONST:
            ob('#');
            od(val);
            break;
        case FREG:
            os("_flacc");
            addoff(offset);
#ifndef USE_YREG
            os(yind);
#endif
            break;
        case NAME:
            if (sn = (symnode *)val)
                switch (sc = sn->storage) {
                    case AUTO:
                        od(val = sn->offset-sp+offset);
                        os(spind);
                        break;
                    case STATICD:
                        if(!datflag) ob((arg & INDIRECT) ? '>' : '<');
                    case STATIC:
                        olbl(sn->offset);
                        goto dooff;
                    case DIRECT:
                    case EXTERND:
                        if(!datflag) ob((arg & INDIRECT) ? '>' : '<');
                    case EXTERN:
                    case EXTDEF:
                        on(sn->sname);
dooff:
                        addoff(offset);
                        if(arg & INDIRECT || (!datflag && (sc != DIRECT
                                    && sc != EXTERND && sc != STATICD))) {
                            if(isftn(sn->type)) os(",pcr");
#ifndef USE_YREG
                            else os(yind);
#endif
                        }
                        break;
                    default:
                        error("storage error");
                        break;
                }
            else od(offset);
            break;
        case XIND:
        case YIND:
        case UIND:
            od(val += offset);
            ob(',');
            switch(arg & NOTIND) {
                case XIND:  ob('x'); break;
                case YIND:  ob('y'); break;
                case UIND:  ob('u'); break;
            }
            break;
        case STACK:
            os(spind); os("++"); sp += 2;	/* ++, explicitly 2 bytes */
            break;
        default:
            error("dereference");
    }

    if(arg & INDIRECT) ob(']');
}


char *
getrel(op)
{
    switch (op) {
        default:    error("rel op");
        case EQ:    return "eq ";
        case NEQ:   return "ne ";
        case LEQ:   return "le ";
        case LT:    return "lt ";
        case GEQ:   return "ge ";
        case GT:    return "gt ";
        case ULEQ:  return "ls ";
        case ULT:   return "lo ";
        case UGEQ:  return "hs ";
        case UGT:   return "hi ";
    }
}


ot(s)
{
    putc(' ',code);
    os(s);
}


ol(s)
{
    ot(s);
    nl();
}


nl()
{
    putc('\n',code);
}


ob(b)
{
    putc(b,code);
}


os(s)
char *s;
{
    fputs(s,code);
}


od(n)
{
#ifdef _OS9
    fprintf(code,"%d",n & 0xFFFF);
#else
    fprintf(code,"%hd",n & 0xFFFF);
#endif
}


label(n)
{
    olbl(n);
    lastst = 0;
    nl();
}


olbl(n)
{
    ob(UNIQUE);
    od(n);
}


on(s)
char *s;
{
    fprintf(code,"%.8s",s);
}


modstk(nsp)
{
    int x;

    if (x=nsp-sp) {
        outlea('s');
        od(x);
        os(spind);
        nl();
    }
    return nsp;
}


nlabel(ptr,scope)
{
    on(ptr);
    if (scope) ob(':');
    nl();
}


static
outlea(reg)
{
    fprintf(code," lea%c ",reg);
}


outstr(reg,l)
{
    outlea(reg);
    fprintf(code,"%c%d,pcr\n",UNIQUE,l);
}
