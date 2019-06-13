/* Mouse.h - Definitions for mouse data structures */

/* structure for reading mouse info packet */

typedef struct rmousin {
    char pt_valid,      /* is info valid ? */
         pt_actv,       /* active side */
         pt_totm,       /* time out initial value */
         pt_rsrv0[2],   /* reserved */
         pt_ttto,       /* time till timeout */
         pt_tsst[2],    /* time since start counter */
         pt_cbsa,       /* current button state - Button A */
         pt_cbsb,       /* current button state - Button B */
         pt_ccta,       /* click count                   A */
         pt_cctb,       /* click count                   B */
         pt_ttsa,       /* time this state               A */
         pt_ttsb,       /* time this state               B */
         pt_tlsa,       /* time last state               A */
         pt_tlsb,       /* time last state               B */
         pt_rsrv1[6],   /* reserved */
         pt_stat,       /* window pointer type location */
         pt_res;        /* resolution */
    int  pt_acx,        /* actual x value */
         pt_acy,        /* actual y value */
         pt_wrx,        /* window relative x value */
         pt_wry;        /* window relative y value */
} MSRET;


/* window regions for mouse */

#define WR_CNTNT 0      /* content region */
#define WR_CNTRL 1      /* control region */
#define WR_OFWIN 2      /* off window */


