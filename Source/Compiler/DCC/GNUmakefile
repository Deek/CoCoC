#	makefile for 6809 C compiler
CFLAGS	= -g -mx32 -fsigned-char \
	-Wno-implicit-int -Wno-implicit-function-declaration \
	-Wno-incompatible-pointer-types
LFLAGS	= -mx32
PROG	= dcc
FILES	= dcc.c

$(PROG): $(FILES) dcc.h GNUmakefile
	$(CC) $< $(CFLAGS) $(LFLAGS) -o $(PROG)

$(FILES): dcc.h GNUmakefile

install: $(PROG)
	install -D -t $(HOME)/bin $(PROG)

clean:
	-rm *.o cstr.* ctmp.* $(PROG)
