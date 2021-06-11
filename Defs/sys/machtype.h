#ifndef _SYS_MACHTYPE_H_
#define _SYS_MACHTYPE_H_
/* Machine type information for an NTSC CoCo 3 with a 6809 CPU */
#ifndef _OS9
# define _OS9		1
#endif

#define _NITROS9	1
#define _OS9LEVEL	2
#define _OS9VERSION	30301L

#ifndef _mc6809_
# define _mc6809_	1
# define _MC6809_	1
#endif

#ifndef _COCO_
# define _COCO_		3
#endif

#define _DIST		"NitrOS-9"
#define _DISTVER	"3.3.1"

/* default ticks per second on CoCo 3 NTSC */
#define HZ			60

#endif /* _SYS_MACHTYPE_H_ */
