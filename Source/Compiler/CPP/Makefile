BINS	= cpp dcpp
OBJS	= scan.r dues.r eval.r history.r lex.r macros.r parse.r
COBJS	= cpp-main.r cpp-misc.r
DOBJS	= dcpp-main.r dcpp-misc.r
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

cpp-main.r: main.c $(DEPS)
	$(CC) $(CFLAGS) -r -f=$(RDIR/)$@ main.c

cpp-misc.r: misc.c $(DEPS)
	$(CC) $(CFLAGS) -r -f=$(RDIR/)$@ misc.c

dcpp-main.r: main.c $(DEPS)
	$(CC) $(CFLAGS) -DDCC -r -f=$(RDIR/)$@ main.c

dcpp-misc.r: misc.c $(DEPS)
	$(CC) $(CFLAGS) -DDCC -r -f=$(RDIR/)$@ misc.c

$(OBJS): $(DEPS)

clean:
	-:del $(BINS) *.r cstr.* ctmp.*

install: $(BINS)
	copy -rw=$(IDIR) $(BINS)

.PHONY: all clean install
