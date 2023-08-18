RFILES1 = lib.c/cstuff.lib lib.a/main.lib
RFILE_F	= fmath/cfloats.lib
RFILE_T	= tmath/trans.lib
RFILES2	= imath/math.lib sys.a/syslib.lib

all: libc.a libct.a

before-libc:
	cd lib.c; $(MAKE) $(MAKEFLAGS)
	cd lib.c; $(MAKE) $(MAKEFLAGS) dbg.l
	cd lib.a; $(MAKE) $(MAKEFLAGS)
	cd fmath; $(MAKE) $(MAKEFLAGS)
	cd imath; $(MAKE) $(MAKEFLAGS)
	cd sys.a; $(MAKE) $(MAKEFLAGS)
#	cd sys.a; $(MAKE) $(MAKEFLAGS) cstart.r

before-libct:
	cd tmath; $(MAKE) $(MAKEFLAGS)

clean:
	cd lib.c; make $(MAKEFLAGS) clean
	cd lib.a; make $(MAKEFLAGS) clean
	cd fmath; make $(MAKEFLAGS) clean
	cd imath; make $(MAKEFLAGS) clean
	cd tmath; make $(MAKEFLAGS) clean
	cd sys.a; make $(MAKEFLAGS) clean
	-del libc.a
	-del libct.a

libc.a: before-libc $(RFILES1) $(RFILE_F) $(RFILES2)
	-del libc.a
	lwar --merge libc.a $(RFILES1) $(RFILE_F) $(RFILES2)

libct.a: before-libct $(RFILES1) $(RFILE_T) $(RFILES2) before-libct
	-del libct.a
	lwar --merge libct.a $(RFILES1) $(RFILE_T) $(RFILES2)

install: clib.l clibt.l
	copy -rw=$(LIB) clib.l clibt.l

.PHONY: dummy before-libc before-libct
