IDIR	= ...../Cmds
LFLAGS	= -e=1
CFLAGS	= -s

BINARY	= tr
ODIR	= ...../Source/Text/Tr
SRC	= tr.c

tr.hlp: tr.man GNUmakefile
	o2u < $< | LANG=C nroff -man | LANG=C col -bfx > $@

$(BINARY): tr.c Makefile
	$(CC) $(CFLAGS) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(SRC)

$(BINARY):

install: $(BINARY)
	attr $(BINARY) e pe
	copy -rw=$(IDIR) $(BINARY)

clean:
	-:del *.r cstr.* ctmp.* $(BINARY)
