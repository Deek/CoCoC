# Makefile for grep / fgrep / egrep
CFLAGS	= -g -mx32 -fsigned-char \
	-Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type
LFLAGS	= -mx32
IDIR	= $(HOME)/bin
PROGS	= grep egrep fgrep

all: $(PROGS)

grep: grep.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ grep.c

fgrep: fgrep.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ fgrep.c

egrep: egrep.y
	$(YACC) -o egrep.c egrep.y
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ egrep.c
	-rm egrep.c

clean:
	-del $(PROGS) egrep.c

.PHONY: all clean
