RDIR   = rels
RFILES1 = abs.r minmax.r uminmax.r clmul.r cldiv.r claddsub.r clcompare.r
RFILES2 = clbits.r clnegcompl.r clconvert.r clmove.r clincdec.r clshifts.r
RFILES3 = clcommon.r ccmult.r ccmod.r ccdiv.r cshifts.r rpterr.r

math.l: $(RFILES1) $(RFILES2) $(RFILES3)
   del math.l
   chd rels;cat $(RFILES1) >../temp1
   chd rels;cat $(RFILES2) >../temp2
   chd rels;cat $(RFILES3) >../temp3
   cat temp1 temp2 temp3 >math.l
   del temp*
