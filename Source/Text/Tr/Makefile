IDIR=   ...../Cmds
LFLAGS= -e=1
CFLAGS= -s

BINARY= tr
ODIR=   ...../Source/Text/Tr
SRC=   tr.c

$(BINARY): tr.c Makefile
    $(CC) $(CFLAGS) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(SRC)

$(BINARY):

install: $(BINARY)
    attr $(BINARY) e pe
    copy -rw=$(IDIR) $(BINARY)

clean:
    -:del *.r cstr.* ctmp.* $(BINARY)
