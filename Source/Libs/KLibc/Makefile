RFILES1 = lib.c/cstuff.l lib.a/main.l
RFILE_F	= fmath/cfloats.l
RFILE_T	= tmath/trans.l
RFILES2	= imath/math.l sys.a/syslib.l
MAKEFLAGS	= CC=dcc RMA=c.asm

dummy: #dummy1
	chd lib.c;make $(MAKEFLAGS)
	chd lib.c;make $(MAKEFLAGS) dbg.l
	chd lib.a;make $(MAKEFLAGS)
	chd fmath;make $(MAKEFLAGS)
	chd tmath;make $(MAKEFLAGS)
	chd imath;make $(MAKEFLAGS)
	chd sys.a;make $(MAKEFLAGS)
	chd sys.a;make $(MAKEFLAGS) cstart.r
	make $(MAKEFLAGS) clib.l
	make $(MAKEFLAGS) clibt.l

clean:
	cd lib.c; make $(MAKEFLAGS) clean
	cd lib.a; make $(MAKEFLAGS) clean
	cd fmath; make $(MAKEFLAGS) clean
	cd imath; make $(MAKEFLAGS) clean
	cd tmath; make $(MAKEFLAGS) clean
	cd sys.a; make $(MAKEFLAGS) clean
	-del clib.l
	-del clibt.l

clib.l: $(RFILES1) $(RFILE_F) $(RFILES2)
	-del clib.l
	merge $(RFILES1) $(RFILE_F) $(RFILES2) >clib.l
#	cp clib.l /dd/m6809/lib

clibt.l: $(RFILES1) $(RFILE_T) $(RFILES2)
	-del clibt.l
	merge $(RFILES1) $(RFILE_T) $(RFILES2) >clibt.l
#	cp clibt.l /dd/m6809/lib

install: clib.l clibt.l
	copy -rw=$(LIB) clib.l clibt.l
