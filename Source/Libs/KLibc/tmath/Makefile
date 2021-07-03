RDIR	= RELs
RFILES1	= trig.r hyp.r log.r trnser.r dsqrt.r dinc.r
RFILES2	= dtype.r dmul.r ddiv.r daddsub.r dser.r dcmpr.r

trans.l: $(RFILES1) $(RFILES2)
   -del trans.l
   cd rels;merge $(RFILES1) >-../temp1
   cd rels;merge $(RFILES2) >-../temp2
   merge temp1 temp2 >-trans.l
   -:del temp*

clean:
	-:del temp* trans.l

