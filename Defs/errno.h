#ifndef _ERRNO_H_
#define _ERRNO_H_

extern int errno;

/*
 * System call error numbers
 *
 * After any error occurs, the errno variable may be set. Often a function
 * that fails will return 0 or -1, and set the errno global variable
 * to one of these error codes.
 */
#define E_PTHFUL	0xc8 /* Path Table full */
#define E_BPNUM		0xc9 /* Bad Path Number */
#define E_POLL		0xca /* Polling Table Full */
#define E_BMODE		0xcb /* Bad Mode */
#define E_DEVOVF	0xcc /* Device Table Overflow */
#define E_BMID		0xcd /* Bad Module ID */
#define E_DIRFUL	0xce /* Module Directory Full */
#define E_MEMFUL	0xcf /* Process Memory Full */
#define E_UNKSVC	0xd0 /* Unknown Service Code */
#define E_MODBSY	0xd1 /* Module Busy */
#define E_BPADDR	0xd2 /* Bad Page Address */
#define E_EOF		0xd3 /* End of File */
#define E_NES		0xd5 /* Non-Existing Segment */
#define E_FNA		0xd6 /* File Not Accesible */
#define E_BPNAM		0xd7 /* Bad Path Name */
#define E_PNNF		0xd8 /* Path Name Not Found */
#define E_SLF		0xd9 /* Segment List Full */
#define E_CEF		0xda /* Creating Existing File */
#define E_IBA		0xdb /* Illegal Block Address */
#define E_MNF		0xdd /* Module Not Found */
#define E_DELSP		0xdf /* Deleting Stack Pointer memory */
#define E_IPRCID	0xe0 /* Illegal Process ID */
#define E_NOCHLD	0xe2 /* No Children */
#define E_ISWI		0xe3 /* Illegal SWI code */
#define E_PRCABT	0xe4 /* Process Aborted */
#define E_PRCFUL	0xe5 /* Process Table Full */
#define E_IFORKP	0xe6 /* Illegal Fork Parameter */
#define E_KWNMOD	0xe7 /* Known Module */
#define E_BMCRC		0xe8 /* Bad Module CRC */
#define E_USIGP		0xe9 /* Unprocessed Signal Pending */
#define E_NEMOD		0xea /* Non Existing Module */
#define E_BNAM		0xeb /* Bad Name */
#define E_NORAM		0xed /* No Ram Available */
#define E_BPRCID	0xee /* Bad Process ID */
#define E_NOTASK	0xef /* No available Task number */
#define E_UNIT		0xf0 /* Illegal Unit (drive) */
#define E_SECT		0xf1 /* Bad SECTor number */
#define E_WP		0xf2 /* Write Protect */
#define E_CRC		0xf3 /* Bad Check Sum */
#define E_READ		0xf4 /* Read Error */
#define E_WRITE		0xf5 /* Write Error */
#define E_NOTRDY	0xf6 /* Device Not Ready */
#define E_SEEK		0xf7 /* Seek Error */
#define E_FULL		0xf8 /* Media Full */
#define E_BTYP		0xf9 /* Bad Type (incompatable) media */
#define E_DEVBSY	0xfa /* Device Busy */
#define E_DIDC		0xfb /* Disk ID Change */
#define E_LOCK		0xfc /* Record is busy (locked out) */
#define E_SHARE		0xfd /* Non-sharable file busy */
#define E_DEADLK	0xfe /* I/O Deadlock error */

/* errors from arithmetic routines */

#define EFPOVR	40	/* Floating point overflow */
#define EDIVERR	41	/* Divide by zero error */
#define EINTERR	42	/* Overflow on conversion to long int */
#define EFPUND	43	/* Floating point underflow (does not abort) */
#define EILLARG	44	/* illegal function argument, e.g. sqrt(-1) */

/*
 * These are some C/Unix-style symbolic names for OS-9 error codes. Some of
 * these are canonical from examinations of OS9/68000 SDK headers -- others
 * are based on the codes returned by OS-9 system calls in various situations.
 */
#define EACCES	E_FNA		/* "Permission denied" */
#define EAGAIN	E_DEVBSY	/* "Resource temporarily unavailable" */
#define EBADF	E_BPNUM		/* "Bad file descriptor" */
#define EBUSY	E_DEVBSY	/* "Device or resource busy" */
#define ECHILD	E_NOCHLD	/* "No child process" */
#define EDEADLK	E_DEADLK	/* "Resource deadlock avoided" */
#define EDOM	EILLARG		/* "Domain error" */
#define EEXIST	E_CEF		/* "File exists" */
#define EFAULT	E_BPADDR	/* "Bad address" */
#define EIO		E_READ		/* "I/O error" */
#define ENOENT	E_PNNF		/* "No such file or directory" */
#define ENOEXEC	E_BMID		/* "Exec format error" */
#define ENOMEDIUM E_SEEK	/* "No medium found" */
#define ENOMEM	E_MEMFUL	/* "Not enough space/cannot allocate memory" */
#define ENOSPC	E_FULL		/* "No space left on device" */
#define ENOSYS	E_UNKSVC	/* "Function not implemented" */
#define ENOTSUP	E_UNKSVC	/* "Operation not supported" */
#define EPERM	E_FNA		/* "Permission denied" */
#define EPIPE	E_EOF		/* "Broken pipe" */
#define ESPIPE	E_SEEK		/* "Invalid seek" */
#define EROFS	E_WP		/* "Read-only filesystem" */
#define ESRCH	E_IPRCID	/* "No such process" */
#define ESTALE	E_DIDC		/* "Stale file handle" */

#endif /* _ERRNO_H_ */
