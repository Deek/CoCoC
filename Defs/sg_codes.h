#ifndef _SG_CODES_H_
#define _SG_CODES_H_

#ifndef _ANSI_H_
# include <ansi.h>		/* for _OP() */
#endif

#define DT_SCF		0	/* sequential character file type */
#define DT_RBF		1	/* random block file type */
#define DT_PIPE		2	/* pipe file type */
#define DT_SBF		3	/* sequential block file type */
#define DT_NFM		4	/* network file type */

#if defined(_OSK) || defined(_OS9000)
# define DT_CDFM	5	/* compact disc file type */
# define DT_UCM		6	/* user communication manager */
# define DT_SOCK	7	/* socket communication manager */
# define DT_PTTY	8	/* pseudo-keyboard manager */
# define DT_INET	9	/* internet interface manager */
# define DT_NRF		10	/* non-volatile ram file manager (CD-I variety) */
# define DT_GFM		11	/* graphics file manager */
# define DT_ISDN	12	/* ISDN file manager */
# define DT_MPFM	13	/* MPEG file manager */
# define DT_RTNFM	14	/* real time network file manager */
# define DT_SPF		15	/* serial protocol file manager */
# define DT_MFM		16	/* multimedia file manager (graphics & audio) */
# define DT_DVM		17	/* device manager */
# define DT_DVDFM	18	/* DVD file manager */
# define DT_MODFM	19	/* Module I/O file manager */
# define DT_NULL	20	/* DPIO NULL file manager */
# define DT_PCF		21	/* Reserved FUTURE .. 68k PCF returns DT_RBF */
#endif

/*
	Get/SetStat function codes
*/

#define SS_Opt		0x00	/* read/write PD options */
#define SS_OPT		0x00
#define SS_Ready	0x01	/* check for device ready */
#define SS_READY	0x01
#define SS_Size		0x02	/* read/write file size */
#define SS_SIZE		0x02
#define SS_Reset	0x03	/* device restore */
#define SS_RESET	0x03
#define SS_WTrk		0x04	/* device write track */
#define SS_WTRK		0x04
#define SS_Pos		0x05	/* get file current position */
#define SS_POS		0x05
#define SS_EOF		0x06	/* test for end of file */
#define SS_Link		0x07	/* link to status routines */
#define SS_LINK		0x07
#define SS_ULink	0x08	/* unlink status routines */
#define SS_ULINK	0x08
#define SS_Feed		0x09	/* destructive forward skip (form feed) */
#define SS_FEED		0x09
#define SS_Frz		0x0a	/* freeze DD_ information */
#define SS_FRZ		0x0a
#define SS_SPT		0x0b	/* set DD_TKS to given value */
#define SS_SQD		0x0c	/* sequence down hard disk */
#define SS_DCmd		0x0d	/* send direct command to device */
#define SS_DCMD		0x0d
#define SS_DevNm	0x0e	/* return device name */
#define SS_DEVNM	0x0e
#define SS_FD		0x0f	/* return file descriptor */
#define SS_Ticks	0x10	/* set lockout honor duration */
#define SS_TICKS	0x10
#define SS_Lock		0x11	/* lock/release record */
#define SS_LOCK		0x11
#define SS_DStat	0x12	/* return display status */
#define SS_DSTAT	0x12
#define SS_Joy		0x13	/* return joystick value */
#define SS_JOY		0x13
#define SS_BlkRd	0x14	/* block read */
#define SS_BLKRD	0x14
#define SS_BlkWr	0x15	/* block write */
#define SS_BLKWR	0x15
#define SS_Reten	0x16	/* retention cycle */
#define SS_RETEN	0x16
#define SS_WFM		0x17	/* write file mark */
#define SS_RFM		0x18	/* read past file mark */
#define SS_ELog		0x19	/* read error log */
#define SS_ELOG		0x19
#define SS_SSig		0x1a	/* send signal on data ready */
#define SS_SSIG		0x1a
#define SS_Relea	0x1b	/* release device */
#define SS_RELEA	0x1b
#define SS_Attr		0x1c	/* set file attributes */
#define SS_Break	0x1d	/* send break out serial device */
#define SS_RsBit	0x1e	/* reserve bitmap sector (for disk reorganization) */
#define SS_RMS		0x1f	/* get/set Motorola RMS status */
#define SS_FDInf	0x20	/* get FD info for specified FD sector */
#define SS_IFC		0x22	/* serial input flow control */
#define SS_OFC		0x23	/* serial output flow control */
#define SS_EnRTS	0x24	/* enable RTS (modem control) */
#define SS_DsRTS	0x25	/* disable RTS (modem control) */
#define SS_DCOn		0x26	/* send signal DCD TRUE */
#define SS_DCOff	0x27	/* send signal DCD FALSE */

