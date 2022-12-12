OBJS=	os9.r rbf.r scf.r coco.r
DEFS=	/DD/Defs
TARGET=	sys.l

all: $(TARGET)

$(TARGET): $(OBJS)
	merge $(OBJS) >- sys.l

nopublic.a: os9.d asm2rma.sed
	sed -f asm2rma.sed -e s/://g os9.d >- nopublic.a

os9.r: os9.a os9.p

rbf.r: rbf.a rbf.p nopublic.a

scf.r: scf.a scf.p nopublic.a

coco.r: coco.a coco.p nopublic.a

os9.p: os9.d asm2rma.sed

rbf.p: rbf.d asm2rma.sed

scf.p: scf.d asm2rma.sed

coco.p: coco.d asm2rma.sed

.a.r:;c.asm -o=$@ $<
.d.p:;sed -f asm2rma.sed < $< >- $@

.PHONY: all
.SUFFIXES: .a .d .p
