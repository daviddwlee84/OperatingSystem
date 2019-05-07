/* fibonacci.c
 *	Simple program to test the TLB miss rate (Lab 4) that calculate the fibonacci series
 */

#include "syscall.h"

#define N 20

int
main()
{
    int i;
    int result[N];
    result[0] = 0;
    result[1] = 1;
    for (i = 2; i < N; i++)
    {
        result[i] = result[i-1] + result[i-2];
    }
    Exit(result[N-1]);
}
