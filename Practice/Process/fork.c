#include <stdio.h>
#include <unistd.h>

// warning: implicit declaration of function 'wait' is invalid in C99 [-Wimplicit-function-declaration]
// https://stackoverflow.com/questions/41884685/implicit-declaration-of-function-wait
#include <sys/wait.h>
#include <sys/types.h>

// warning: implicitly declaring library function 'exit' with type 'void (int) __attribute__((noreturn))' [-Wimplicit-function-declaration]
// https://stackoverflow.com/questions/2406986/c-warning-implicit-declaration-of-function-exit
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork(); // Create a child process
    if(pid < 0){
        fprintf(stderr, "fork failed");
        exit(-1);
    } else if(pid == 0) { // Child process
        execlp("/bin/ls", "ls", NULL);
    } else { // Parent process
        wait(NULL); // parent will wait for the child to complete
        printf("child complete");
        exit(0);
    }
}
