/* Action Instruction Strings */

static char
    d[]     = "d",          x[]     = "x",          y[]     = "y",
    u[]     = "u",          dx[]    = "d,x",        dy[]    = "d,y",
    du[]    = "d,u",        dxu[]   = "d,x,u",      dxyu[]  = "d,x,y,u",
    xupc[]  = "x,u,pc",     yd[]    = "y,d",        yu[]    = "y,u",
    ud[]    = "u,d",        upc[]   = "u,pc",
    clra[]  = "clra",       clrb[]  = "clrb",
    cmpb[]  = "cmpb",       cmpd[]  = "cmpd",       cmpx[]  = "cmpx",
    cmpy[]  = "cmpy",       cmpu[]  = "cmpu",       ldb[]   = "ldb",
    ldd[]   = "ldd",        ldx[]   = "ldx",        ldy[]   = "ldy",
    ldu[]   = "ldu",        stb[]   = "stb",        std[]   = "std",
    stx[]   = "stx",        sty[]   = "sty",        stu[]   = "stu",
    pshs[]  = "pshs",       puls[]  = "puls",       leax[]  = "leax",
    leay[]  = "leay",       leau[]  = "leau",       leas[]  = "leas",
    sex[]   = "sex",        tfr[]   = "tfr",        zero[]  = "#0",
    p2x[]   = "2,x",        p1x[]   = "1,x",        zx[]    = "0,x",
    m1x[]   = "-1,x",       m2x[]   = "-2,x",       p2y[]   = "2,y",
    p1y[]   = "1,y",        zy[]    = "0,y",        m1y[]   = "-1,y",
    m2y[]   = "-2,y",       p2u[]   = "2,u",        p1u[]   = "1,u",
    zu[]    = "0,u",        m1u[]   = "-1,u",       m2u[]   = "-2,u",
    p2s[]   = "2,s",        zs[]    = "0,s",        m2s[]   = "-2,s",
    litnum[]= "#<";

typedef struct ins {
    struct ins *nxtins;
    char *mnp,*opp;
} instr;

static instr
    act1b = {NULL,      std,    NULL},
    act1a = {&act1b,    ldd,    1},     rep1a = {NULL,      std,    NULL},

    act2f = {NULL,      clra,   NULL},
    act2e = {&act2f,    clrb,   NULL},
    act2d = {&act2e,    pshs,   d},     rep2d = {NULL,      clra,   NULL},
    act2c = {&act2d,    clra,   NULL},  rep2c = {&rep2d,    clrb,   NULL},
    act2b = {&act2c,    clrb,   NULL},  rep2b = {&rep2c,    pshs,   d},
    act2a = {&act2b,    pshs,   d},     rep2a = {&rep2b,    pshs,   d},

    act3b = {NULL,      leas,   p2s},
    act3a = {&act3b,    pshs,   d},     rep3a = {NULL,      std,    zs},

    act4b = {NULL,      leas,   p2s},
    act4a = {&act4b,    std,    m2s},   rep4a = {NULL,      std,    ",s++"};

typedef struct act {
    struct act *nxtact;
    instr *actp,*repp;
} action;

action actions[] = {
    {1,     &act1a,     &rep1a},
    {1,     &act2a,     &rep2a},
    {1,     &act3a,     &rep3a},
    {1,     &act4a,     &rep4a},
    {0,     NULL,       NULL},
};

/*
action actions[] = {
    { EXIT,     stx,    0,      ldx,    1,      1,      1,      0},
    { EXIT,     stb,    0,      ldb,    1,      1,      1,      0},
    { EXIT,     ldd,    0,      std,    1,      1,      1,      0},
    { EXIT,     ldx,    0,      stx,    1,      1,      1,      0},
    { REPEAT,   tfr,    yd,     pshs,   d,      2,      y,      0},
    { REPEAT,   tfr,    ud,     pshs,   d,      2,      u,      0},
    { EXIT,     pshs,   x,      pshs,   d,      1,      dx,     0},
    { EXIT,     pshs,   u,      pshs,   d,      1,      du,     0},
    { EXIT,     pshs,   y,      pshs,   d,      1,      dy,     0},
    { EXIT,     pshs,   u,      pshs,   y,      1,      yu,     0},
    { EXIT,     ldd,    0,      cmpd,   zero,   1,      1,      0},
    { EXIT,     ldx,    0,      cmpx,   zero,   1,      1,      0},
    { EXIT,     ldy,    0,      cmpy,   zero,   1,      1,      0},
    { EXIT,     ldu,    0,      cmpu,   zero,   1,      1,      0},
    { EXIT,     sty,    0,      cmpy,   zero,   1,      1,      0},
    { EXIT,     stu,    0,      cmpu,   zero,   1,      1,      0},
    { EXIT,     ldb,    0,      stb,    1,      1,      1,      0},
    { REPEAT,   sex,    0,      clra,   0,      2,      0,      0},
    { EXIT,     leax,   p1x,    0,      m1x,    2,      ",x+",  0},
    { EXIT,     leay,   p1y,    0,      m1y,    2,      ",y+",  0},
    { EXIT,     leau,   p1u,    0,      m1u,    2,      ",u+",  0},
    { EXIT,     leax,   m1x,    0,      zx,     2,      ",-x",  0},
    { EXIT,     leay,   m1y,    0,      zy,     2,      ",-y",  0},
    { EXIT,     leau,   m1u,    0,      zu,     2,      ",-u",  0},
    { EXIT,     leax,   p2x,    0,      m2x,    2,      ",x++", 0},
    { EXIT,     leay,   p2y,    0,      m2y,    2,      ",y++", 0},
    { EXIT,     leau,   p2u,    0,      m2u,    2,      ",u++", 0},
    { EXIT,     leax,   m2x,    0,      zx,     2,      ",--x", 0},
    { EXIT,     leay,   m2y,    0,      zy,     2,      ",--y", 0},
    { EXIT,     leau,   m2u,    0,      zu,     2,      ",--u", 0},
    { EXIT,     sex,    0,      cmpd,   litnum, cmpb,   2,      0},
    { EXIT,     clra,   0,      cmpd,   litnum, cmpb,   2,      0},
    { EXIT,     leas,   p2s,    std,    m2s,    2,      ",s++", 0},
    { EXIT,     leas,   p2s,    pshs,   d,      std,    zs,     0},
    { EXIT,     leas,   p2s,    puls,   upc,    2,      xupc,   0},
    { EXIT,     pshs,   u,      leas,   m2s,    1,      du,     0},
    { EXIT,     pshs,   u,      leas,   "-4,s", 1,      dxu,    0},
    { EXIT,     pshs,   u,      leas,   "-6,s", 1,      dxyu,   0},
    {0},
};
*/
