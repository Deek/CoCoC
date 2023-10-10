OFILES	= prof.o pwent.o getopt.o adump.o defdrive.o popen.o \
 asctime.o ctime.o systime.o time.o localtime.o mktime.o isleap.o timevars.o

DFILES	= dbg.o getsp.o

all: cstuff.lib dbg.lib

include ../cmoc.make

CFLAGS += -I../defs

cstuff.lib: $(OFILES)
	lwar -c $@ $^

dbg.lib: $(DFILES)
	lwar -c $@ $^

clean:
	-rm *.o *.s *.cp

$(OFILES): GNUmakefile $(DCCMOC) $(RMA2LW)
