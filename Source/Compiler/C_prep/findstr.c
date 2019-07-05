/*
findstr(pos,string,pattern)
int pos;
char *string, *pattern;

return int position of 1st matching char (1-...)
else NULL if no match
else ERROR if pattern is NULL pointer
*/

#asm
findstr:
 pshs u,y
 ldd 6,s  (pos)
 addd 8,s (string)
 subd #1
 tfr d,y  (string+pos)
 ldu 10,s (pattern)
 tst ,u
 beq NMATCH
LP2
 lda ,y+
 beq NMATCH
 cmpa ,u
 bne LP2
 leax ,y
 leau 1,u
LP3
 tst ,u
 beq MATCH2
 lda ,y+
 beq NMATCH
 cmpa ,u+
 beq LP3
 ldu 10,s
 leay ,x
 bra LP2
MATCH2
 tfr x,d
 subd 8,s
 puls u,y,pc
NMATCH
 clra
 clrb
 puls u,y,pc
#endasm

