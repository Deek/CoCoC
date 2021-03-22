# Makefile for grep / fgrep / egrep
CFLAGS	= -s2
LFLAGS	= -e1
IDIR	= /DD/Cmds
ODIR	= ..../Source/Text/Grep
PROGS	= grep egrep fgrep
HELPDIR	= /DD/Sys/Help

all: $(PROGS)

grep: grep.c
	$(CC) $(CFLAGS) $(LFLAGS) -n=Grep -f=$(ODIR/)$@ grep.c

fgrep: fgrep.c
	$(CC) $(CFLAGS) $(LFLAGS) -n=FGrep -f=$(ODIR/)$@ fgrep.c

egrep: egrep.y
	$(YACC) -o egrep.c egrep.y
	$(CC) $(CFLAGS) $(LFLAGS) -n=EGrep -f=$(ODIR/)$@ egrep.c
	-del egrep.c

install: all
	attr grep e w r pe -pw pr
	attr egrep e w r pe -pw pr
	attr fgrep e w r pe -pw pr
	attr grep.hlp -e w r -pe -pw pr
	copy -rw=$(IDIR) $(PROGS)
	copy -rw=$(HELPDIR) grep.hlp

uninstall:
	-del -x grep
	-del -x egrep
	-del -x fgrep

clean:
	-del $(PROGS) egrep.c

.PHONY: all clean install uninstall

