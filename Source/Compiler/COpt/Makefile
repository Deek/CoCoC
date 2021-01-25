CC      = cc
BINARY  = dco68
OBJS    = main.r parse.r labels.r inst.r optim.r match.r misc.r ctype.r
ODIR    = ...../Source/Compiler/COpt
IDIR    = ...../Cmds
LFLAGS  = -e5 -m10
CFLAGS  = -s2 #-dDEBUG

$(BINARY): $(OBJS)
    $(CC) $(LFLAGS) -n=$* -f=$(ODIR/)$@ $?

$(OBJS): op.h Makefile

optim.r: optim.c
optim.c: actions.h ctype.h

parse.r: parse.c
parse.c: ctype.h

clean:
    -:del $(BINARY) *.r cstr.* ctmp.*

install: $(BINARY)
    attr $(BINARY) e pe
    copy -rw=$(IDIR) $(BINARY)
