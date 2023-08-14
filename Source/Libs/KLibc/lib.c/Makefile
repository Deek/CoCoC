CFLAGS	= -s -DASMOPT
RDIR	= rels
RFILES	= prof.r pwent.r getopt.r adump.r defdrive.r popen.r \
 asctime.r ctime.r systime.r time.c localtime.r mktime.r isleap.r timevars.r
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

.c.r:
	-del $(RDIR/)asm.tmp
	$(CC) $(CFLAGS) -Of=$(RDIR/)asm.tmp $<
	$(RMA) $(RFLAGS) -o=$(RDIR/)$@ $(RDIR/)asm.tmp
	-del $(RDIR/)asm.tmp
