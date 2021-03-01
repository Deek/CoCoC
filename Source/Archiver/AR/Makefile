LZFILES = lz1.r delz1.r lz1glob.r lz1init.r
ARFILES = ar.r arsup.r dir.r

/dd/cmds/ar: $(LZFILES) $(ARFILES)
 $(CC) -m=2k -f=ar $(ARFILES) $(LZFILES)

$(LZFILES): lz1.h

$(ARFILES): ar.h
