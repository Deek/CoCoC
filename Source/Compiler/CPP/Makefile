CC	= c3
BINARY	= c3prep
OBJS	= scan.r dues.r eval.r history.r lex.r macros.r main.r misc.r parse.r

%.r: %.c
	$(CC) -r $<
$(BINARY):	$(OBJS)
	$(CC) -f=$@ $(OBJS)

clean:
	-rm -f $(BINARY) *.r cstr.* ctmp.*

install: $(BINARY)
	cp $(BINARY) $(HOME)/bin
