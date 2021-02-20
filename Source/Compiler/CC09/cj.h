#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*	Canonicalize our system definitions: __unix__, _OS9, _OSK, _OS9000
 */
#if defined(unix) || defined(__linux__) || defined(__MACH__)
# ifndef __unix__
#  define __unix__	1
# endif
#endif

#ifdef OS9
# ifndef _OS9
#  define _OS9	1
# endif
#endif

#ifdef OSK
# ifndef _OSK
#  define _OSK	1
# endif
#endif

/*	In some cases, we just want to know if we're running on a Microware-style
 *	operating system, because that means we've got paths like /dd/foo.
 */
#if defined(_OS9) || defined(_OSK) || defined(_OS9000)
# define MWOS	1
#endif

#ifdef MAIN
# define global
#else
# define global	extern
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

/*	Endianness definitions.
 *
 *	We want the UNIX-style BYTE_ORDER/foo_ENDIAN definitions as well as the
 *	simple flag-based ones OSK/OS-9000 use today.
 */
#ifdef __unix__
# include <endian.h>
# if BYTE_ORDER == BIG_ENDIAN
#  define _BIG_END 1
# elif BYTE_ORDER == LITTLE_ENDIAN
#  define _LIL_END 1
# elif BYTE_ORDER == PDP_ENDIAN
#  define _PDP_END 1
# else
#  error "unknown endianness, can't continue"
# endif
#else /* not UNIX */
# define LITTLE_ENDIAN	1234
# define BIG_ENDIAN	4321
# define PDP_ENDIAN	3412
# if defined(MWOS)
#  if defined(_OS9) || defined(_OSK) || defined(_BIG_END) /* predefined bigendian */
#   define BYTE_ORDER	BIG_ENDIAN
#   ifndef _BIG_END
#    define _BIG_END 1
#   endif
#  elif defined(_LIL_END)
#   define BYTE_ORDER	LITTLE_ENDIAN
#  endif /* done with MWOS */
# elif defined(_WIN32)
#  define BYTE_ORDER	LITTLE_ENDIAN
#  define _LIL_END 1 /* done with Win32 */
# else
#  warning "unknown system, arbitrarily assuming big-endian"
#  define BYTE_ORDER	BIG_ENDIAN
#  define _BIG_END 1
# endif
#endif /* not UNIX */

/*  Machine/Operating System dependent definitions.
 *
 *  see also 'local.c'.
 */

#ifdef _OS9
# define INTTYPE	int
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	ibrk
#endif

#ifdef _OSK
# define INTTYPE	short
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	ebrk
#endif

#ifdef __unix__
# include <errno.h>
# include <string.h>
# include <fcntl.h>

# define direct
# define register
# define INTTYPE	short
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	sbrk
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

/* primitive (basic) types */
#define UNDECL      0   /* type hasn't been declared */
#define INT         1   /* most convenient integer type */
#define CHAR        2   /* smallest unit of memory */
#define LONG        3   /* long int (and type modifier token) */
#define UNSIGN      4   /* unsigned int (and unsigned modifier token) */
#define UCHAR       5   /* unsigned char */
#define ULONG       6   /* unsigned long int */
#ifdef  DOFLOATS
#define FLOAT       7   /* 32-bit floating point */
#define DOUBLE      8   /* 64-bit floating point */
#endif
#define STRUCT      9
#define UNION       10
#define LABEL       11
#define USTRUCT     12  /* incomplete struct or union type */
/* 13, 14, 15 free */

/* storage classes */
#define ARG         12  /* function arguments */
#define AUTO        13  /* "auto" storage (on stack) */
#define STRTAG      14  /* struct tag */
#define EXTERN      15
#define STATIC      16
#define REG         17  /* register storage */
#define MOS         18  /* member of struct or union */
#define DIRECT      19  /* on direct page */
#define EXTERND     20  /* extern direct */
#define STATICD     21  /* static direct */
#define EXTDEF      22

#define RETURN      23
#define IF          24
#define WHILE       25
#define ELSE        26
#define SWITCH      27
#define CASE        28
#define BREAK       29
#define CONTIN      30
#define DO          31
#define DEFAULT     32
#define FOR         33
#define GOTO        34
#define TYPEDEF     35
#define DEFTYPE     36
#define CAST        37

#define SIGN        38  /* mostly ignored */
#define SHORT       39	/* short type modifier */

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
#define INCBEF      60	/* ++x */
#define DECBEF      61	/* --x */
#define INCAFT      62	/* x++ */
#define DECAFT      63	/* x-- */
#define NOT         64
#define AMPER       65	/* & address-of operator */
#define STAR        66	/* pointer dereference */
#define NEG         67
#define COMPL       68
#define DOT         69	/* struct member access */
#define ARROW       70	/* struct member pointer access */
#define DBLAND      71	/* && logical AND */
#define DBLOR       72	/* || logical OR */
#define LCONST      74	/* long constant */
#ifdef DOFLOATS
#define FCONST      75	/* float constant */
#endif
#define UMOD        76	/* % modulus (unsigned) */
#define USHR        77	/* >> (unsigned) */
#define UDIV        78	/* unsigned integer division */
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

/* conditional branch types */
#define EQ          90
#define NEQ         91
/* HACK ALERT
	The fact that e.g. ULEQ is LEQ+4 is used by the code. Don't change these
	numbers in relation to one another!
*/
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
#define ITOL        131		/* int to long */
#define LTOI        132		/* long to int */
#define CTOI        133		/* char to int */
#define UTOI        134		/* unsigned char to int */
#define UTOL        135		/* unsigned to long */
#define DBLOP       136		/* double operation (any) */
#define LONGOP      137		/* long operation (any) */
#define MOVE        138
#define STOI        139
#define TEST        140
#ifdef DOFLOATS
#define FTOD        141
#define DTOF        142
#define ITOD        143
#define DTOI        144
#define LTOD        145
#define DTOL        146
#define UTOD        147
#endif
#define XIND        148		/* x indexed */
#define YIND        149		/* y indexed */
#define UIND        150		/* u indexed */

#define HALVE       151
#define UHALVE      152
#define IDOUBLE     153

#define ASSPLUS     160		/* += */
#define ASSMIN      161		/* -= */
#define ASSMUL      162		/* *= */
#define ASSDIV      163		/* /= */
#define ASSMOD      164		/* %= */
#define ASSSHR      165		/* >>= */
#define ASSSHL      166		/* <<= */
#define ASSAND      167		/* &= */
#define ASSOR       168		/* |= */
#define ASSXOR      169		/* ^= -- rarely seen */

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

#define islong(t)   ((t)->type == LONG || (t)->type == ULONG)
#define isuint(t)   ((t)->type == UNSIGN || (t)->type == UCHAR || (t)->type == ULONG)
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
# ifndef CKEYSFILE
#  ifdef MWOS
#   define CKEYSFILE "/dd/lib/ckeys"
#  else
#   define CKEYSFILE "/usr/local/share/dcc/ckeys"
#  endif
# endif
#endif

global direct int   dflag,  /* debug flag; if set causes tree prints */
                    lflag,  /* if set, source lines copied to code file */
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
char *grab();

extern  expnode *parsexp(), *primary(), *explist(), *getcast(),
                *newnode(), *optim(), *fold(), *chtype(), *fixup(),
                *tranexp(), *tranbool(), *treecopy();

extern  dimnode *dimwalk();
