CFLAGS = -g -m32

all: lex

lex: lmain.o y.tab.o sub1.o sub2.o header.o
	cc $(CFLAGS) lmain.o y.tab.o sub1.o sub2.o header.o -o lex

smallex:
	cc $(CFLAGS) -DSMALL lmain.c y.tab.c sub1.c sub2.c header.c -o smallex

y.tab.c: parser.y
	yacc parser.y
lmain.o:lmain.c ldefs.c once.c
	cc -c $(CFLAGS) lmain.c
sub1.o: sub1.c ldefs.c
	cc -c $(CFLAGS) sub1.c
sub2.o: sub2.c ldefs.c
	cc -c $(CFLAGS) sub2.c
header.o: header.c ldefs.c
	cc -c $(CFLAGS) header.c

install: all
	install -s lex $(DESTDIR)/usr/local/bin
	install -c ncform $(DESTDIR)/usr/local/share/lex

clean:
	rm -f *.o lex y.tab.c
