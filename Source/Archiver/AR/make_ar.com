$ cc lz1init.c
$ cc lz1glob.c
$ cc delz1.c
$ cc lz1.c
$ cc dir.c
$ cc arsup.c
$ cc ar.c
$ link ar,arsup,dir,lz1,delz1,lz1glob,lz1init
