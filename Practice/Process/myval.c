#include <stdio.h>
#include <stdlib.h>

int myval;
int main(int argc, char * argv[])
{
    myval = atoi(argv[1]);
    while(1)
    {
        printf("myval is %d, loc 0x%lx\n", myval, (long) &myval);
    }
    return 0;
}