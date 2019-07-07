#define NULL 0

char nullarg[]  = "";
char clra[]     = "clra";
char clrb[]     = "clrb";
char d[]        = "d";
char leax[]     = "leax";
char leay[]     = "leay";
char leau[]     = "leau";
char leas[]     = "leas";
char pshs[]     = "pshs";
char std[]      = "std";

typedef struct is {
    struct is *nis;
    char *mnem,*op;
} instr;

instr act1b     = {NULL,    leax,      "-2,x"};
instr act1a     = {&act1b,  leax,      "-2,x"};
instr rep1a     = {NULL,    leax,      "-4,x"};

instr act2f     = {NULL,    clra,       nullarg};
instr act2e     = {&act2f,  clrb,       nullarg};
instr act2d     = {&act2e,  pshs,       d};
instr act2c     = {&act2d,  clra,       nullarg};
instr act2b     = {&act2c,  clrb,       nullarg};
instr act2a     = {&act2b,  pshs,       d};
instr rep2d     = {NULL,    clra,       nullarg};
instr rep2c     = {&rep2d,  clrb,       nullarg};
instr rep2b     = {&rep2c,  pshs,       d};
instr rep2a     = {&rep2b,  pshs,       d};

instr act3b     = {NULL,    leas,       "2,s"};
instr act3a     = {&act2b,  pshs,       d};
instr rep3a     = {NULL,    std,        "0,s"};

instr act4b     = {NULL,    leas,       "2,s"};
instr act4a     = {&act4b,  std,        "-2,s"};
instr rep4a     = {NULL,    std,        ",s++"};

typedef struct ac {
    struct ac *nac;
    instr *act,*rep;
} action;

action act1 =   {NULL,  &act1a,     &rep1a};
action act2 =   {NULL,  &act2a,     &rep2a};
action act3 =   {NULL,  &act3a,     &rep3a};
action act4 =   {NULL,  &act4a,     &rep4a};
