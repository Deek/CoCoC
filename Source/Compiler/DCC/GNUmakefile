#	makefile for 6809 C compiler
CFLAGS	= -g -mx32 -fsigned-char
LFLAGS	= -mx32
PROG	= dcc
FILES	= dcc.c

$(PROG): $(FILES)
	$(CC) $< $(CFLAGS) $(LFLAGS) -o $(PROG)

$(FILES): dcc.h Makefile

install: $(PROG)
	install -D -t $(HOME)/bin $(PROG)

clean:
	-rm *.o cstr.* ctmp.* $(PROG)
