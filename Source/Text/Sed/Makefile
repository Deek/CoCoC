IDIR=   ...../Cmds
LFLAGS= -e=1
CFLAGS= -s

BINARY= sed
ODIR=   ...../Source/Text/Sed
OBJS=   sed0.r sed1.r

$(BINARY): sed0.r sed1.r
    :$(CC) $(LFLAGS) -n=$@ -f=$(ODIR/)$@ $(OBJS)

$(OBJS): sed.h Makefile

install: $(BINARY)
    attr $(BINARY) e pe
    copy -rw=$(IDIR) $(BINARY)

clean:
    -:del *.r cstr.* ctmp.* $(BINARY)
