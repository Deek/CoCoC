/*
** f i x p a t h
**  initialize a new descriptor
**   delete any lines not necessary to copy to new
*/

fixpath(pn)
int   pn;
   {
   static struct  sgbuf newstat, oldstat;

   getstat(SS_OPT, pn, &oldstat);                  /* get port parms */

   setmem(&newstat, sizeof(newstat), 0);  /* zero out new descriptor */

   /* now copy over necessary bytes */
   newstat.sg_class = oldstat.sg_class;   /* dev class, 0=scf, 1=rbf */
   newstat.sg_case= oldstat.sg_case;     /* 0 = both, 1 = upper only */
   newstat.sg_backsp = oldstat.sg_backsp; /* 0 = BSE, 1 = BSE-SP-BSE */
   newstat.sg_delete = oldstat.sg_delete;         /* delete sequence */
   newstat.sg_echo = oldstat.sg_echo;                 /* 0 = no echo */
   newstat.sg_alf = oldstat.sg_alf;         /* 0 = no auto line feed */
   newstat.sg_nulls = oldstat.sg_nulls;    /* end of line null count */
   newstat.sg_pause = oldstat.sg_pause;  /* 0 = no end of page pause */
   newstat.sg_page = oldstat.sg_page;              /* lines per page */
   newstat.sg_bspch = oldstat.sg_bspch;       /* backspace character */
   newstat.sg_dlnch = oldstat.sg_dlnch;     /* delete line character */
   newstat.sg_eorch = oldstat.sg_eorch;   /* end of record character */
   newstat.sg_eofch = oldstat.sg_eofch;     /* end of file character */
   newstat.sg_rlnch = oldstat.sg_rlnch;    /* reprint line character */
   newstat.sg_dulnch = oldstat.sg_dulnch;/* duplicate last line char */
   newstat.sg_psch = oldstat.sg_psch;             /* pause character */
   newstat.sg_kbich = oldstat.sg_kbich;    /* keyboard int character */
   newstat.sg_kbach = oldstat.sg_kbach;       /* keyboard abort char */
   newstat.sg_bsech = oldstat.sg_bsech;       /* backspace echo char */
   newstat.sg_bellch = oldstat.sg_bellch;    /* overflow char (bell) */
   newstat.sg_parity = oldstat.sg_parity;    /* device init (parity) */
   newstat.sg_baud = oldstat.sg_baud;                   /* baud rate */
   newstat.sg_d2p = oldstat.sg_d2p;    /* offset to second dev name  */
   newstat.sg_xon = oldstat.sg_xon;               /* xmit start char */
   newstat.sg_xoff = oldstat.sg_xoff;              /* xmit stop char */
   newstat.sg_err = oldstat.sg_err;      /* most recent error status */
   newstat.sg_tbl = oldstat.sg_tbl; /* pointer to user avail dev tbl */
   }


