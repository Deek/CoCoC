#ifndef _CGLOB_H_
#define _CGLOB_H_

extern double _flacc;	/* floating point & longs accumulator */

extern char *memend;	/* high bound of requested memory from OS-9 */
extern char *_spare;	/* free memory at end of current sbrk() page */
extern char *_mtop;		/* memory top (lowest stack address) */
extern char *_stbot;	/* worst stack pointer so far */

extern int errno;		/* global error holder */

#endif /* _CGLOB_H_ */
