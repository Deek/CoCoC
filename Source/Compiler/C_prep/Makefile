c_prep : cp.r cp1.r cp2.r cp3.r cp4.r findstr.r solve.r
         $(CC) -f=c_prep -m20K -e10 cp.r cp1.r cp2.r cp3.r cp4.r findstr.r solve.r

cp.r: cp.c cp.h

cp1.r: cp1.c cp.h

cp2.r: cp2.c cp.h

cp3.r: cp3.c cp.h

cp4.r: cp4.c cp.h

findstr.r: findstr.c

solve.r: solve.a

lha: c_prep
     touch c_prep
     del c_prep
     copy /dd/cmds/c_prep c_prep
     touch cprep19a.lzh
     del cprep19a.lzh
     lha a -x cprep19a c_prep *.c *.h *.a *.doc makefile


