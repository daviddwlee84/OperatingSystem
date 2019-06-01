# NachOS

Not Another Completely Heuristic Operating System, or Nachos, is instructional software for teaching undergraduate, and potentially graduate level operating systems courses. It was developed at the University of California, Berkeley, designed by Thomas Anderson, and is used by numerous schools around the world.

* [Wiki - Not Another Completely Heuristic Operating System](https://en.wikipedia.org/wiki/Not_Another_Completely_Heuristic_Operating_System)

## Dependency Graph

In `Makefile.common`

```makefile
# The dependency graph between assignments is:
#   1. THREADS before everything else
#   2. USERPROG must come before VM
#   3. USERPROG can come before or after FILESYS, but if USERPROG comes
#      before (as in this distribution), then it must define FILESYS_STUB
#
#   Other than that, you have complete flexibility.
```

## Directory Structure

Makefiles

* Makefile
* Makefile.common
* Makefile.dep

Folder

* bin: for MIPS interpreter (coff2noff)
* machine: for MIPS environment simulation
* threads: main and threads
* userprog: user-space program
* test: some test user program
* filesys
* network
* vm

> * [Nachos:- Directory listing](https://www.cs.odu.edu/~cs471/soumya/list.html)

## Executables

* `nachos`: one for each compile (subdirectory test)
* `coff2noff`: converting a COFF (Common Object File Format) file to a NOFF file (Nachos Object File Format)

### coff2noff

> For more detail checkout [here](COFFvsNOFF.md)

Nachos runs user programs in their own private address space. Nachos can run any MIPS binary.

In Unix, "a.out" files are stored in "coff" format. Nachos requires that executables be in the simpler "Noff" format.

Use the `coff2noff` program to convert binary. Consult the `code/test/Makefile` for details.

When executing a program, Nachos

1. creates an address space
2. copies the contents of the instruction
3. initialized variable segments into the address space
4. (the uninitialized variable section doesn't need to be read from the file, since it is defined to contain all zeros)

#### Noff

Noff-format files consist of four parts

1. Noff header
   * describes the contents of the rest of the file
   * giving information about the program's instruction, initialized variables and uninitialized variables

Noff header resides at the very start of the file. And containes pointers to the remaining section.

* noffMagic: a reserved "magic" number that indicates that the file is in Noff format. (first 4 bytes of the file)
* virtualAddr: what virtual address that segment begins at (normally zero)
* inFileAddr: pointer within the Noff file where that section actuall begins (so that Nachos can read it into memory before execution begins)
* size:the size (in bytes of that segment)

## Debugging Nachos

* [**Debugging Nachos**](https://www.student.cs.uwaterloo.ca/~cs350/common/debug.html)

### Internal Debugging in Nachos

#### The Trace Facility

The `-d` argument

```sh
nachos -d <debug_flag>
```

> `code/lib/debug.h` (does not exist in current version Nachos code, but the flags are the same)
>
> ```c
> // The pre-defined debugging flags are:
>
> const char dbgAll = '+';		// turn on all debug messages
> const char dbgThread = 't';		// threads
> const char dbgSynch = 's';		// locks, semaphores, condition vars
> const char dbgInt = 'i'; 		// interrupt emulation
> const char dbgMach = 'm'; 		// machine emulation (USER_PROGRAM)
> const char dbgDisk = 'd'; 		// disk emulation (FILESYS)
> const char dbgFile = 'f'; 		// file system (FILESYS)
> const char dbgAddr = 'a'; 		// address spaces (USER_PROGRAM)
> const char dbgNet = 'n'; 		// network emulation (NETWORK)
> ```

The debug flag is defined in `code/threads/utility.h`

```c
//	The debugging routines allow the user to turn on selected
//	debugging messages, controllable from the command line arguments
//	passed to Nachos (-d).  You are encouraged to add your own
//	debugging flags.  The pre-defined debugging flags are:
//
//	'+' -- turn on all debug messages
//   	't' -- thread system
//   	's' -- semaphores, locks, and conditions
//   	'i' -- interrupt emulation
//   	'm' -- machine emulation (USER_PROGRAM)
//   	'd' -- disk emulation (FILESYS)
//   	'f' -- file system (FILESYS)
//   	'a' -- address spaces (USER_PROGRAM)
//   	'n' -- network emulation (NETWORK)

...

extern void DEBUG (char flag, char* format, ...);  	// Print debug message 
							// if flag is enabled
```

> I've add `c` for (random) Context Switch used in `threads/system.cc`.
> I've add `b` for Barrier related in `threads/synch.cc`
> I've add `w` for Reader-Writer Lock related in `threads/synch.cc`
> I've add `T` for TLB handling in `machine/exception.cc`
> I've add `M` for memory management (allocation, data structure (e.g. bitmap)) in `machine/machine.cc`
> I've add `S` for system call in `machine/exception.cc`
> I've add `D` for multi-level directory in `filesys/filesys.cc`

#### Single Stepping

The `-s` argument

#### Assertions

```c
ASSERT()
```

### External Debugger

#### Using GDB with Nachos

```sh
gdb nachos
```

## Links

* [**Washington Nachos (Official Website)**](https://homes.cs.washington.edu/~tom/nachos/)
* [**Nachos Beginner's Guide**](https://www.ida.liu.se/~TDDI04/material/begguide/) - Nachos support documentation
  * [A Road Map Through Nachos](https://www.ida.liu.se/~TDDI04/material/begguide/roadmap/index.html)
* [**Nachos Introduction Slides**](http://mescal.imag.fr/membres/vania.marangozova-martin/TEACHING/NACHOS/nachos2009_2010.pdf)
* [NYU Nachos Resources](https://cs.nyu.edu/courses/spring04/V22.0202-001/nachos-resources.htm)
* [Nachos Overview](https://www.cs.odu.edu/~cs471/soumya/salsa.html)
  * [Nachos Details](https://www.cs.odu.edu/~cs471/soumya/details.html)
* [CS 1550 Nachos](https://people.cs.pitt.edu/~manas/courses/CS1550/nachos.htm)
* [NachOS安裝心得](https://zishen.blogspot.com/2011/04/nachos.html)

### UC Berkeley

* [CS162: Operating Systems and Systems Programming](https://cs162.eecs.berkeley.edu/)
  * Book Reading
    * Operating Systems: Principles and Practice 2nd Edition (Required)
    * Operating System Concepts 9th Edition (Recommended)
    * Understanding the Linux Kernel, Third Edition (Supplemental)
    * Linux Kernel Development 3rd Edition (Supplemental)
  * [Semester archives](https://inst.eecs.berkeley.edu/~cs162/archives.html)
  * [CS162 Spring 2005](http://www-inst.eecs.berkeley.edu/~cs162/sp05/Nachos/)
  * [CS162 Spring 2007 Phases](https://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/phases.shtml) - Using Java
    * [Nachos Phase 1: Build a thread system](https://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/phase1.shtml)
    * [Nachos Phase 2: Multiprogramming](https://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/phase2.shtml)
    * [Nachos Phase 3: Caching and Virtual Memory](https://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/phase3.shtml)
    * [Nachos Phase 4: Networks And Distributed Systems](https://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/phase4.shtml)

### University of Waterloo

* [University of Waterloo CS350 Resources](https://www.student.cs.uwaterloo.ca/~cs350/common/)
* [**Nachos: Installation and Getting Started**](https://www.student.cs.uwaterloo.ca/~cs350/common/install.html)

#### C++

* [A Road Map Through Nachos](https://users.cs.duke.edu/~narten/110/nachos/main/main.html)
  * [**pdf**](http://www.cs.kent.edu/~javed/class-OS10S/nachos-roadmap.pdf)
* [Github - Nachos 4.1](https://github.com/tfriedel/nachos)
* [University of Chicago Nachos](http://people.cs.uchicago.edu/~odonnell/OData/Courses/CS230/NACHOS/reading-code.html)
* [**Build Nachos 4.1**](http://web-ext.u-aizu.ac.jp/~yliu/teaching/os/lab01.html)

#### JAVA

* [A Guide to Nachos 5.0j](http://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/walk/walk.html)
  * [pdf](https://inst.eecs.berkeley.edu/~cs162/sp08/Nachos/walk/walk.pdf)
* [Github - Nachos for Java](https://github.com/thinkhy/CS162/tree/master/nachos)
* [Nachos Projects](https://people.eecs.berkeley.edu/~kubitron/courses/cs162-F05/Nachos/index.html)

### Duke University

* CPS 110 / EE 153 Operating Systems - [Y2K spring](http://db.cs.duke.edu/courses/spring00/cps110/)
  * [Nachos Project Guide](http://db.cs.duke.edu/courses/spring00/cps110/nachos-guide/)
    * [Nachos Lab Assignments](http://db.cs.duke.edu/courses/cps110/spring00/nachos-guide/nachos.10.html)
      * Lab 1: The Trouble with Concurrent Programming
      * Lab 2: Threads and Synchronization
      * Lab 3: Programming with Threads
      * Lab 4: Multiprogrammed Kernel
      * Lab 5: I/O
      * Lab 6: Virtual Memory

### Longwood University

* [CMSC 442 Operating Systems](https://www.coursicle.com/longwood/courses/CMSC/442/)
  * [Lab 1: Semaphores](http://marmorstein.org/~robert/Fall2017/cs442/Lab1-Trains.pdf)
  * [Lab 2: Building a Thread System](http://marmorstein.org/~robert/Fall2017/cs442/Lab2-NachosThreading.pdf)
  * [Lab 3: Multiprogramming in NachOS](http://marmorstein.org/~robert/Fall2017/cs442/Lab3-UserSpace.pdf)
  * [Lab 4: Virtual Memory in NachOS](http://marmorstein.org/~robert/Fall2017/cs442/Lab4-VirtualMemory.pdf)
  * [Lab 5: File Systems](http://marmorstein.org/~robert/Fall2017/cs442/Lab5-Filesystem.pdf)
