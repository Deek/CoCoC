OFILES	= prof.o pwent.o getopt.o adump.o defdrive.o popen.o \
 asctime.o ctime.o localtime.o mktime.o isleap.o timevars.o
 
MISSING = omktime.o u2otime.o

DFILES	= dbg.r getsp.r

include ../cmoc.make

cstuff.lib: $(OFILES)
	lwar -c $@ $^

dbg.lib: $(DFILES) ../defs/dbg.h
	lwar -c $@ $^

clean:
	-rm *.o *.s *.cp

$(OFILES): GNUmakefile $(DCCMOC) $(RMA2LW)
