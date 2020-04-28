RDIR    = rels
RFILES1 = abort.r signal.r chown.r chmod.r stat.r access.r create.r
RFILES2 = open.r read.r write.r lseek.r misc.r mod.r dir.r mem.r
RFILES3 = time.r process.r id.r intercept.r syscall.r syscommon.r
RFILES4 = cfinish.r profdummy.r tidyup.r

syslib.l: $(RFILES1) $(RFILES2) $(RFILES3) $(RFILES4)
   del syslib.l
   chd rels;cat $(RFILES1) >../temp1
   chd rels;cat $(RFILES2) >../temp2
   chd rels;cat $(RFILES3) >../temp3
   chd rels;cat $(RFILES4) >../temp4
   cat temp1 temp2 temp3 temp4 >syslib.l
   del temp*

cstart.r: cstart.a
   rma -o=rels/cstart.r cstart.a
   cp rels/cstart.r /dd/lib

root.r: root.a
   rma -o=rels/root.r root.a
   cp rels/root.r /dd/lib