#ifdef _OS9 /* OS-9/6809 */
# define SS_SetMF	0x24	/* reserved for Gimix G68 */
# if defined(_COCO_) || defined(_DRAGON_)
#  define SS_AlfaS	0x1c	/* get alfa display status */
#  define SS_Cursr	0x25	/* get cursor information */
#  define SS_ScSiz	0x26	/* get screen size */
#  define SS_KySns	0x27	/* get/set for CoCo keyboard */
#  define SS_ComSt	0x28	/* get/set for baud/parity */
#  define SS_COMST	0x28
#  define SS_HngUp	0x2b	/* set to tell driver to hangup phone */
#  define SS_FSig	0x2c	/* set to have signal sent on file change */
#  define SS_AAGBf	0x80	/* set to allocate additional graphic buffer */
#  define SS_SlGBf	0x81	/* set to select a different graphic buffer */
#  define SS_Mount	0x82	/* set to mount Network 4 Omnidrive */
#  define SS_RdNet	0x83	/* get to read raw sector from Network 4 Omnidrive */
#  define SS_MpGPB	0x84	/* set to map a Get/Put buffer into workspace */
#  define SS_Slots	0x85	/* get to list Network 4 host devices? */
#  define SS_CDSta	0x99	/* Serial: get carrier detect state? */
#  define SS_CDSig	0x9a	/* Serial: set to have signal sent on CD change */
#  define SS_CDRel	0x9b	/* Serial: release any pending CD signal */
#  define SS_Fill	0xa0	/* SCF: preload device input buffer */
#  define SS_Hist	0xa1	/* "enable command-line history easily" -- ??? */
#  if (_COCO_ >= 3 && _OS9LEVEL >= 2)
#   define SS_DSize	0x26	/* get disk size */
#   define SS_VarSect 0x12	/* get variable sector size info */
#   define SS_WnSet	0x86	/* set up high level windowing information */
#   define SS_MnSel	0x87	/* req. high-level menu handler process next event */
#   define SS_SBar	0x88	/* set position of scroll bar(s) */
#   define SS_Mouse	0x89	/* get mouse information packet */
#   define SS_MsSig	0x8a	/* set to receive signal on mouse event */
#   define SS_AScrn	0x8b	/* allocate a screen for direct access */
#   define SS_DScrn	0x8c	/* display a screen allocated with SS_AScrn */
#   define SS_FScrn	0x8d	/* free a screen allocated with SS_AScrn */
#   define SS_PScrn	0x8e	/* polymorph a screen into different screen type */
#   define SS_ScInf	0x8f	/* get current screen info for direct writes */
#   define SS_Palet	0x91	/* get palette information */
#   define SS_Montr	0x92	/* get/set monitor type */
#   define SS_ScTyp	0x93	/* get screen type */
#   define SS_GIP	0x94	/* set global input parameters */
#   define SS_UMBar	0x95	/* set to update menu bar */
#   define SS_FBRgs	0x96	/* get color registers */
#   define SS_DfPal	0x97	/* get/set default palette registers */
#   define SS_Tone	0x98	/* set to generate a tone */
#   define SS_GIP2	0x99	/* set global input params #2 (v3 upgrade) */
#   define SS_AnPal	0x9a	/* Animate palettes (v3 upgrade) */
#   define SS_FndBf	0x9b	/* Find named buffer (v3 upgrade) */
/* VRN driver get/set calls */
#   define SS_VCtr	0x80	/* get FS2 total VIRQ count */
#   define SS_VSig	0x81	/* get and reset FS2 signal send count */
#   define SS_FClr	0x81	/* set to clear FS2 VIRQ */
#   define SS_FSet	0xc7	/* set FS2+ VIRQ */
#   define SS_KSet	0xc8	/* set KQ3 VIRQ */
#   define SS_KClr	0xc9	/* set to clear KQ3 VIRQ */
#   define SS_ARAM	0xca	/* set: allocate contiguous RAM blocks */
#   define SS_DRAM	0xcb	/* set: deallocate (free) allocated RAM blocks */
/* SDisk 3 get/set calls */
#   define SS_DRead	0x80	/* Direct Sector Read */
#   define SS_DWrit	0x80	/* Direct sector Write */
#   define SS_UnFrz	0x81	/* UNFreeze DD info */
#   define SS_MOFF	0x82	/* fast motor off call */
#   define SS_MoTim	0x83	/* Set motor shut off time */
#   define SS_SDRD	0x84	/* System direct read */
#   define SS_SDWRT	0x84	/* System direct writes */
#   define SS_Sleep	0x85	/* (DMC ONLY) enable/disable F$Sleep calls in read/write 1773 I/O */
#   define SS_DrvCh	0x86	/* (DMC ONLY) set/get cached drive number */
#  endif /* _COCO_ 3 && _OS9LEVEL 2 */
# endif /* _COCO_ || _DRAGON_ */
# define SS_Open	0x29	/* notification of new path opened */
# define SS_Close	0x2a	/* notification of path being closed */
#endif

