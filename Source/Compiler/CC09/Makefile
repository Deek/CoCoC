#	makefile for 6809 C compiler
CC = cc
RDIR    = RELs
ODIR    = ...../Source/Compiler/CC09
CFLAGS  = -sS -dOS9 -d_OS9 -dPROF -dREGCONTS
LFLAGS  = -M=8 -e=13 -l=/dd/lib/sys.l -l=/dd/lib/clib.l
PROG    = dcc68
FILES   = cmain.r misc.r build.r optim.r stats.r longs.r floats.r\
        inits.r get.r codgen.r local.r lex.r tranexp.r regscont.r\
        declare.r bool.r scale.r #printf.r

$(PROG): $(FILES)
    chd $(RDIR); rlink /dd/lib/cstart.r $(FILES) $(LFLAGS) -n=$(PROG) -o=$(ODIR/)$(PROG)

$(FILES): cj.h Makefile

install: $(PROG)
    attr $(PROG) e pe
    copy -rw=/DD/CMDS $(PROG)

clean:
    :del $(RDIR/)*.r cstr.* ctmp.* $(PROG)

date.list: cj.h cmain.c misc.c build.c optim.c stats.c longs.c floats.c\
    inits.c get.c codgen.c local.c lex.c tranexp.c regscont.c\
    declare.c bool.c printf.c
    pr -p96 -h1 -t2 -r90 -n $? ! spl -jn=$(PROG)
    touch date.list

