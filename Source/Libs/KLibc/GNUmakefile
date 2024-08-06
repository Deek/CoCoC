RFILES1 = lib.c/cstuff.l lib.a/main.l
RFILE_F	= fmath/cfloats.l
RFILE_T	= tmath/trans.l
RFILES2	= imath/math.l sys.a/syslib.l

all: libc.a libct.a

before-libc:
	cd lib.c; $(MAKE) -f GNUMakefile $(MAKEFLAGS)
	cd lib.c; $(MAKE) -f GNUMakefile $(MAKEFLAGS) dbg.l
	cd lib.a; $(MAKE) -f GNUMakefile $(MAKEFLAGS)
	cd fmath; $(MAKE) -f GNUMakefile $(MAKEFLAGS)
	cd imath; $(MAKE) -f GNUMakefile $(MAKEFLAGS)
	cd sys.a; $(MAKE) -f GNUMakefile $(MAKEFLAGS)
#	cd sys.a; $(MAKE) -f GNUMakefile $(MAKEFLAGS) cstart.r

before-libct:
	cd tmath; $(MAKE) -f GNUMakefile $(MAKEFLAGS)

clean:
	cd lib.c; make -f GNUMakefile $(MAKEFLAGS) clean
	cd lib.a; make -f GNUMakefile $(MAKEFLAGS) clean
	cd fmath; make -f GNUMakefile $(MAKEFLAGS) clean
	cd imath; make -f GNUMakefile $(MAKEFLAGS) clean
	cd tmath; make -f GNUMakefile $(MAKEFLAGS) clean
	cd sys.a; make -f GNUMakefile $(MAKEFLAGS) clean
	-rm libc.a
	-rm libct.a

libc.a: before-libc $(RFILES1) $(RFILE_F) $(RFILES2)
	-rm libc.a
	lwar --create libc.a $(RFILES1) $(RFILE_F) $(RFILES2)

libct.a: before-libct $(RFILES1) $(RFILE_T) $(RFILES2) before-libct
	-rm libct.a
	lwar --create libct.a $(RFILES1) $(RFILE_T) $(RFILES2)

install: clib.l clibt.l
	copy -rw=$(LIB) clib.l clibt.l

# Package up libraries for testing
cmoc_kreider: libct.a
	rm -rf $@/lib $@/defs
	mkdir $@/lib
	cp libct.a $@/lib
	mkdir $@/defs
	cp -r ../../../Defs/* $@/defs
	cp $(NITROS9DIR)/lib/libnos96809l1.a $@/lib
	tar cvzf $@.tgz $@
		
.PHONY: dummy before-libc before-libct
