/* filesyscall.c
 *	Simple program to test file system syscall (Lab 6)
 */

#include "syscall.h"

#define BUFFER_SIZE 11

int main() {
    char data[9]; // as file name and content
    char buffer[9];
    OpenFileId fid_write;
    OpenFileId fid_read;
    int numBytes;

    data[0] = 't';
    data[1] = 'e';
    data[2] = 's';
    data[3] = 't';
    data[4] = '.';
    data[5] = 't';
    data[6] = 'x';
    data[7] = 't';
    data[8] = '\0';

    Create(data);

    fid_write = Open(data);
    fid_read = Open(data);

    Write(data, 8, fid_write);

    numBytes = Read(buffer, 8, fid_read);

    Close(fid_write);
    Close(fid_read);

    Exit(numBytes);
}
