RDIR	= rels
RFILES1 = abs.r minmax.r uminmax.r clmul.r cldiv.r cludiv.r claddsub.r clcompare.r
RFILES2 = clbits.r clnegcompl.r clconvert.r clmove.r clincdec.r clshifts.r
RFILES3 = clcommon.r ccmult.r ccmod.r ccdiv.r cshifts.r rpterr.r

math.l: $(RFILES1) $(RFILES2) $(RFILES3)
   -del math.l
   cd rels;merge $(RFILES1) >-../temp1
   cd rels;merge $(RFILES2) >-../temp2
   cd rels;merge $(RFILES3) >-../temp3
   merge temp1 temp2 temp3 >-math.l
   -:del temp*

clean:
	-:del temp* math.l

