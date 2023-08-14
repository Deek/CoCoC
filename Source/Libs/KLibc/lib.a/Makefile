CFLAGS  = -s
RDIR    = RELs
RFILES1 = swab.r pwcryp.r bsearch.r dirutil.r errmsg.r prgname.r \
          datmods.r devtyp.r skip.r sets.r pflinit.r
RFILES2 = fopen.r puts.r gets.r mktemp.r fread.r fwrite.r printf.r
RFILES3 = setbuf.r fseek.r putc.r ftelldummy.r getc.r flshdummy.r setbase.r \
          findstr.r pfldummy.r pffdummy.r htoi.r htol.r ltoa.r xtoa.r
RFILES4 = system.r reverse.r strings.r strcmp.r strlen.r strncpy.r strncat.r \
          strncmp.r strhcpy.r strtok.r strpbrk.r strspn.r index.r patmatch.r
RFILES5 = stringsu.r strucmp.r strnucmp.r case.r strclr.r memccpy.r memchr.r \
          memcmp.r memcpy.r memset.r atol.r atoi.r iob_data.r chcodes.r
RFILES6 = sleep.r setjmp.r strass.r realloc.r calloc.r memory.r rand.r
RFILES7 = atof.r scale.r ldexp.r frexp.r gs1.r gs2.r ss1.r ss2.r ss3.r

main.l: $(RFILES1) $(RFILES2) $(RFILES3) $(RFILES4) \
        $(RFILES5) $(RFILES6) $(RFILES7)
	-del main.l
	cd rels; merge $(RFILES1)  >-../temp1
	cd rels; merge pffinit_c $(RFILES2)  scanf_c >-../temp2
	cd rels; merge $(RFILES3)  >-../temp3
	cd rels; merge $(RFILES4)  >-../temp4
	cd rels; merge $(RFILES5)  >-../temp5
	merge temp1 temp2 temp3 temp4 temp5 >-temp6
	-del temp1 temp2 temp3 temp4 temp5
	cd rels; merge l3tol_c ltol3_c qsort_c $(RFILES6)  >-../temp7
	cd rels; merge $(RFILES7)  >-../temp8
	merge temp6 temp7 temp8 >-main.l
	-del temp6 temp7 temp8

.c.r:
	-del $(RDIR/)asm.tmp
	$(CC) $(CFLAGS) -Of=$(RDIR/)asm.tmp $<
	$(RMA) $(RFLAGS) -o=$(RDIR/)$@ $(RDIR/)asm.tmp
	-del $(RDIR/)asm.tmp

clean:
	-:del RELs/*.r temp*