#ifdef _OSK
# define SS_ACRTC	0x21	/* get/set Hitachi ACRTC status */
# define SS_Skip	0x28	/* skip block(s) */
# define SS_Mode	0x29	/* set RBF access mode */
# define SS_Open	0x2a	/* notification of new path opened */
# define SS_Close	0x2b	/* notification of path being closed */
# define SS_Path	0x2c	/* CDFM return pathlist for open path */
# define SS_Play	0x2d	/* CDFM play (CD-I) file */
# define SS_HEADER	0x2e	/* CDFM return header of last sector read */
# define SS_Raw		0x2f	/* CDFM read raw sectors */
# define SS_Seek	0x30	/* CDFM issue physical seek command */
# define SS_Abort	0x31	/* CDFM abort asynchronous operation in progress */
# define SS_CDDA	0x32	/* CDFM play CD digital audio */
# define SS_Pause	0x33	/* CDFM pause the disc driver */
# define SS_Eject	0x34	/* CDFM open the drive door */
# define SS_Mount	0x35	/* CDFM mount disc by disc number */
# define SS_Stop	0x36	/* CDFM stop the disc drive */
# define SS_Cont	0x37	/* CDFM start the disc after pause */
# define SS_Disable	0x38	/* CDFM disable hardware controls */
# define SS_Enable	0x39	/* CDFM enable hardware controls */
# define SS_ReadToc	0x3a	/* CDFM read TOC (on red discs) */
# define SS_SM		0x3b	/* CDFM's soundmap control status code */
# define SS_SD		0x3c	/* CDFM's sound data manipulation status code */
# define SS_SC		0x3d	/* CDFM's sound control status code */

# define SS_SEvent	0x3e	/* set event on data ready */
# define SS_Sound	0x3f	/* produce audible sound */
# define SS_DSize	0x40	/* get drive size (in sectors) */
# define SS_Net		0x41	/* NFM wild card getstat/setstat, with subcode */
# define SS_Rename	0x42	/* rename file */
# define SS_Free	0x43	/* return free statistics (NRF) */
# define SS_VarSect	0x44	/* variable sector size supported query */
# define SS_VolStore 0x45	/* return volume storage statistics */
# define SS_MIDI	0x46	/* MIDI wild card getstat/setstat, with subcode */
# define SS_ISDN	0x47	/* ISDN getstat/setstat, with subcode */
# define SS_PMOD	0x48	/* Protocol module get/setstat, with subcode */
# define SS_SPF		0x48	/* SPF get/setstat, with subcode: SAME as SS_PMOD */
# define SS_LUOPT	0x49	/* DPIO: get/set logical unit options */
# define SS_RTNFM	0x4a	/* RTNFM set/getstat with sub-code */
# define SS_SYMLINK	0x4b	/* create symbolic link to file */
# define SS_UCM		0x4c	/* UCM reserved */
# define SS_DM		0x51	/* UCM's drawmap control status code */
# define SS_GC		0x52	/* UCM's graphics cursor status code */
# define SS_RG		0x53	/* UCM's region status code */
# define SS_DP		0x54	/* UCM's drawing parameters status code */
# define SS_DR		0x55	/* UCM's graphics drawing status code */
# define SS_DC		0x56	/* UCM's display control status code */
# define SS_CO		0x57	/* UCM's character output status code */
# define SS_VIQ		0x58	/* UCM's video inquiry status code */
# define SS_PT		0x59	/* UCM's pointer status code */
# define SS_SLink	0x5a	/* UCM link external subroutine module to UCM */
# define SS_KB		0x5b	/* keyboard status code */

