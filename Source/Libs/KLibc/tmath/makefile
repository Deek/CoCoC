RDIR   = rels
RFILES1 = trig.r hyp.r log.r trnser.r dsqrt.r dinc.r
RFILES2 = dtype.r dmul.r ddiv.r daddsub.r dser.r dcmpr.r

trans.l: $(RFILES1) $(RFILES2)
   del trans.l
   chd rels;cat $(RFILES1) >../temp1
   chd rels;cat $(RFILES2) >../temp2
   cat temp1 temp2 >trans.l
   del temp*
