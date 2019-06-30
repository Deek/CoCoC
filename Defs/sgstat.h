/* structures for 'getstat()' and 'setstat()'                */
/* The following are for an SCF type device. See below for
 * structure member definitions for an RBF device.
 */
struct sgbuf {
   char sg_class,            /* device class, 0=scf, 1=rbf */
        sg_case,         /* 0 = both , 1 = upper case only */
        sg_backsp,              /* 0 = BSE, 1 = BSE-SP-BSE */
        sg_delete,                      /* delete sequence */
        sg_echo,                            /* 0 = no echo */
        sg_alf,                   /* 0 = no auto line feed */
        sg_nulls,                /* end of line null count */
        sg_pause,              /* 0 = no end of page pause */
        sg_page,                         /* lines per page */
        sg_bspch,                   /* backspace character */
        sg_dlnch,                 /* delete line character */
        sg_eorch,               /* end of record character */
        sg_eofch,                 /* end of file character */
        sg_rlnch,                /* reprint line character */
        sg_dulnch,        /* duplicate last line character */
        sg_psch,                        /* pause character */
        sg_kbich,          /* keyboard interrupt character */
        sg_kbach,              /* keyboard abort character */
        sg_bsech,              /* backspace echo character */
        sg_bellch,       /* line overflow character (bell) */
        sg_parity,       /* device initialisation (parity) */
        sg_baud;                              /* baud rate */
   int  sg_d2p;     /* offset to second device name string */
   char sg_xon,                         /* xmit start char */
        sg_xoff,                         /* xmit stop char */
        sg_err;                /* most recent error status */
   int  sg_tbl;           /* pointer to user avail dev tbl */
   char sg_spare[3];     /* spare bytes - to fill out buff */
};

struct rbfopt {
   char sg_class,    /* device class - repeated from above */
        sg_drive,                          /* drive number */
        sg_step,                              /* step rate */
        sg_dtype,                           /* device type */
        sg_dense;                    /* density capability */
   int  sg_cyls;           /* number of cylinders (tracks) */
   char sg_sides,                       /* number of sides */
        sg_verify;                 /* 0 = verify on writes */
   int  sg_spt,               /* default sectors per track */
        sg_spt0;                          /* ditto track 0 */
   char sg_intlv,              /* sector interleave factor */
        sg_salloc,              /* segment allocation size */
        sg_tfm;                       /* dma transfer mode */
   int  sg_exten;     /* path extension for record locking */
   char sg_xxxx,                              /* junk fill */
        sg_att,                         /* file attributes */
        sg_fdpsn[3],                /* file descriptor PSN */
        sg_dipsn[3];               /* file's directory PSN */
   long sg_dirptr;              /* directory entry pointer */
   int  sg_dvt;           /* address of device table entry */
};