/*
	Get/SetStat function codes for sockets
*/

# define SS_Bind	0x6c	/* bind a socket name */
# define SS_Listen	0x6d	/* listen for connections */
# define SS_Connect	0x6e	/* initiate a connection */
# define SS_Resv	0x6f	/* socket characteristics specification */
# define SS_Accept	0x70	/* accept socket connections */
# define SS_Recv	0x71	/* receive data */
# define SS_Send	0x72	/* send data */
# define SS_GNam	0x73	/* get socket name */
# define SS_SOpt	0x74	/* set socket option */
# define SS_GOpt	0x75	/* get socket option */
# define SS_Shut	0x76	/* shut down socket connection */
# define SS_SendTo	0x77	/* send to address */
# define SS_RecvFr	0x78	/* receive from address */
# define SS_Install	0x79	/* install upper level protocol (ULP) */
							/* handler on ifman list */
# define SS_PCmd	0x7a	/* protocol direct command */
# define SS_FTL		0x7b	/* flash translation layer get/set w/subcode */
# define SS_DDC_FUNC 0x7c	/* Get/Set Stat code for Direct Driver Call */
							/* (with subcodes) */
# define SS_SN		0x8c	/* DSM's screen functions */
# define SS_AR		0x8d	/* DSM's action region functions */
# define SS_MS		0x8e	/* DSM's message functions */
# define SS_AC		0x8f	/* DSM's action cursor functions */
# define SS_CDFD	0x90	/* CDFM return file descriptor information */
# define SS_CChan	0x91	/* CDFM change channel request */
# define SS_FG		0x92	/* GFM frame grabber set/getstat functions */
# define SS_XWin	0x93	/* X windows set/getstat functions */
# define SS_SOCKMAN	0x94	/* Get/Set Stat code for SOCKMAN */
							/* (with subcodes)*/
# define SS_MAUI	0x95	/* GFX/SND Get/Set Stat code for MAUI */
							/* (with subcodes) */
# define SS_ETC		0x96	/* Embedded Traffic Controller (with subcodes) */
# define SS_RSIG	0x97	/* Send Sig on data Ready count */
# define SS_WSIG	0x98	/* Send Sig on write buffer free count */
# define SS_WRDY	0x99	/* GetStat returns Buffer free space and size */
# define SS_FLUSHBUF 0x9a	/* Flush read and/or write buffers in driver */
# define SS_MAUIDEV	0x9b	/* CDB/MSG Get/Set Stat code for MAUI */
							/* (with subcodes) */
#endif /* _OSK */

int getstat _OP((int code, int fd, ...));
int _gs_devn _OP((int fd, char *buf));
int _gs_eof _OP((int fd));
int _gs_gfd _OP((int fd, struct fildes *buf, int buflen));
int _gs_opt _OP((int fd, struct sgbuf *opts));
int _gs_rdy _OP((int fd));
long _gs_pos _OP((int fd));
long _gs_size _OP((int fd));

int setstat _OP((int code, int fd, ...));

int _ss_attr _OP((int fd, short int attrs));
int _ss_lock _OP((int fd, long int locksize));
int _ss_opt _OP((int fd, struct sgbuf *opts));
int _ss_pfd _OP((int fd, struct fildes *buf));
int _ss_rel _OP((int fd));
int _ss_rest _OP((int fd));
int _ss_size _OP((int fd, long size));
int _ss_ssig _OP((int fd, short int code));
int _ss_tiks _OP((int fd, int count));
int _ss_wtrk _OP((int fd, int track, int side, char *buf));

#endif /* _SG_CODES_H_ */
