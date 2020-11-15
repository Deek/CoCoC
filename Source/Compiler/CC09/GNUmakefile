CFLAGS	= -g -mx32 -fsigned-char -DUNIX -DPTREE -DPROF -DREGCONTS \
	-Wall -Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type \
	-DCKEYSFILE=\"$(HOME)/.ckeys\"

LDFLAGS = -mx32

BINARY	= dcc68
CKEYS	= ckeys
OBJS	= bool.o build.o cmain.o codgen.o declare.o floats.o get.o inits.o \
	lex.o local.o longs.o misc.o optim.o prtree.o regscont.o stats.o tb.o \
	tranexp.o

$(BINARY):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean install uninstall

clean:
	rm -f $(BINARY) *.o

install: $(BINARY) $(CKEYS)
	install -D -t $(HOME)/bin $(BINARY)
	chmod 0600 ckeys
	cp -p $(CKEYS) $(HOME)/.ckeys

uninstall:
	-rm $(HOME)/bin/$(BINARY)
	-rm $(HOME)/.ckeys

$(OBJS) : cj.h GNUmakefile
