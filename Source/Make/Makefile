# Makefile for make!
RDIR = RELS

CFLAGS= -dOS9

ROBJS = rels/check.r rels/input.r rels/macro.r rels/main.r rels/make.r \
       rels/reader.r rels/rules.r rels/files.r
OBJS   =       check.r input.r macro.r main.r make.r reader.r rules.r files.r
SRCS   =       check.c input.c macro.c main.c make.c reader.c rules.c files.c

make:            $(OBJS)
        cc $(ROBJS) $(CFLAGS) -f=make

$(OBJS):       h.h

listing: $(SRCS) h.h
      cocopr $?
      touch listing