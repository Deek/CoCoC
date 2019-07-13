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
    /* All of these use the same replacement pattern */
    act01b = {NULL,     stb,    NULL},
    act01a = {&act01b,  ldb,    1},
    act02b = {NULL,     std,    NULL},
    act02a = {&act02b,  ldd,    1},
    act03b = {NULL,     stx,    NULL},
    act03a = {&act03b,  ldx,    1},     rep01 = {NULL,      2,      NULL},

    act04f = {NULL,     clra,   NULL},
    act04e = {&act04f,  clrb,   NULL},
    act04d = {&act04e,  pshs,   d},     rep04c = {NULL,     clra,   ""},
    act04c = {&act04d,  clra,   NULL},  rep04b = {&rep04c,  clrb,   ""},
    act04b = {&act04c,  clrb,   NULL},  rep04a = {&rep04b,  pshs,   d},
    act04a = {&act04b,  pshs,   d},     rep04 = {&rep04a,   pshs,   d},

    act05b = {NULL,     leas,   p2s},
    act05a = {&act05b,  pshs,   d},     rep05 = {NULL,      std,    zs},

    act06b = {NULL,     leas,   p2s},
    act06a = {&act06b,  NULL,   m2s},   rep06 = {NULL,      1,      ",s++"},

    act07b = {NULL,     leau,   m2u},
    act07a = {&act07b,  NULL,   zu},    rep07 = {NULL,      1,      ",--u"},

    act08b = {NULL,     leau,   m1u},
    act08a = {&act08b,  NULL,   zu},    rep08 = {NULL,      1,      ",-u"},

    act09b = {NULL,     leau,   p1u},
    act09a = {&act09b,  NULL,   m1u},   rep09 = {NULL,      1,      ",u+"},

    act10b = {NULL,     leau,   p2u},
    act10a = {&act10b,  NULL,   m2u},   rep10 = {NULL,      1,      ",u++"},

    act11b = {NULL,     leax,   m2x},
    act11a = {&act11b,  NULL,   zx},    rep11 = {NULL,      1,      ",--x"},

    act12b = {NULL,     leax,   m1x},
    act12a = {&act12b,  NULL,   zx},    rep12 = {NULL,      1,      ",-x"},

    act13b = {NULL,     leax,   p1x},
    act13a = {&act13b,  NULL,   m1x},   rep13 = {NULL,      1,      ",x+"},

    act14b = {NULL,     leax,   p2x},
    act14a = {&act14b,  NULL,   m2x},   rep14 = {NULL,      1,      ",x++"},

    act15b = {NULL,     leay,   m2y},
    act15a = {&act15b,  NULL,   zy},    rep15 = {NULL,      1,      ",--y"},

    act16b = {NULL,     leay,   m1y},
    act16a = {&act16b,  NULL,   zy},    rep16 = {NULL,      1,      ",-y"},

    act17b = {NULL,     leay,   p1y},
    act17a = {&act17b,  NULL,   m1y},   rep17 = {NULL,      1,      ",y+"},

    act18b = {NULL,     leay,   p2y},
    act18a = {&act18b,  NULL,   m2y},   rep18 = {NULL,      1,      ",y++"},

    act19b = {NULL,     sex,    NULL},
    act19a = {&act19b,  cmpd,   litnum},rep19 = {NULL,      cmpb,   1},
    act20b = {NULL,     clra,   NULL},
    act20a = {&act20b,  cmpd,   litnum},

    act21b = {NULL,     sex,    NULL},
    act21a = {&act21b,  clra,   NULL},  rep21 = {NULL,      1,      1},

    act22b = {NULL,     pshs,   d},
    act22a = {&act22b,  pshs,   x},     rep22 = {NULL,      2,      dx},

    act23b = {NULL,     pshs,   d},
    act23a = {&act23b,  pshs,   y},     rep23 = {NULL,      2,      dy},

    act24b = {NULL,     pshs,   u},
    act24a = {&act24b,  leas,   m2s},   rep24 = {NULL,      2,      du},

    act25b = {NULL,     pshs,   u},
    act25a = {&act25b,  leas,   "-4,s"},rep25 = {NULL,      2,      dxu},

    act26b = {NULL,     pshs,   u},
    act26a = {&act26b,  leas,   "-6,s"},rep26 = {NULL,      2,      dxyu};


typedef struct act {
    struct act *nxtact;
    instr *actp,*repp;
} action;

action actions[] = {
    {1,     &act01a,    &rep01},
    {1,     &act02a,    &rep01},
    {1,     &act03a,    &rep01},
    {1,     &act04a,    &rep04},
    {1,     &act05a,    &rep05},
    {1,     &act06a,    &rep06},
    {1,     &act07a,    &rep07},
    {1,     &act08a,    &rep08},
    {1,     &act09a,    &rep09},
    {1,     &act10a,    &rep10},
    {1,     &act11a,    &rep11},
    {1,     &act12a,    &rep12},
    {1,     &act13a,    &rep13},
    {1,     &act14a,    &rep14},
    {1,     &act15a,    &rep15},
    {1,     &act16a,    &rep16},
    {1,     &act17a,    &rep17},
    {1,     &act18a,    &rep18},
    {1,     &act19a,    &rep19},
    {1,     &act20a,    &rep19},
    {1,     &act21a,    &rep21},
    {1,     &act22a,    &rep22},
    {1,     &act23a,    &rep23},
    {1,     &act24a,    &rep24},
    {1,     &act25a,    &rep25},
    {1,     &act26a,    &rep26},
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
//    { REPEAT,   sex,    0,      clra,   0,      2,      0,      0},
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
//    { EXIT,     sex,    0,      cmpd,   litnum, cmpb,   2,      0},
//    { EXIT,     clra,   0,      cmpd,   litnum, cmpb,   2,      0},
//    { EXIT,     leas,   p2s,    std,    m2s,    2,      ",s++", 0},
    { EXIT,     leas,   p2s,    pshs,   d,      std,    zs,     0},
    { EXIT,     leas,   p2s,    puls,   upc,    2,      xupc,   0},
//    { EXIT,     pshs,   u,      leas,   m2s,    1,      du,     0},
//    { EXIT,     pshs,   u,      leas,   "-4,s", 1,      dxu,    0},
//    { EXIT,     pshs,   u,      leas,   "-6,s", 1,      dxyu,   0},
    {0},
};
*/
