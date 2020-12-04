#include <stdint.h>
#ifdef unix
# define getline UNIX_getline
# include <errno.h>
# include <string.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#undef getline

#ifdef MAIN
#define global
#else
#define global extern
#endif

/*	Floating-point format
 *
 *	The native compiler uses a variant of PDP-11 D_floating format for
 *	'double'. The cross-compiler tries to convert the system's native double
 *	format into as close a form as possible. 'float' isn't really a problem
 *	until we have prototypes.
 */
#ifdef __STDC_IEC_559__
# define IEEE_FLOATS
#endif /* __STDC_IEC_559__ */
/*  Machine/Operating System dependent definitions.
 *
 *  see also 'local.c'.
 */

#ifdef OS9
#define INTTYPE     int
#define LONGTYPE    long
#define FLOATYPE    float
#define DBLETYPE    double
#endif

#ifdef OSK
#define INTTYPE     short
#define LONGTYPE    long
#define FLOATYPE    float
#define DBLETYPE    double
#endif

#ifdef unix
#define direct
#define register
#define INTTYPE     short
#define LONGTYPE    long
#define FLOATYPE    float
#define DBLETYPE    double
#endif

#define MICRO               /* suppresses word boundary forcing */
#define DOFLOATS            /*  include floating point code */
#define FNAMESIZE   30      /* file name size */

#define LINESIZE    256
#define MODNAMSIZE  15      /* module name */

#define NAMESIZE    8       /* identifiers for assembler */
#define UNIQUE      '_'     /* for local labels etc. */
#define NEWLINE     13

typedef struct structstruct {
    struct structstruct *strnext;   /* next element in structure */
    struct symstr *element;         /* ptr to mos entry */
} elem;

typedef struct dimstr {
    short dim;              /* size of this dimension */
    struct dimstr *dptr;    /* ptr to next dimension */
} dimnode;

typedef struct downsym {
    short type;             /* type of entry */
    int size;               /* size of entry */
    dimnode *dimptr;        /* ptr to list of dimensions */
    int offset;             /* general purpose */
    int storage;            /* storage class */
    union {
        elem *elems;        /* chain of structure members */
        int labflg;         /* label flags */
    } x;
    short blklev;           /* block level of declaration */
    struct symstr *downptr, /* ptr to 'hidden' outer declaration */
                  *snext;   /* ptr to entry in various lists */
} pshnode;

#define DOWNSIZE sizeof(pshnode)

typedef struct symstr {
    short type;             /* type of entry */
    int size;               /* size of entry */
    dimnode *dimptr;        /* ptr to list of dimensions */
    int offset;             /* general purpose */
    int storage;            /* storage class */
    union {
        elem *elems;        /* chain of structure members */
        int labflg;         /* label flag */
    } x;
    short blklev;           /* block level of declaration */
    struct symstr *downptr, /* ptr to 'hidden' outer declaration */
                  *snext,   /* ptr to entry in various lists */
                  *hlink;   /* ptr to next symbol in hash chain */
    char sname[NAMESIZE+1]; /* name entry */
} symnode;

#define SYMSIZE sizeof(symnode)

typedef struct expstr {
    short type;             /* type of node */
    int size;               /* size of basic type */
    dimnode *dimptr;        /* ptr to list of dimensions */
    short op;               /* operator */
    union {
        int num;            /* numeric value */
        symnode *sp;        /* symbol pointer */
        long *lp;           /* long constant ptr */
#ifdef DOFLOATS
        FLOATYPE *fp;       /* float constant ptr */
        DBLETYPE *dp;       /* double constant ptr */
#endif
    } val;                  /* value or symbol ptr */
    struct expstr *left,    /* left node ptr */
                  *right;   /* right node ptr */
    short lno;              /* line # of token causing this node */
    char *pnt;              /* ptr to line position of token */
    short sux;              /* pseudo Sethi-Ullman number */
    short modifier;         /* added to NAME references */
} expnode;

#define NODESIZE sizeof(expnode)

typedef struct initstruct {
    struct initstruct *initnext;    /* next initializer */
    expnode *initp;                 /* ptr to initializer expression */
    symnode *initname;              /* ptr to dude being initialized */
} initnode;

