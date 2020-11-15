CFLAGS	= -g -mx32 -fsigned-char -DUNIX -DDEBUG \
	-Wall -Wno-char-subscripts -Wno-incompatible-pointer-types \
	-Wno-implicit-int -Wno-implicit-function-declaration -Wno-parentheses \
	-Wno-return-type

LDFLAGS = -mx32

BINARY	= dco68
OBJS    = main.o parse.o labels.o inst.o optim.o misc.o ctype.o

$(BINARY):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean install uninstall

clean:
	rm -f $(BINARY) *.o

install: $(BINARY) $(CKEYS)
	install -D -t $(HOME)/bin $(BINARY)

uninstall:
	-rm $(HOME)/bin/$(BINARY)

$(OBJS) : op.h ctype.h GNUmakefile
