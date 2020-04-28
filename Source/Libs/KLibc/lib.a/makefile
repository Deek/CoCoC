CFLAGS  = -s -q
RDIR    = rels
RFILES1 = swab.r pwcryp.r bsearch.r dirutil.r errmsg.r prgname.r \
          datmods.r utime.r devtyp.r skip.r sets.r pflinit.r
RFILES2 = fopen.r puts.r gets.r mktemp.r fread.r fwrite.r printf.r
RFILES3 = setbuf.r fseek.r putc.r ftelldummy.r getc.r flshdummy.r setbase.r \
          findstr.r pfldummy.r pffdummy.r htoi.r htol.r ltoa.r xtoa.r
RFILES4 = system.r reverse.r strings.r strcmp.r strlen.r strncpy.r strncat.r \
          strncmp.r strhcpy.r strtok.r strpbrk.r strspn.r index.r patmatch.r
RFILES5 = stringsu.r strucmp.r strnucmp.r case.r strclr.r memccpy.r memchr.r \
          memcmp.r memcpy.r memset.r atol.r atoi.r iob_data.r chcodes.r
RFILES6 = sleep.r setjmp.r strass.r realloc.r calloc.r memory.r rand.r
RFILES7 = atof.r scale.r gs1.r gs2.r ss1.r ss2.r ss3.r

main.l: $(RFILES1) $(RFILES2) $(RFILES3) $(RFILES4) \
        $(RFILES5) $(RFILES6) $(RFILES7)
   del main.l
   chd rels; cat $(RFILES1)  >../temp1
   chd rels; cat pffinit_c $(RFILES2)  scanf_c >../temp2
   chd rels; cat $(RFILES3)  >../temp3
   chd rels; cat $(RFILES4)  >../temp4
   chd rels; cat $(RFILES5)  >../temp5
   cat temp1 temp2 temp3 temp4 temp5 >temp6
   del temp1 temp2 temp3 temp4 temp5
   chd rels; cat l3tol_c ltol3_c qsort_c $(RFILES6)  >../temp7
   chd rels; cat $(RFILES7)  >../temp8
   cat temp6 temp7 temp8 >main.l
   del temp6 temp7 temp8
