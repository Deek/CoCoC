# collapse tabs and multiple spaces
s/	/ /g
s/  */ /g
# delete lines for true and false
s/^true.*//g
s/^false.*//g
# rewrite uses of true and false
s/ true / 1 /g
s/ false / 0 /g
# delete foo.D SET 1 lines at top
/^[0-9A-Z]*.D SET.*/d
s/ RMB/ rmb/g
s/ EQU/ equ/g
s/ SET/ equ/g
s/ ORG/ org/g
s/ org / ends\
 csect /g
# delete the first artificial "ends" we add
1,/ends/{s/ends//g;}
s/\([^ ]\) rmb/\1: rmb/
s/\([^ ]\) equ/\1: equ/
s/ equ \./ equ \*/g
s/ rmb \./ rmb \*/g
s/\(equ [$0-9a-fA-F]*-\)\./\1\*/g
s/\(rmb [$0-9a-fA-F]*-\)\./\1\*/g
