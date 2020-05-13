# Makefile for make!
RDIR=   RELs
ODIR=   ...../Source/Make

CFLAGS= -dOS9 -s

ROBJS=  $(RDIR/)check.r $(RDIR/)input.r $(RDIR/)macro.r $(RDIR/)main.r \
    $(RDIR/)make.r $(RDIR/)reader.r $(RDIR/)rules.r $(RDIR/)files.r
OBJS=   check.r input.r macro.r main.r make.r reader.r rules.r files.r
SRCS=   check.c input.c macro.c main.c make.c reader.c rules.c files.c

make: $(OBJS)
    cc $(ROBJS) $(CFLAGS) -n=Make -e=19 -f=$(ODIR/)make

$(OBJS): h.h Makefile
listing: $(SRCS) h.h
    cocopr $?
    touch listing

