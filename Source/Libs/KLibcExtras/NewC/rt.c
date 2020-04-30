#include <stdio.h>

/*
** you never declared it (and default = int).
*/

long time();

main()
{
    int     n, i, j;
    long    m;

/*
** compiler it's not an int. Therefore m = 2415L instead of
** m = 2415.
*/

    m = time(NULL);
     printf("Seed: %d\n", (int) m);

    srand(m);
    for(i=0; i<10; ++i) {
        for(j=0; j<4; ++j) {
            n = rand();

/*
** number, you must tell it it's a long using %15ld, and not %15d
*/

            printf("%15d  ",n);
        }
        printf("\n");
    }
}
