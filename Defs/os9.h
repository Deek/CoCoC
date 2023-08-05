#ifndef _OS9_H_
#define _OS9_H_

#ifndef _ANSI_H_
# include <ansi.h>	// for _OP()
#endif

#ifndef _SYS_MACHTYPE_H_
# include <sys/machtype.h>
#endif

#ifndef _FUNCS_H_
# include <funcs.h>
#endif

#ifndef _SG_CODES_H_
# include <sg_codes.h>
#endif

#ifndef _SYS_REG_H_
# include <sys/reg.h>
#endif

/* Universal OS-9 system call */
int _os9 _OP((int code, REGISTERS *regs));

#endif // _OS9_H_
