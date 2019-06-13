CFILES = cuts.c cuts_decode.c cuts_encode.c cuts_getopt.c
RFILES = cuts.r cuts_decode.r cuts_encode.r cuts_getopt.r
CFLAGS = -dOS9

cuts: $(RFILES)
  cc $(RFILES) -f=cuts

cuts.r: cuts.c
cuts_decode.r: cuts_decode.c
cuts_encode.r: cuts_encode.c
cuts_getopt.r: cuts_getopt.c
$(RFILES): cuts.h

print.file: (CFILES)
   list $? >/p
   touch print.file