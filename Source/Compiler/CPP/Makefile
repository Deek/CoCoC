CC	= cc
BINS	= cpp dcpp
OBJS	= scan.r dues.r eval.r history.r lex.r macros.r misc.r parse.r
COBJS	= cpp-main.r
DOBJS	= dcpp-main.r
ODIR    = ...../Source/Compiler/CPP
IDIR    = ...../Cmds
LFLAGS  = -e=24 -m=32
CFLAGS  = -s -dOS9
DEPS	= prep.h Makefile

all: $(BINS)

dcpp:	$(DOBJS) $(OBJS)
	$(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(DOBJS) $(OBJS)

cpp:	$(COBJS) $(OBJS)
	$(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(COBJS) $(OBJS)

$(COBJS): main.c $(DEPS)
	$(CC) $(CFLAGS) -r -f=$(RDIR/)$@ main.c

$(DOBJS): main.c $(DEPS)
	$(CC) $(CFLAGS) -DDCC -r -f=$(RDIR/)$@ main.c

$(OBJS): $(DEPS)

clean:
	-:del $(BINS) *.r cstr.* ctmp.*

install: $(BINS)
	copy -rw=$(IDIR) $(BINS)

.PHONY: all clean install
