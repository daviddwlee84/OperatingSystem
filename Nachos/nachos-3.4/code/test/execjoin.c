/* execjoin.c
 *	Simple program to test the thread syscall (Lab 6) using the
 *  previous made "exit" program.
 *  This is used to test the address space control operations
 *  This will create executable Exec child with it and the parent will Join
 *  the thread and then Exit with the result
 */

#include "syscall.h"

int main() {
    char executable[5];
    int exitCode;
    SpaceId sp;

    executable[0] = 'e';
    executable[1] = 'x';
    executable[2] = 'i';
    executable[3] = 't';
    executable[4] = '\0';

    sp = Exec(executable);

    exitCode = Join(sp);
    Exit(exitCode);
}