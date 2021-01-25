/* Action Instruction Strings */

typedef struct ins {
    struct ins *nxtins;
    char *mnp,*opp;
} instr;

typedef struct act {
    struct act *nxtact;
    instr *actp,*repp;
} action;

/*
PUSH ORDER: pc,u/s,y,x,dp,{b,a|d},cc
PULL ORDER: cc,{d|a,b},dp,x,y,u/s,pc
*/
