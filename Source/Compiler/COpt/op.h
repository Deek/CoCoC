#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LABELSIZE       11
#define MNEMSIZE        10
#define ARGSIZE         90

#define LINESIZE 100

#define LABEL       1
#define MNEM        2
#define ARGS        3
#define BRALIM     36	/* used to calculate branch distance somehow */
#ifdef _OS9
#define MAXINS    300
#else
#define MAXINS    32767
#endif

/* instruction type flags */
#define SUBR        0x0001
#define BRANCH      0x0020
#define BRATYPE     0x001F
#define BRAMASK     (BRANCH|BRATYPE)
#define LONG        0x0040
#define LONGBRA     (LONG|BRANCH)
#define BLOCKDUP    0x0080
#define CODEBRK     0x0100
#define DESTPTR     0x0200

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#define FORWARD 4
#define BACK    5

#ifndef _OS9
# define direct
#endif

#define GLOBAL      0x01    /* global label flag */

#if defined (__STDC__)
# if defined(__STDC_VERSION__)
#  if (__STDC_VERSION__ >= 199409L) /* C94+ */
#   define HAVE_C94	1
#  endif
#  if (__STDC_VERSION__ >= 199901L) /* C99+ */
#   define HAVE_C99	1
#  endif
#  if (__STDC_VERSION__ >= 201112L) /* C11+ */
#   define HAVE_C11	1
#  endif
#  if (__STDC_VERSION__ >= 201710L) /* C18+ */
#   define HAVE_C18	1
#  endif
# endif
#endif

typedef struct chstruct {
    struct chstruct *succ,*pred;
} chain;

typedef struct lstruct {
    struct lstruct *succ,*pred;
#ifdef HAVE_C11
    struct istruct *dest;
#else
    union istruct *dest;
#endif
    chain *rlist;
    struct lstruct *nextl;
    int lflags;
    char lname[LABELSIZE+1];
} label;

#ifdef HAVE_C11
typedef struct istruct {
    struct istruct *succ,*pred;
    struct lstruct *llist;
    int itype;
    char mnem[MNEMSIZE+1];
    union {
        char *args;
        struct lstruct *lab;
    };
} instruction;
#else
typedef union istruct {
    struct {
        union istruct *succ,*pred;
        struct lstruct *llist;
        int itype;
        char mnem[MNEMSIZE+1];
        char *args;
    } instyp1;
    struct {
        union istruct *succ,*pred;
        struct lstruct *llist;
        int itype;
        char mnem[MNEMSIZE+1];
        struct lstruct *lab;
    } instyp2;
} instruction;
#endif

extern direct chain ilist;
extern chain ltable[],*newchain();
extern char *newarg(),*grab();
extern label *inslabel(),*newlabel(),*findlabel();
extern direct label *lfree;
extern instruction *insins(),*newins();
extern direct int inscount;
extern FILE *in,*out,*datfile;
extern direct int lbf,lbdone,opsdone;
extern direct int dbflag,l2flag;
extern char *parse();

extern match();
extern matchcpy();

#ifndef CONFDIR
# if defined(_OS9) || defined(_OSK) || defined(_OS9000)
#  define CONFDIR "/DD/Lib"
# else
#  define CONFDIR "/usr/local/share/dcc"
# endif
#endif
