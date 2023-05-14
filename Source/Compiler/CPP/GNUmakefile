-include local.make

ARCH	?= -m32

CFLAGS	= -g $(ARCH) -fsigned-char -Dunix -DUNIX -DDCC -DDEBUG \
	-Wall -Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type

LDFLAGS = $(ARCH) -g

BINARY	= dcpp
DEPS	= prep.h GNUmakefile $(wildcard local.make)

OBJS	= scan.o dues.o eval.o history.o lex.o macros.o main.o misc.o parse.o

$(BINARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean install uninstall

clean:
	-rm -f $(BINARY) *.o

install: $(BINARY)
	install -D -t $(HOME)/bin $(BINARY)

uninstall:	
	-rm $(HOME)/bin/$(BINARY)

$(OBJS): $(DEPS)
