CFLAGS	= -g -mx32 -fsigned-char -Dunix -DUNIX \
	-Wall -Wno-incompatible-pointer-types -Wno-implicit-int \
	-Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type

LDFLAGS = -mx32

BINARY	= dcpp
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

$(OBJS): prep.h GNUmakefile
