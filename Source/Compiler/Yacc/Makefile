CFLAGS=-d_OS9 -s
IDIR=/DD/Cmds
ODIR=....../Source/Compiler/Yacc
LDIR=/DD/Lib

yacc: y1.r y2.r y3.r y4.r
    :$(CC) -f=$(ODIR/)yacc -n=Yacc y?.r

y1.r y2.r y3.r y4.r: dextern.h files.h Makefile

install:
    :attr $(ODIR/)yacc e pe
    :copy -rw=$(IDIR) yacc
    :copy -rw=$(LDIR) yaccpar

clean :
    :del *.r
