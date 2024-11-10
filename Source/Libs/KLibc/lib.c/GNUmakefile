OFILES	= prof.o pwent.o getopt.o adump.o defdrive.o popen.o \
 asctime.o ctime.o localtime.o mktime.o isleap.o timevars.o

MISSING = omktime.o u2otime.o

DFILES	= dbg.o getsp.o

include ../cmoc.make

cstuff.l: $(OFILES)
	lwar -c $@ $^

dbg.l: $(DFILES) ../defs/dbg.h
	lwar -c $@ $^

clean:
	-rm *.o *.s *.cp *.l

$(OFILES): GNUmakefile $(DCCMOC) $(RMA2LW)
