******************************
* Example Make file
* 
CC=cc1              # Color Computer C compiler name
LC=rlink            # Color Computer Linker name

PROG = updn lsit    # Program/file names which this makefile can make

LIB = ../LIB        # Directory where library files are kept
ODIR = /DD/CMDS     # Directory where object code files will be created
RDIR = RELS         # Directory where .r files will be kept


*
* This example will make either program
* and uses defaults for Program.a
* USE: Make lsit or Make updn
*
$(PROG):
  rlink $(RDIR)/$@.r -l=$(LIB)/sys.l -o=$(ODIR)/$*


*
* This example uses no defaults
* and is for updn only.
*

*updn: $(RDIR)/updn.r
*  rlink $(RDIR)/updn.r -l=$(LIB)/sys.l -o=$(ODIR)/updn
*
*$(RDIR)/updn.r: updn.a
*  rma updn.a -o=$(RDIR)/updn.r


