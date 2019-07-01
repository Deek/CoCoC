CC	= cc
BINARY	= cpp
OBJS	= scan.r dues.r eval.r history.r lex.r macros.r main.r misc.r parse.r
ODIR    = ...../Source/Compiler/CPP
IDIR    = ...../Cmds
LFLAGS  = -e=24 -m=32
CFLAGS  = -s -dOS9

$(BINARY):	$(OBJS)
	$(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(OBJS)

$(OBJS): prep.h

clean:
	-:del $(BINARY) *.r cstr.* ctmp.*

install: $(BINARY)
	copy -rw=$(IDIR) $(BINARY)
