/* exit.c
 *	Simple program to test multi-thread user program (Lab 4)
 */

#include "syscall.h"

int
main()
{
    int i;
    for (i = 0; i < 100; i++) {
        // do nothing
    }
    Exit(87);
}
