CFLAGS	= -s
RDIR	= rels
RFILES	= prof.r pwent.r getopt.r adump.r defdrive.r popen.r
DFILES	= dbg.r getsp.r

cstuff.l: $(RFILES)
	-del cstuff.l
	cd rels;merge $(RFILES) >-../cstuff.l

dbg.l: $(DFILES) /dd/defs/dbg.h
	-del dbg.l
	merge rels/dbg.r rels/getsp.r >-dbg.l
#	copy -r dbg.l /dd/lib/dbg.l

clean:
	-del cstuff.l dbg.l RELs/*.r
