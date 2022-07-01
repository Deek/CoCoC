#ifndef _FUNCS_H_
#define _FUNCS_H_

#define F_ALLBIT	0x13	/* F$AllBit:Allocate in Bit Map */
#define F_CHAIN		0x05	/* F$Chain:	Chain Process to New Module */
#define F_CMPNAM	0x11	/* F$CmpNam:Compare Two Names */
#define F_CRC		0x17	/* F$CRC:	Generate CRC */
#define F_DELBIT	0x14	/* F$DelBit:Deallocate in Bit Map */
#define F_EXIT		0x06	/* F$Exit:	Terminate Process */
#define F_FORK		0x03	/* F$Fork:	Start New Process */
#define F_ICPT		0x09	/* F$Icpt:	Set Signal Intercept */
#define F_ID		0x0c	/* F$ID:	Return Process ID */
#define F_LINK		0x00	/* F$Link:	Link to Module */
#define F_LOAD		0x01	/* F$Load:	Load Module from File */
#define F_MEM		0x07	/* F$Mem:	Set Memory Size */
#define F_PERR		0x0f	/* F$PErr:	Print Error */
#define F_PRSNAM	0x10	/* F$PrsNam:Parse Pathlist Name */
#define F_SCHBIT	0x12	/* F$SchBit:Search Bit Map */
#define F_SEND		0x08	/* F$Send:	Send Signal to Process */
#define F_SLEEP		0x0a	/* F$Sleep:	Suspend Process */
#define F_SPRIOR	0x0d	/* F$SPrior:Set Process Priority */
#define F_SSPD		0x0b	/* F$SSpd:	Suspend Process */
#define F_SSVC		0x32	/* F$SSvc:	Service Request Table Initialization */
#define F_SSWI		0x0e	/* F$SSWI:	Set Software Interrupt */
#define F_STIME		0x16	/* F$STime:	Set Current Time */
#define F_STRAP F_SSWI		/* F$STrap:	Set Trap Intercept */
#define F_TIME		0x15	/* F$Time:	Get Current Time */
#define F_UNLINK	0x02	/* F$UnLink:Unlink Module */
#define F_WAIT		0x04	/* F$Wait:	Wait for Child Process to Die */
#ifndef NOSYSTEM	/* System mode system calls (L1+) */
# define F_ALL64	0x30	/* F$All64:	Allocate 64-byte memory block */
# define F_ALLPD F_ALL64	/* F$AllPD:	Allocate Process/Path Descriptor */
# define F_APROC	0x2c	/* F$AProc:	Enter Active Process Queue */
# define F_FIND64	0x2f	/* F$Find64:Find 64-byte memory block */
# define F_FINDPD F_FIND64	/* F$FindPD:Find Process/Path Descriptor */
# define F_IODEL	0x33	/* F$IODel:	Delete I/O Module */
# define F_IOQU		0x2b	/* F$IOQu:	Enter I/O Queue */
# define F_IRQ		0x2a	/* F$IRQ:	Enter IRQ Polling Table */
# define F_NPROC	0x2d	/* F$NProc:	Start Next Process */
# define F_RET64	0x31	/* F$Ret64:	Return 64-byte memory block */
# define F_RETPD F_RET64	/* F$RetPD:	Return Process/Path Descriptor */
# define F_SRQMEM	0x28	/* F$SRqMem:Request System Memory */
# define F_SRTMEM	0x29	/* F$SRtMem:Return (free) System Memory */
# define F_VMODUL	0x2e	/* F$VModul:Validate Module */
#endif
#if defined(_OSK) || (defined(_OS9LEVEL) && _OS9LEVEL >= 2) /* Level 2 || OSK */
# define F_CLRBLK	0x50	/* F$ClrBlk:Clear Specific Blocks */
# define F_CPYMEM	0x1b	/* F$CpyMem:Copy External Memory */
# define F_GBLKMP	0x19	/* F$GBlkMp:get System Block Map copy */
# define F_GMODDR	0x1a	/* F$GModDr:get Module Directory copy */
# define F_GPRDSC	0x18	/* F$GPrDsc:get Process Descriptor copy */
# define F_SUSER	0x1c	/* F$SUser:	Set User ID number */
# define F_UNLOAD	0x1d	/* F$UnLoad:Unlink Module by name */
# ifndef NOSYSTEM
#  define F_ALLIMG	0x3a	/* F$AllImg:Allocate Image RAM blocks (sys) */
#  define F_ALLPRC	0x4b	/* F$AllPrc:Allocate Process Descriptor (sys) */
#  define F_ALLRAM	0x39	/* F$AllRAM:Allocate RAM blocks (sys) */
#  define F_ALLTSK	0x3f	/* F$AllTsk:Allocate Process Task number (sys) */
#  define F_DATLOG	0x44	/* F$DATLog:Convert DAT Block/Offset to Logical (sys) */
#  define F_DATTMP	0x45	/* F$DATTmp:Make temporary DAT image */
#  define F_DELIMG	0x3b	/* F$DelImg:Deallocate Image RAM blocks (sys) */
#  define F_DELPRC	0x4c	/* F$DelPrc:Deallocate Process Descriptor (sys) */
#  define F_DELRAM	0x51	/* F$DelRAM:Deallocate RAM blocks (sys) */
#  define F_DELTSK	0x40	/* F$DelTsk:Deallocate Process Task number (sys) */
#  define F_ELINK	0x4d	/* F$ELink:	Link using Module Directory Entry (sys) */
#  define F_FMODUL	0x4e	/* F$FModul:Find Module Directory Entry (sys) */
#  define F_FREEHB	0x3e	/* F$FreeHB:Get Free High Block (sys) */
#  define F_FREELB	0x3d	/* F$FreeLB:Get Free Low Block (sys) */
#  define F_GPROCP	0x37	/* F$GProcP:Get Process ptr (sys) */
#  define F_LDABX	0x49	/* F$LDABX:	Load A from 0,X in task B (sys) */
#  define F_LDAXY	0x46	/* F$LDAXY:	Load A [X,[Y]] (sys) */
#  define F_LDAXYP	0x47	/* F$LDAXYP:Load A [X+,[Y]] (sys) */
#  define F_LDDDXY	0x48	/* F$LDDDXY:Load D [D+X,[Y]] (sys) */
#  define F_MAPBLK	0x4f	/* F$MapBlk:Map Specific Block (sys) */
#  define F_MOVE	0x38	/* F$Move:	Move Data (sys) */
#  define F_PERMIT F_ALLIMG	/* F$Permit:Allocate Image RAM blocks (sys) */
#  define F_PROTECT F_DELIMG/* F$Protect:Deallocate Image RAM blocks (sys) */
#  define F_RELTSK	0x43	/* F$RelTsk:Release Task number (sys) */
#  define F_RESTSK	0x42	/* F$ResTsk:Reserve Task number (sys) */
#  define F_SETIMG	0x3c	/* F$SetImg:Set Process DAT Image (sys) */
#  define F_SETTSK	0x41	/* F$SetTsk:Set Process Task DAT registers (sys) */
#  define F_STABX	0x4a	/* F$STABX:	Store A at 0,X in task B (sys) */
# endif
#endif
#ifdef _OS9		/* These are missing from OS9/68000 */
# define F_TPS		0x25	/* F$TPS:	Get System Ticks Per Second */
# if (defined(_OS9LEVEL) && _OS9LEVEL >= 2)
#  define F_BOOT	0x35	/* F$Boot:	Bootstrap System (sys) */
#  define F_BTMEM	0x36	/* F$BtMem:	Bootstrap Memory Request (sys) */
#  define F_GCMDIR	0x52	/* F$GCMDir:Pack module directory (sys) */
#  define F_SLINK	0x34	/* F$SLink:	System Link (sys) */
#  define F_VIRQ	0x27	/* F$VIRQ:	Install Virtual Interrupt (sys) */
#  if defined(_COCO_) || defined(_NITROS9)	/* CoCo-specific OS-9 syscalls */
#   define F_ALARM	0x1e	/* F$Alarm:	Set a system alarm, or send signal */
#   define F_ALLHRAM 0x53	/* F$AllHRAM:Allocate high physical memory (sys) */
#   define F_NMLINK	0x21	/* F$NMLink:Link to module without mapping */
#   define F_NMLOAD	0x22	/* F$NMLoad:Load module from file without mapping */
#   define F_NVRAM	0x71	/* F$NVRAM:	Nonvolatile RAM read/write */
#   define F_REBOOT	0x54	/* F$ReBoot:Reboot machine or reload OS9Boot */
#   define F_REGDMP	0x70	/* F$RegDmp:Dump registers (needs KrnP4) */
#   define F_VBLOCK	0x57	/* F$VBlock:Verify memory block & add to mdir */
#   define F_XTIME	0x56	/* F$XTime:	Get extended time packet from RTC */
#  endif/* _OS9LEVEL >= 2 && (_COCO_ || _NITROS9) */
# endif	/* _OS9LEVEL >= 2 */
#endif	/* _OS9 */
#ifdef _NITROS9
# define F_CRCMOD	0x55	/* F$CRCMod:Set kernel CRC checking state */
# define F_DEBUG	0x23	/* F$Debug:	Debug (Reboot) (sys) */
#endif/* _NITROS9 */
#ifdef _OSK		/* These don't exist in OS-9/6809 */
# define F_ALARM	0x56	/* F$Alarm:	send alarm signal */
# define F_CCTL		0x5a	/* F$CCtl:	cache control */
# define F_CHKMEM	0x58	/* F$ChkMem:determine if user process may access memory area */
# define F_CRYPT	0x67	/* F$Crypt:	Perform Cryptographic Functions */
# define F_DATMOD	0x25	/* F$DatMod:Create data module */
# define F_DATTACH	0x64	/* F$DAttach:Debugger attach to running process */
# define F_DEXEC	0x23	/* F$DExec:	Debugging execution call (single step) */
# define F_DEXIT	0x24	/* F$DExit:	Debugging exit call (kill child) */
# define F_DFORK	0x22	/* F$DFork:	Debugging Fork call */
# define F_EVENT	0x53	/* F$Event:	Create/Link to named event */
# define F_FIRQ		0x61	/* F$FIRQ:	Add/Remove Fast IRQ service */
# define F_FLASH	0x65	/* F$Flash:	Manage FLASH device(s) */
# define F_GPRDBT	0x1f	/* F$GPrDBT:Get system global data copy */
# define F_GREGOR	0x54	/* F$Gregor:Convert julian date to gregorian date */
# define F_GSPUMP	0x5b	/* F$GSPUMp:get SPU map information for a process */
# define F_HLPROTO	0x70	/* F$HLProto:High-Level Protocol manager request */
# define F_JULIAN	0x20	/* F$Julian:Convert gregorian to Julian date */
# define F_MBUF		0x5f	/* F$MBuf:	Memory buffer manager */
# define F_PANIC	0x5e	/* F$Panic:	Panic warning */
# define F_POSK		0x5d	/* F$POSK:	execute svc request */
# define F_PWRMAN	0x66	/* F$PwrMan:Perform Power Management functions */
# define F_RTE		0x1e	/* F$RTE:	Return from Intercept routine */
# define F_SEMA		0x62	/* F$Sema:	Semphore P/V (reserve/release) service */
# define F_SETCRC	0x26	/* F$SetCRC:Generate valid header and CRC in module */
# define F_SETSYS	0x27	/* F$SetSys:Set/examine system global variable */
# define F_SIGMASK	0x57	/* F$SigMask:set signal mask */
# define F_SIGREST	0x63	/* F$SigReset:Reset signal intercept context */
# define F_SRQCMEM	0x5c	/* F$SRqCMem:System Colored Memory Request */
# define F_SYSDBG	0x52	/* F$SysDbg:Invoke system level debugger */
# define F_SYSID	0x55	/* F$SysID:	return system identification */
# define F_TLINK	0x21	/* F$TLink:	Link trap subroutine package */
# define F_TRANS	0x60	/* F$Trans:	Translate memory address to/from external bus */
# define F_UACCT	0x59	/* F$UAcct:	inform user accounting of process status */
#endif	/* _OSK */
/* 0x68..0x6f, 0x71..0x7f free */
#define I_ATTACH	0x80	/* I$Attach:Attach I/O Device */
#define I_CHGDIR	0x86	/* I$ChgDir:Change Default Directory */
#define I_CLOSE		0x8f	/* I$Close:	Close Path */
#define I_CREATE	0x83	/* I$Create:Create New File */
#define I_DELETE	0x87	/* I$Delete:Delete File */
#define I_DELETX	0x90	/* I$DeletX:Delete from current exec dir */
#define I_DETACH	0x81	/* I$Detach:Detach I/O Device */
#define I_DUP		0x82	/* I$Dup:	Duplicate Path */
#define I_GETSTT	0x8d	/* I$GetStt:Get Path Status */
#define I_MAKDIR	0x85	/* I$MakDir:Make Directory File */
#define I_OPEN		0x84	/* I$Open:	Open Existing File */
#define I_READ		0x89	/* I$Read:	Read Data */
#define I_READLN	0x8b	/* I$ReadLn:Read Line of ASCII Data */
#define I_SEEK		0x88	/* I$Seek:	Change Current Position */
#define I_SETSTT	0x8e	/* I$SetStt:Set Path Status */
#define I_WRITE		0x8a	/* I$Write:	Write Data */
#define I_WRITLN	0x8c	/* I$WritLn:Write Line of ASCII Data */
#if defined(_OS9) && (defined (_COCO_) || defined (_NITROS9))
# if defined (_NITROS9) && defined (_EOU)
#  define I_MODDSC	0x91	/* I$ModDsc:Modify a device descriptor */
# endif
#endif
#ifdef _OSK
# define I_SGETST	0x92	/* I$SGetSt:Getstat using system path number */
#endif
#endif /* _FUNCS_H_ */
