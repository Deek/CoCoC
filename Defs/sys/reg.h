#ifndef _SYS_REG_H_
#define _SYS_REG_H_

typedef union registers {
	struct {
		u_char	rg_cc,	// condition codes
				rg_a,	// accumulator A
				rg_b,	// accumulator B
				rg_dp;	// direct page register
		u_short	rg_x,	// index register X
				rg_y,	// index register Y
				rg_u;	// combo register U
	} regsab;
	struct {
		u_char	__rd1;
		u_short	rg_d;	// int register D
	} regsd;
	struct {
		u_char	r3_cc,	// same as 6809
				r3_a,
				r3_b,
				r3_e,	// 6309 accumulator E
				r3_f,	// 6309 accumulator F
				r3_dp;	// 6309 DP register
		u_short	r3_x,	// 6309 index X
				r3_y,	// 6309 index Y
				r3_u;	// 6309 combo reg U
	} regsef;
	struct {
		u_char	__rd3[3];
		u_short	r3_w;	// 6309 int register W
	} regsdw;
} REGISTERS;

#endif // _SYS_REG_H_
