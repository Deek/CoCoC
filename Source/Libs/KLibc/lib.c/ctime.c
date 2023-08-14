#include <time.h>

#undef ctime

#ifdef ASMOPT
#asm
ctime:
 ldd 2,S
 pshs D
 lbsr localtim
 std ,S
 lbsr asctime
 puls X,PC
#endasm
#else
char *
ctime(ticks)
time_t	*ticks;
{
	return (asctime(localtime(ticks)));
}
#endif
