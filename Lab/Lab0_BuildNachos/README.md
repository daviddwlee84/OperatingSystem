# Lab 0: Build Nachos

## Preparation

* [Development Environment](../../Notes/DevEnvironment.md)
  * 32-bit gcc, g++

## Steps

Because we compile 32-bit program on 64-bit OS. We need to add some flag in the `Makefile`.

1. Modify `code/Makefile.common`
   * add `-m32` after gcc and g++
   * add `--32` after as (or change `$(AS) -o switch.o swtch.s` to `$(AS) --32 -o switch.o swtch.s`)
2. (alterative to step 1) Modify `code/Makefile.dep`
   * change `HOSTCFLAGS` from `HOSTCFLAGS = -Dx86 -DLINUX` to `HOSTCFLAGS = -Dx86 -DLINUX -march=athlon64 -O2 -m32`
   * add `-m32` after `LDFLAGS =`
3. Also add `-m32` in `code/bin/Makefile`

Build (in `code/`)

```sh
make
```

Test. Execute `userprog/nachos`

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
