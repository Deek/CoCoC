CC	= cc
BINARY	= cpp
OBJS	= scan.r dues.r eval.r history.r lex.r macros.r main.r misc.r parse.r
ODIR    = ...../Source/Compiler/CPP
IDIR    = ...../Cmds
LFLAGS  = -m=38 -e=24
CFLAGS  = -s

$(BINARY):	$(OBJS)
	$(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(OBJS)

$(OBJS): prep.h

clean:
	-:del $(BINARY) *.r cstr.* ctmp.*

install: $(BINARY)
	copy -rw=$(IDIR) $(BINARY)
