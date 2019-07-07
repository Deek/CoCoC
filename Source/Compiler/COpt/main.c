#include "op.h"
#include "../ident.c"

#define LINESIZE 100

FILE *in,*out,*datfile;
direct int dbflag,inserts,inlcnt;

#ifdef DEBUG
direct int debugon,debugcnt;
#endif

extern int insmax;

main(argc,argv)
char **argv;
{
    int c;
    char *argp,*infile,*outfile;

    in = stdin;
    out = stdout;
    infile = outfile = NULL;

#ifdef DEBUG
    debugcnt = 29999;
#endif

    while (++argv, --argc) {
        if (*(argp = *argv) == '-')
            switch(*++argp) {
                case '\0':
                    dbflag = 1;
                    break;
#ifdef DEBUG
                case 'd':
                    debugon = 1;
                    debugcnt = 0;
                    if (*++argp != '=') --argp;
                    while (c = *++argp)
                        debugcnt = debugcnt * 10 + (c - '0');
                    break;
#endif
                case 'i':
                    insmax = 0;
                    if (*++argp != '=') --argp;
                    while (c = *++argp)
                        insmax = insmax * 10 + (c - '0');
                    break;
                default:
                    error("unknown option '%s'\n",--argp);
            }
        else if (!infile) infile = argp;
        else if (!outfile) outfile = argp;
        else error("too many files");
    }

    if (infile)
        if((in = freopen(infile,"r",stdin)) == NULL)
            error("can't open %s",infile);

    if (outfile)
        if((out = fopen(outfile,"w")) == NULL)
            error("can't open %s",outfile);

    datfile = out;

#ifdef DEBUG
    debug("files open,");
#endif
    labelinit();
#ifdef DEBUG
    debug(" labels initted,");
#endif
    insinit();
#ifdef DEBUG
    debug(" instructs initted,");
#endif
    optinit();
#ifdef DEBUG
    debug("opts initted.\n");
#endif

    optimise();
#ifdef DEBUG
    debug("finished optimise\n");
#endif

    if(dbflag) {
        int totbytes,bytes;

        fprintf(stderr,"statistics:\n");
        fprintf(stderr,"\ttotal instructions : %d\n",inserts);
        fprintf(stderr,"\tlong branches :  %5d, %5d, %3d%%\n",
                lbf,lbdone,percent(lbdone,lbf)) ;
        fprintf(stderr,"\tremoved       :         %5d, %3d%%\n",
                opsdone,percent(opsdone,inserts));
        bytes = 2 * lbdone;
        bytes += estimate(opsdone);
        totbytes = estimate(inserts);
        fprintf(stderr,"\ttotal bytes   :  %5d, %5d, %3d%%\n",
                totbytes,bytes,percent(bytes,totbytes));
    }
}

int estimate(n)
{
    return n + n + (n * 3 / 5);
}

int percent(x,y)
unsigned x,y;
{
    return y ? x / ((y + 50) / 100 ) : 0;
}


static direct int datflag,endflag;
#define DIRECT  2

optimise()
{
    char line[LINESIZE];
    char lab[LABELSIZE+1],mnem[MNEMSIZE+1],args[ARGSIZE+1];
    label *l,*lp1,*lp2;
    char *p;
    instruction *i;
    int dpflag,globflag;

    l = NULL;
    datflag = endflag = FALSE;
    inlcnt = 0;

    while (fgets(line,LINESIZE,in) != NULL) {
        {
            register char *lp = line;
            while (*lp != '\n') ++lp;
            *lp = '\0';
        }
        ++inlcnt;
#ifdef DEBUG
        if (inlcnt >= debugcnt) dbflag = debugon;
        debug("\nline %5d: \"%s\"\n",inlcnt,line);
#endif
        p = line;

        if(*p == '\0' || *p == '*')
            continue;

        p = parse(LABEL,lab,p,&globflag);
        p = parse(MNEM,mnem,p);
        while (*p == ' ' || *p == '\t') ++p;

        /* scrutinise the first character for a match
         * before bothering 'strcmp()', he's very busy
         */
        switch(*mnem) {
            case 'e':
                if (strcmp(mnem,"endsect") == 0) {
                    endsect();
                    endflag = TRUE;
                    continue;
                }
                break;
            case 'i':
                if (strcmp(mnem,"info") == 0)
                    goto put;
                break;
            case 'n':
                if(strcmp(mnem,"nam") == 0) {
put:                fprintf(out,"%s\n",line);
                    continue;
                }
                break;
            case 'p':
                if(strcmp(mnem,"psect") == 0) {
                    endsect();
                    goto put;
                }
                break;
            case 't':
                if (strcmp(mnem,"ttl") == 0)
                    goto put;
                break;
            case 'v':
                if (strcmp(mnem,"vsect") == 0) {
                    dpflag = (p[0] == 'd' && p[1] == 'p'
                                ? DIRECT : TRUE);
                    if (datflag == dpflag && endflag) {
                        endflag = FALSE;
                        continue;
                    }
                    endsect();
                    datflag = dpflag;
                    goto put;
                }
                break;
        }

        endsect();
        if (datflag) fprintf(datfile,"%s\n",line);
        else {
            if (*lab) {
                if ((lp1 = findlabel(lab)) == NULL)
                    lp1 = inslabel(lab);
                if (globflag) lp1->lflags |= GLOBAL;
                for (lp2 = lp1; lp2->nextl; lp2 = lp2->nextl);
                lp2->nextl = l;
                l = lp1;
            }
            if (*mnem) {
                parse(ARGS,args,p);
                if (insins(&ilist,mnem,args,&l)) {
                    optim(ilist.pred);
                    finddupl(ilist.pred);
                }
                ++inserts;
                if (inscount >= insmax) remove();
            }
        }
    }

    if (l) insins(&ilist,"nop","",&l);
    endsect();
    while (inscount) remove();
}

endsect()
{
    if (endflag) {
        if (datflag) datflag = FALSE;
        else while (inscount) remove();
        fprintf(out," endsect\n");
        endflag = FALSE;
    }
}
