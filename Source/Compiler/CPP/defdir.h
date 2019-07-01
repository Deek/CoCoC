/*
	@(#)defdir.h	2.2.1	6/15/87
*/
#define DEFNAME "/dd"
#define DEVNAME "/d0"

#if defined(vms)
#	define DEFDIR "Osk$Defs:"
#elif defined(sys5)
#	define DEFDIR "/user/defs"
#else
#	define DEFDIR "/dd/defs"
#endif
