# disable built-in rules
MAKEFLAGS+=-rR

AFLAGS= --pragma=index0tonone,condundefzero,undefextern,dollarnotlocal,noforwardrefmax
CFLAGS= -I/dd/defs -O2 -nodefaultlibs --function-stack=0 --os9

# default top level to current dir
TOP?=	$(dir $(realpath $(lastword $(MAKEFILE_LIST))))
# our commands
SED?=	sed
CMOC?=	cmoc
LWASM?=	lwasm
O2U?=	tr '\015' '\012'
#our scripts
DCCMOC	= $(TOP)/dccmoc.sed
RMA2LW	= $(TOP)/rma2lw.sed

# implicit rules to compile with CMOC/lwasm
%.s: %.a $(RMA2LW)
	$(O2U) < $< | $(SED) -f $(RMA2LW) > $@

%.cp: %.c $(DCCMOC)
	$(O2U) < $< | $(SED) -f $(DCCMOC) > $@

%.s: %.cp
	$(CMOC) $(CFLAGS) -S -o $@ $<

%.o: %.s
	$(LWASM) $(AFLAGS) --obj -o $@ $<
