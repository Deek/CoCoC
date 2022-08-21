# Makefile for grep / fgrep / egrep
CFLAGS	= -g -mx32 -fsigned-char \
	-Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type
LFLAGS	= -mx32
IDIR	= $(HOME)/bin
PROGS	= grep egrep fgrep
O2U	= tr '\015' '\012'

all: $(PROGS)

grep: grep.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ grep.c

fgrep: fgrep.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ fgrep.c

egrep: egrep.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ egrep.c

egrep.c: egrep.y
	$(YACC) $(YFLAGS) -o $@ $<

grep.hlp: grep.man GNUmakefile
	$(O2U) < $< | LANG=C nroff -man | LANG=C col -bx > $@

clean:
	-rm $(PROGS) egrep.c

.PHONY: all clean
