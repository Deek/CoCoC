#	makefile for 6809 C compiler
RDIR    =
ODIR    = ...../Source/Compiler/DCC
CFLAGS  = -s
LFLAGS  = -e1 -n=$(PROG) -ls
PROG    = dcc
FILES   = dcc.c
HELPDIR = /DD/SYS/HELP

$(PROG): $(FILES)
    $(CC) $(FILES) $(CFLAGS) $(LFLAGS) -f=$(ODIR/)$(PROG)

$(FILES): dcc.h Makefile

install: $(PROG)
    attr $(PROG) e
    -attr dcc.hlp r pr -pw
    copy -rw=/DD/CMDS $(PROG)
    copy -rw=$(HELPDIR) dcc.hlp

clean:
    -:del $(RDIR/)*.r cstr.* ctmp.* $(PROG)
