LFLAGS = -l=/dd/lib/cgfx.l -l=/dd/lib/sys.l
CFLAGS = -s

RFILES = help.r pager.r topics.r
PROG = help

$(PROG): $(RFILES)
   cc $(RFILES) $(LFLAGS) -f=$(PROG)

$(RFILES): help.h