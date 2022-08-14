#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#ifndef _ANSI_H_
# include <ansi.h>
#endif

#ifndef _CLOCK_T_
# define _CLOCK_T_
typedef unsigned clock_t;
#endif

/* OS-9 Signals */
#define SIGKILL		0	/* sytem abort (cannot be caught or ignored) */
#define SIGWAKE		1	/* wake up */
#define SIGQUIT		2	/* keyboard abort */
#define SIGINT		3	/* keyboard interrupt */
#define SIGHUP		4	/* hang up */
#define SIGALRM		5	/* alarm signal */
#define SIGWINCH	4	/* window change */

/* Special addresses */
#define SIG_DFL		0	/* default action */
#define SIG_IGN		1	/* ignore */

#define intercept intercep

int intercept _OP((_VOID (*func)(int)));
int kill _OP((int pid, int signal));
int pause _OP((_VOID));
int signal _OP((int sig, _VOID (*sigfunc)()));

clock_t sleep _OP((unsigned seconds));
clock_t tsleep _OP((clock_t ticks));

#endif /* _SIGNAL_H_ */