/* object sizes - char is by definition 1 */
#define INTSIZE     2
#define LONGSIZE    4

#ifdef  DOFLOATS
#define FLOATSIZE   4
#define DOUBLESIZE  8
#endif

#define POINTSIZE   2

/*  symbols  */
#define POINTER     0x10
#define ARRAY       0x20
#define FUNCTION    0x30

#define XTYPE       0x30
#define BASICT      0x0F

#define INT         1
#define CHAR        2
#define UNION       3
#define STRUCT      4
#ifdef  DOFLOATS
#define FLOAT       5
#define DOUBLE      6
#endif
#define UNSIGN      7
#define LONG        8
#define LABEL       9
#define USTRUCT     10

#define STRTAG      8
#define SHORT       10
#define ARG         11
#define EXTDEF      12
#define AUTO        13
#define EXTERN      14
#define STATIC      15
#define REG         16
#define RETURN      18
#define MOS         17
#define IF          19
#define WHILE       20
#define ELSE        21
#define SWITCH      22
#define CASE        23
#define BREAK       24
#define CONTIN      25
#define DO          26
#define DEFAULT     27
#define FOR         28
#define GOTO        29
#define TYPEDEF     30
#define DEFTYPE     31
#define CAST        32
#define DIRECT      33   /* direct */
#define EXTERND     34   /* extern direct */
#define STATICD     35   /* static direct */

#define SEMICOL     40
#define LBRACE      41
#define RBRACE      42
#define LBRACK      43
#define RBRACK      44
#define LPAREN      45
#define RPAREN      46
#define COLON       47
#define COMMA       48
#define KEYWORD     51
#define NAME        52
#define CONST       54
#define STRING      55
#define SIZEOF      59
#define INCBEF      60
#define DECBEF      61
#define INCAFT      62
#define DECAFT      63
#define NOT         64
#define AMPER       65
#define STAR        66
#define NEG         67
#define COMPL       68
#define DOT         69
#define ARROW       70
#define DBLAND      71
#define DBLOR       72
#define LCONST      74
#ifdef DOFLOATS
#define FCONST      75
#endif
#define UMOD        76
#define USHR        77
#define UDIV        78
#define RSUB        79
#define PLUS        80
#define MINUS       81
#define TIMES       82
#define DIV         83
#define MOD         84
#define SHR         85
#define SHL         86
#define AND         87
#define OR          88
#define XOR         89

#define EQ          90
#define NEQ         91
#define LEQ         92
#define LT          93
#define GEQ         94
#define GT          95
#define ULEQ        96
#define ULT         97
#define UGEQ        98
#define UGT         99

#define ASSIGN      120

#define QUERY       100
#define CALL        101
#define BSLASH      102
#define SHARP       103
#define PRIME       104
#define QUOTE       105
#define LETTER      106
#define DIGIT       107
#define NEWLN       108
#define SPACE       109

/* code generator symbols */
#define STACK       110
#define UREG        111
#define DREG        112
#define XREG        113
#define EXG         115
#define LEA         116
#define LOAD        117
#define YREG        118
#define NODE        119
#define STORE       121
#define PUSH        122
#define LEAX        123
#define JMP         124
#define JMPEQ       125
#define JMPNE       126
#define LOADIM      127
#define FREG        128

#define COMPARE     129
#define CNDJMP      130
#define ITOL        131
#define LTOI        132
#define CTOI        133
#define UTOL        134
#define DBLOP       135
#define LONGOP      136
#define MOVE        137
#define STOI        138
#define TEST        139
#ifdef DOFLOATS
#define FTOD        140
#define DTOF        141
#define ITOD        142
#define DTOI        143
#define LTOD        144
#define DTOL        145
#define UTOD        146
#endif
#define XIND        147
#define YIND        148
#define UIND        149

#define HALVE       150
#define UHALVE      151
#define IDOUBLE     152

#define ASSPLUS     160
#define ASSMIN      161
#define ASSMUL      162
#define ASSDIV      163
#define ASSMOD      164
#define ASSSHR      165
#define ASSSHL      166
#define ASSAND      167
#define ASSOR       168
#define ASSXOR      169

