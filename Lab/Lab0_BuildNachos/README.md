# Lab 0: Build Nachos

## Preparation

* [Development Environment](DevEnvironment.md)
  * 32-bit gcc, g++

## Exercise

1. Learn Makefile
   * Understand Nachos Makefile structure
2. Understanding Nachos code structure
   * Read `threads/main.cc`, understand the booting process.
   * Read `threads/system.cc` and `threads/system.h`, understand the OS and VM component

## Steps for Nachos-3.4

Because we compile 32-bit program on 64-bit OS. We need to add some flag in the `Makefile`.

1. Modify `code/Makefile.common`
   * add `-m32` after gcc and g++
   * add `--32` after as (or change `$(AS) -o switch.o swtch.s` to `$(AS) --32 -o switch.o swtch.s`)
2. (alterative to step 1) Modify `code/Makefile.dep`
   * change `HOSTCFLAGS` from `HOSTCFLAGS = -Dx86 -DLINUX` to `HOSTCFLAGS = -Dx86 -DLINUX -march=athlon64 -O2 -m32`
   * add `-m32` after `LDFLAGS =`
3. Also add `-m32` in `code/bin/Makefile`

**Build** (in `code/`)

```sh
make
```

This step will modify the following files automatically

* `filesys/Makefile`
* `network/Makefile`
* `threads/Makefile`
* `userprog/Makefile`
* `vm/Makefile`
* `test/halt`
* `test/matmult`
* `test/shell`
* `test/sort`

and generate the following files

* `bin/coff2noff`
* `test/halt.coff`
* `test/matmult.coff`
* `test/shell.coff`
* `test/sort.coff`
* `filesys/nachos`
* `filesys/swtch.s`
* `network/nachos`
* `network/swtch.s`
* `threads/nachos`
* `threads/swtch.s`
* `userprog/nachos`
* `userprog/swtch.s`
* `vm/nachos`
* `vm/swtch.s`

**Test**. Execute `userprog/nachos`

```txt
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 10, idle 0, system 10, user 0
Disk I/O: reads0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0

Cleaning up...
```

## Steps for Nachos-4.1

1. Compile `coff2noff`
   1. Run `coff2noff/configure`
   2. Modify Makefile and add 32bit flag
2. Modify `code/lib/sysdep.h`
   * Change `#include "iostream.h"` to
        ```c
        #include "iostream"
        using namespace std;
        ```
3. Go to `code/build.linux`
4. modify Makefile
   * add 32bit flag
   * remove `-fwritable-strings` form CFLAGS
5. `make depend`
6. `make nachos` or just `make` (TODO)
7. Run `./nachos -u`

> haven't finish yet

## Trouble shooting

1. `code/threads/switch.c > switch.s`

    ```txt
    switch.s: Assembler messages:
    switch.s:5: Error: invalid instruction suffix for 'push'
    ... for 'call'
    ... for 'pop'
    ```

    * [Solution](https://uw.cs.cs350.narkive.com/bs6gQ2yP/nachos-on-linux)

    > didn't add `--32` to assembler (as)

2. Cannot find `-lstdc++`

> didn't install `g++-multilib`

3. `code/bin/coff2noff halt.coff halt`

    ```txt
    File is not a OMAGIC file
    ```

    > This happen because `code/bin/coff2noff` that we've compiled is 64-bit.
    >
    > Program can be check by `$ file filename`. ([How to check if my software is 32-bit or 64-bit](https://askubuntu.com/questions/437857/how-to-check-if-my-software-is-32-bit-or-64-bit))

    > didn't add `-m32` to `code/bin/Makefile`

4. make (Nachos-4.1 build.linux)

    ```txt
    /usr/include/x86_64-linux-gnu/c++/7/32/bits/error_constants.h

    error: ‘EAFNOSUPPORT’ was not declared in this scope
    error: ‘EADDRINUSE’ was not declared in this scope
    ...
    Makefile:339 recipe for target 'bitmap.o' failed
    ```

    > It seems to be C vs. C++ problem, not sure.
