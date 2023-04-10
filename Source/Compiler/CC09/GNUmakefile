-include local.make

DCCDIR	?= $(HOME)/.dcc
ARCH	?= -m32

CFLAGS	= -g $(ARCH) -fsigned-char -DFUNCNAME -DUNIX -DPTREE -DPROF -DREGCONTS \
	-Wall -Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type -Wno-char-subscripts \
	-DCKEYSFILE=\"$(DCCDIR)/ckeys\"

LDFLAGS = $(ARCH) -g -lm

BINARY	= dcc68
CKEYS	= ckeys
# This is a nifty hack, 'wildcard' expands to files that exist...
DEPS	= cj.h GNUmakefile $(wildcard local.make)
OBJS	= bool.o build.o cmain.o codgen.o declare.o floats.o get.o inits.o \
	lex.o local.o longs.o misc.o optim.o prtree.o regscont.o stats.o tb.o \
	tranexp.o fltconv.o \
	scale.c

$(BINARY):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) 


clean:
	rm -f $(BINARY) *.o

install: $(BINARY) $(CKEYS)
	install -D -t $(HOME)/bin $(BINARY)
	chmod 0600 ckeys
	install -D -t $(DCCDIR) ckeys

uninstall:
	-rm $(HOME)/bin/$(BINARY)
	-rm $(DCCDIR)/ckeys

$(OBJS) : $(DEPS)

.PHONY: clean install uninstall
