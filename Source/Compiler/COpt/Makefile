CC      = cc
BINARY  = t.opt
OBJS    = main.r parse.r labels.r inst.r optim.r misc.r ctype.r
ODIR    = ...../Source/Compiler/COpt
IDIR    = ...../Cmds
LFLAGS  = -e=3 -m=2k
CFLAGS  = -s -dDEBUG

$(BINARY): $(OBJS)
    $(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(OBJS)

$(OBJS): op.h Makefile

optim.r: optim.c
optim.c: actions.h ctype.h

parse.r: parse.c
parse.c: ctype.h

clean:
    -:del $(BINARY) *.r cstr.* ctmp.*

install: $(BINARY)
    copy -rw=$(IDIR) $(BINARY)
