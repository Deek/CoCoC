########################################################
# Makefile for CGFX library version 7
# by Mike Sweet 9/11/90
#
TEMP=/r0
.SUFFIXES: .r .a .c
.a.r:
    c.asm -o=$@ $<

.c.r:
    cc -Of=$(TEMP/)tmp.o $<
    c.asm -o=$@ $(TEMP/)tmp.o
    -del $(TEMP/)tmp.o

OBJS = cbuffer.r polyline.r polyfill.r polyfunc.r \
       dialog.r mousekey.r settype.r sinecosine.r\
       button.r tandy.r object.r mvmenu.r mvmenuxy.r mvfname.r \
       getstr.r fname.r menu.r menuxy.r shadow.r text.r getput.r play.r \
       window.r config.r font.r radio.r draw.r status.r mouse.r \
       alarm.r mv_window.r keysense.r sound.r crw.r mousexy.r \
       getenv.r movemem.r drawfunc.r nobuffer.r

cgfx: $(OBJS) malloc.r
    -del /dd/lib/cgfx.l
    merge $(OBJS) >/dd/lib/cgfx.l
