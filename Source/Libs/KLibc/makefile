-b
RFILES1 = lib.c/cstuff.l lib.a/main.l
RFILE_F  = fmath/cfloats.l
RFILE_T  = tmath/trans.l
RFILES2 = imath/math.l sys.a/syslib.l

dummy: dummy1
    chd lib.c;make -s
    chd lib.c;make -s dbg.l
    chd lib.a;make -s
    chd fmath;make -s
    chd tmath;make -s
    chd imath;make -s
    chd sys.a;make -s
    chd sys.a;make -s cstart.r
    make -s clib.l
    make -s clibt.l

clib.l: $(RFILES1) $(RFILE_F) $(RFILES2)
    del clib.l
    cat $(RFILES1) $(RFILE_F) $(RFILES2) >clib.l
    cp clib.l /dd/lib

clibt.l: $(RFILES1) $(RFILE_T) $(RFILES2)
    del clibt.l
    cat $(RFILES1) $(RFILE_T) $(RFILES2) >clibt.l
    cp clibt.l /dd/lib
