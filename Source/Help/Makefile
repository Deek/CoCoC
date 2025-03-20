LFLAGS = -lg -ls
CFLAGS = -s

RFILES = help.r pager.r topics.r
PROG = help

$(PROG): $(RFILES)
   $(CC) $(RFILES) $(LFLAGS) -e=3 -f=$(PROG)

$(RFILES): help.h