/* indirection masks */
#define INDIRECT    0x8000
#define NOTIND      0x7FFF

/* label tests */
#define DEFINED     1
#define GONETO      2

/* dp indicators */
#define DDP         1  /* on dp */
#define NDP         0  /* not on dp */

#define UNKN        0
#define UNDECL      0

#ifdef TRUE
#undef TRUE
#undef FALSE
#endif

#define TRUE        1
#define FALSE       0
#define ERR         0xFFFF

#define TAB_CHAR    9
#define LINEFEED    10
#define FORMFEED    12
#define RET_CHAR    13
#define VTAB_CHAR   11
#define BACKSPACE   8

#define btype(t)    ((t) & BASICT)
#define isptr(t)    (((t) & XTYPE) == POINTER)
#define isary(t)    (((t) & XTYPE) == ARRAY)
#define isftn(t)    (((t) & XTYPE) == FUNCTION)
#define modtype(t,b) (((t) & (~BASICT)) + b)

#define getlabel(a) (++curlabel)

#define islong(t)   (t->type == LONG)
#ifdef DOFLOATS
#define isfloat(t)  (t->type == FLOAT || t->type == DOUBLE)
#endif

global direct FILE  *in,    /* input file pointer */
                    *code;  /* code file pointer */

global direct int   lineno,     /* current line number */
                    errcount,
                    curlabel,   /* next internal label no. */
                    sp,         /* current stack pointer offset */
                    stlev,      /* stack reservation level */
                    callflag,   /* function call flag */
                    sflag,      /* suppress stack checking code */
#ifdef PROF
                    pflag,      /* if set generate profile calls */
#endif
                    nocode,     /* if set no code output */
                    ftype,      /* type of current function */
                    lastst,     /* last statement type */
                    blklev,     /* current block level */
                    breaklab,   /* break label */
                    cntlab,     /* continue label */
                    swflag,     /* current switch level */
                    mosflg,     /* member of structure flag */
                    sym,        /* current lexical token */
                    symval,     /* and its associated value */
                    scount,     /* chars on current line of string */
                    symline,    /* lineno of current symbol */
                    maxpush;    /* maximum push level */

global char filename[FNAMESIZE];    /* current filename buffer */

global direct symnode *freedown,  /* list of free push-down entries */
                    *arglist,   /* list of arguments */
                    *labelist,  /* list of labels    */
                    *vlist;     /* list of declared variables */

global direct char  *symptr,    /* start of current symbol */
                    cc,         /* current character */
                    *lobrk,     /* lower store bound */
                    *hibrk,     /* upper store bound */
                    *lptr;      /* input line pointer */

global direct FILE  *strfile;   /* strings output file */

extern char     chartab[128],   /* character translater table */
                valtab[128];    /* operator precedence table */

global symnode  *hashtab[128],  /* main hash table */
                *mostab[128];   /* structure hash table */

#ifdef PTREE
global char *kw[200];           /* pointers to symbol names */
#ifndef CKEYSFILE
# if defined(OS9) || defined(OSK) || defined(_OSK) ||defined(_OS9000)
#   define CKEYSFILE "/dd/lib/ckeys"
#  else
#   define CKEYSFILE "/usr/local/share/cc09/ckeys"
#  endif
# endif
#endif

global direct int   dflag,  /* debug flag; if set causes tree prints */
                    lflag,  /* if set, source lines copied to code file */
                    tflag,  /* got an input file indicator */
                    tcount;

global symnode  sdummy;         /* dummy expression node */

global expnode  *freenode;      /* free expression node list */

global char line[LINESIZE],     /* input line buffer */
            lastline[LINESIZE]; /* saved last line */

extern  char    strname[];      /* string file name */

/*  label state structure  */
typedef struct {
    int     labnum,
            labsp;
#ifdef REGCONTS
    expnode *labdreg,
            *labxreg;
#endif
} labstruc;


/*  function type definitions  */

extern  expnode *parsexp(), *primary(), *explist(), *getcast(),
                *newnode(), *optim(), *fold(), *chtype(), *fixup(),
                *tranexp(), *tranbool(), *treecopy();

extern  dimnode *dimwalk();
