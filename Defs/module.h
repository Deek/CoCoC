#ifndef _MODULE_H_
#define _MODULE_H_
/* OS-9 module header definitions */

#ifndef _ANSI_H_
# include <ansi.h>
#endif

#ifndef _TYPES_H_
# include <types.h>
#endif

/* Module sync bytes */
#define MODSYNC	0x87CD		/* Module header sync bytes */
#define CRCCON	0x800FE3	/* CRC polynomial constant */

/* Module Type values */
#define MT_ANY		0		/* not used -- wildcard in syscalls */
#define MT_PROGRAM	0x10	/* Program module */
#define MT_SUBROUT	0x20	/* Subroutine module */
#define MT_MULTI	0x30	/* Multi-module (reserved) */
#define MT_DATA		0x40	/* Data module */
#define MT_CDBDATA	0x50	/* "Config data block" data module */
#define MT_CSDDATA	MT_CDBDATA
#define MT_TRAPLIB	0xB0	/* (68K+) User trap library */
#define MT_SYSTEM	0xC0	/* System module */
#define MT_FILEMAN	0xD0	/* File manager module */
#define MT_DEVDRVR	0xE0	/* Device driver */
#define MT_DEVDESC	0xF0	/* Device descriptor */
#define MT_MASK		0xF0

/* Module Language values */
#define ML_ANY		0		/* not used -- wildcard in syscalls */
#define ML_OBJECT	1		/* Object (machine) code */
#define ML_ICODE	2		/* Basic09 I-code */
#define ML_PCODE	3		/* Pascal P-code */
#define ML_CCODE	4		/* C I-code (does not exist) */
#define ML_CBLCODE	5		/* COBOL code */
#define ML_FRTNCODE	6		/* FORTRAN code */
#define ML_OBJ6309	7		/* (NitrOS-9) Object code for 6309 only */
#define ML_JAVACODE	7		/* (68K+) Java code (yes, really) */
#define ML_MASK		0x0F

#define mktypelang(type,lang)	(((type) & MT_MASK) | ((lang) & ML_MASK))

/* Module Attribute values */
#define MA_NATIV	0x20	/* (NitrOS-9) Module for 6309 native mode */
#define MA_SUPER	0x20	/* (68K+) Module is for system state */
#define MA_NPROT	0x40	/* (GMX) Module is not write-protected */
#define MA_GHOST	0x40	/* (68K+) Module is "sticky" */
#define MA_REENT	0x80	/* Module is reentrant */

#define MA_MASK		0xF0
#define MR_MASK		0x0F

#define mkattrevs(attr,revs)	(((attr) & MA_MASK) | ((revs) & MR_MASK))

/* Macro definitions for accessing module header fields */
#define MODNAME(mod)	(char*)((char*)(mod) + (mod)->m_name)
#define MODSIZE(mod)	((mod)->m_size)

/* Module header common section -- found in all OS-9 module types */
typedef struct mod_com {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */
} mod_com;
/* Size of header EXCEPT for the parity byte */
#define M_PARITY	(sizeof(mod_com)-1)

/* Executable memory module */
typedef struct mod_exec {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */

	_USHORT	m_exec,			/* offset to execution start */
			m_store;		/* initial storage size */
} mod_exec;

typedef struct mod_driver {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */

	_USHORT	m_exec,			/* offset to function jump table */
			m_ddata;		/* data storage size */
	_BYTE	m_mode;			/* device mode capabilities */
} mod_driver;

/* Device descriptor module */
typedef struct mod_dev {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */

	_USHORT	m_fmname,		/* offset to file manager name */
			m_ddname;		/* offset to device driver name */
	_BYTE	m_mode,			/* device mode capabilities */
			m_control[3],	/* device controller address (24 bit)*/
			m_tabsize;		/* option table size */
} mod_dev;

/* Configuration module */
typedef struct mod_config {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */

	_BYTE	m_ramtop[3],	/* top limit of free ram */
			m_irqno,		/* IRQ polling entry count */
			m_devno;		/* device entries */
	_USHORT	m_startup,		/* offset to startup mod. name */
			m_sysdrive,		/* offset to default drive name */
			m_consol,		/* offset to console terminal name */
			m_boot;			/* offset to bootstrap module name */
	_BYTE	m_wprot,		/* "Write protect flag (?)" */
			m_os9lvl[4],	/* operating system level/version/edition */
			m_feat1,		/* Feature byte #1 */
			m_feat2;		/* Feature byte #2 */
	_USHORT	m_os9rev,		/* offset to OS-9 level/revision string */
			m_install;		/* installation name offset */
	_BYTE	m_spare[4];
							/* CoCo 3 section */
	_BYTE	m_montype,		/* Monitor type */
			m_hrmous,		/* Mouse high-res? */
			m_lrmous,		/* Mouse left/right port */
			m_krstart,		/* Key repeat: start constant */
			m_krdelay;		/* Key repeat: delay constant */
} mod_config;

/* Data module */
typedef struct mod_data {
	_USHORT	m_sync,			/* sync bytes ($87cd) */
			m_size,			/* module size */
			m_name;			/* offset to module name */
	_BYTE	m_tylan,		/* type & language */
			m_attrev,		/* attributes & revision */
			m_parity;		/* header parity */

	_USHORT	m_data,			/* offset to data */
			m_dsize;		/* size of data */
} mod_data;

/* Link to module NAME of type TYPE, with language code LANGUAGE */
mod_com *modlink _OP((const char *name, int type, int language));

/* Load and link to module NAME of type TYPE, with language code LANGUAGE */
mod_com *modload _OP((const char *name, int type, int language));

/* Unlink module pointed at by MOD from the calling process */
int munlink _OP((const mod_com *mod));

/* Get the name of the calling program module (same as argv[0]) */
const char *_prgname _OP((_VOID));

int crc _OP((_VOIDPTR start, ssize_t count, _VOIDPTR accum));

#endif /* _MODULE_H_ */
