#include <stdio.h>
#include <unistd.h>

int main()
{
            // 1
    fork(); // 2
    fork(); // 4
    fork(); // 8
    printf("hello!\n");

    return 0;
}