OFILES1 = \
 abs.o minmax.o uminmax.o clmul.o cldiv.o cludiv.o claddsub.o clcompare.o
OFILES2 = clbits.o clnegcompl.o clconvert.o clmove.o clincdec.o clshifts.o
OFILES3 = clcommon.o ccmult.o ccmod.o ccdiv.o cshifts.o rpterr.o

OFILES = $(OFILES1) $(OFILES2) $(OFILES3)

include ../cmoc.make

math.lib: $(OFILES)
	lwar -c $@ $^

clean:
	-rm math.lib $(OFILES)

$(OFILES): GNUmakefile ../cmoc.make $(DCCMOC) $(RMA2LW)
