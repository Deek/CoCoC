#ifndef _DIRECT_H_
#define _DIRECT_H_

#ifndef _SYS_TYPES_H_
# include <sys/types.h>
#endif

/* Actual on-disk structure of a directory entry */
struct direct {
	_BYTE	dir_name[29],
			dir_addr[3];
};

/* File descriptor structure */
struct fildes {
	mode_t	fd_att;			/* file attributes: dsewrewr */
	uid_t	fd_own;			/* file owner */
	_BYTE	fd_date[5];		/* last written date/time */
	nlink_t	fd_link;		/* link count */
	off_t	fd_fsize;		/* file size */
	_BYTE	fd_dcr[3];		/* creation date (no time) */
	struct {	/* File data segment list */
		_BYTE	addr[3];	/* LSN of segment start */
		size_t	size;		/* byte size of segment */
	} fdseg[48];
};
typedef struct fildes *Fd;

/* RBF superblock structure */
struct ddsect {
	_BYTE		dd_tot[3],	/* Total number of sectors on media */
				dd_tks;		/* Track size in sectors */
	unsigned	dd_map,		/* Number of bytes in allocation map */
				dd_bit;		/* Disk cluster size (sectors/bit) */
	_BYTE		dd_dir[3];	/* LSN of root directory */
	uid_t		dd_own;		/* Disk owner ID */
	mode_t		dd_att;		/* Disk attributes: dsewrewr */
	unsigned	dd_dsk;		/* Disk serial # */
	_BYTE		dd_fmt;		/* Disk format (density/sides) */
	unsigned	dd_spt,		/* Disk sectors/track */
				dd_res;		/* Reserved */
	_BYTE		dd_bt[3];	/* System bootstrap LSN */
	unsigned	dd_bsz;		/* Size of system bootstrap (bytes) */
	_BYTE		dd_date[5],	/* Disk creation date/time */
				dd_name[32];/* Volume name */
#ifdef _LSN0_EXTENDED_
	_BYTE		dd_opt[32],	/* PD options */
				dd_res2,	/* Reserved */
				dd_sync[4];	/* Media integrity code */
	_DWORD		dd_maplsn;	/* Bitmap starting LSN (valid if !0) */
	unsigned	dd_lsnsize,	/* Logical sector size (0 = 256 bytes) */
				dd_versid;	/* Sector 0 version ID */
	char		dd_res3[148]; /* Reserved */
#endif
};

#endif /* _DIRECT_H_ */
