#ifndef _OS9_H_
#define _OS9_H_

#ifndef _ANSI_H_
# include <ansi.h>		/* for _OP() */
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

struct registers {
	char		rg_cc,	/* condition codes */
				rg_a,	/* 6809 accumulator A */
				rg_b,	/* 6809 accumulator B */
				rg_dp;	/* 6809 direct page register */
	unsigned	rg_x,	/* 6809 index register X */
				rg_y,	/* 6809 index register Y */
				rg_u;	/* 6809 combo register U */
};

/* Universal OS-9 system call */
int _os9 _OP((int code, struct registers *regs));

#endif /* _OS9_H_ */
