DCCDIR	= $(HOME)/.dcc
CFLAGS	= -g -mx32 -fsigned-char -DUNIX -DDEBUG -DCONFDIR=\"$(DCCDIR)\" \
	-Wall -Wno-char-subscripts -Wno-incompatible-pointer-types \
	-Wno-implicit-int -Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type

LDFLAGS = -mx32

BINARY	= dco68
OBJS    = main.o parse.o labels.o inst.o optim.o match.o misc.o ctype.o
CONFS=	level1.patterns level2.patterns

$(BINARY):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean install uninstall

clean:
	rm -f $(BINARY) *.o

install: $(BINARY) $(CKEYS)
	install -D -t $(HOME)/bin $(BINARY)
	install -D -m0600 -t $(DCCDIR) $(CONFS)

uninstall:
	-rm $(HOME)/bin/$(BINARY)
	-rm $(foreach file,$(CONFS),$(DCCDIR)/$(file))
	-rmdir $(DCCDIR)

$(OBJS) : op.h ctype.h actions.h GNUmakefile
