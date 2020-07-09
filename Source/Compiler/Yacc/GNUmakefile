CFLAGS=-O -g -m32 -DWORD32
head: yacc
yacc: y1.o y2.o y3.o y4.o
	$(CC) $(CFLAGS) -o yacc y?.o
y1.o y2.o y3.o y4.o: dextern.h files.h
install:
	install -s yacc $(DESTDIR)/usr/bin
	install -c yaccpar $(DESTDIR)/usr/lib
clean :
	rm -f *.o
