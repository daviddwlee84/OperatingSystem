#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    int x = 1;

    /* fork a child process */
    pid = fork();

    if(pid == 0){ /* child process */
        printf("child: pid=%d, x=%d\n", pid, ++x);
        return 0;
    } else { /* parent process */
        /* parent will wait for the child to complete */
        printf("parent: pid=%d, x=%d\n", pid, --x);
        return 0;
    }
}