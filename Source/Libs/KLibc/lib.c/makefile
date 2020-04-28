CFLAGS = -s -q
RDIR = rels
RFILES = prof.r pwent.r getopt.r adump.r defdrive.r popen.r
DFILES = dbg.r getsp.r

cstuff.l: $(RFILES)
   del cstuff.l
   chd rels;cat $(RFILES) >../cstuff.l

dbg.l: $(DFILES) /dd/defs/dbg.h
   del dbg.l
   cat rels/dbg.r rels/getsp.r >dbg.l
   cp dbg.l /dd/lib/dbg.l
