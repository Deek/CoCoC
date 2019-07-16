#	makefile for 6809 C compiler
RDIR = rels
ODIR = /h0/cmds
CFLAGS = -ccc09 -dPROF -S

PROG = cc09
FILES = cmain.r misc.r build.r optim.r stats.r longs.r floats.r\
		inits.r get.r codgen.r local.r lex.r tranexp.r regscont.r\
		declare.r bool.r printf.r

$(PROG): $(FILES)
	chd $(RDIR) rlink /h0/lib.new/cstart.r $(FILES)\
		-l=/h0/lib.new/clib.l -l=/dd/lib/sys.l -o=$(PROG) -M=6 -e=13

$(FILES): cj.h
	cc3 $(CFLAGS) -f$(RDIR) $*


date.list: cj.h cmain.c misc.c build.c optim.c stats.c longs.c floats.c\
		inits.c get.c codgen.c local.c lex.c tranexp.c regscont.c\
		declare.c bool.c printf.c
	pr -p96 -h1 -t2 -r90 -n $? ! spl -jn=$(PROG)
	touch date.list

