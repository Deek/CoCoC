RDIR	= rels
RFILES1 = abort.r signal.r chown.r chmod.r stat.r access.r create.r
RFILES2 = open.r read.r write.r lseek.r misc.r mod.r dir.r brk.r ibrk.r sbrk.r mem.r
RFILES3 = time.r process.r id.r intercept.r syscall.r syscommon.r
RFILES4 = cfinish.r profdummy.r tidyup.r

syslib.l: $(RFILES1) $(RFILES2) $(RFILES3) $(RFILES4)
	-del syslib.l
	chd rels;merge $(RFILES1) >-../temp1
	chd rels;merge $(RFILES2) >-../temp2
	chd rels;merge $(RFILES3) >-../temp3
	chd rels;merge $(RFILES4) >-../temp4
	merge temp1 temp2 temp3 temp4 >-syslib.l
	-:del temp*

cstart.r: cstart.a
	$(RMA) -o=rels/cstart.r cstart.a
#	copy -rw=/DD/Lib rels/cstart.r

root.r: root.a
	$(RMA) -o=rels/root.r root.a
#	copy -rw=/DD/Lib rels/root.r

clean:
	-:del temp* rels/*.r syslib.l
