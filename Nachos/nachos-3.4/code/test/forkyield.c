/* forkyield.c
 *	Simple program to test the thread syscall (Lab 6)
 *  This is used to test the user-level thread operations
 *  This example will create two thread and each of them
 *  will write something in the file.
 */

#include "syscall.h"

char file[9];

void ThreadA() {
    char ch = 'a';
    int i;
    OpenFileId fd;

    fd = Open(file);
    for(i = 0; i < 20; i ++) {
        Write(&ch, 1, fd);
        Yield();
    }
}

void ThreadB() {
    char ch = 'b';
    int i;
    OpenFileId fd;

    fd = Open(file);
    for(i = 0; i < 10; i ++) {
        Write(&ch, 1, fd);
        Yield();
    }
}

int main() {
    file[0] = 't';
    file[1] = 'e';
    file[2] = 's';
    file[3] = 't';
    file[4] = '.';
    file[5] = 't';
    file[6] = 'x';
    file[7] = 't';
    file[8] = '\0';

    Create(file);

    Fork(ThreadA);
    Fork(ThreadB);
}