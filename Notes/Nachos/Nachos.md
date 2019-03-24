# NachOS

Not Another Completely Heuristic Operating System, or Nachos, is instructional software for teaching undergraduate, and potentially graduate level operating systems courses. It was developed at the University of California, Berkeley, designed by Thomas Anderson, and is used by numerous schools around the world.

* [Wiki - Not Another Completely Heuristic Operating System](https://en.wikipedia.org/wiki/Not_Another_Completely_Heuristic_Operating_System)

## Directory Structure

## Executables

* `nachos`
* `coff2noff`: converting a COFF (Common Object File Format) file to a NOFF file (Nachos Object File Format)

## Debugging Nachos

* [**Debugging Nachos**](https://www.student.cs.uwaterloo.ca/~cs350/common/debug.html)

### Internal Debugging in Nachos

#### The Trace Facility

The `-d` argument

```sh
nachos -d t
```

`code/lib/debug.h` (does not exist in current version Nachos code, but the flags are the same)

```c
// The pre-defined debugging flags are:

const char dbgAll = '+';		// turn on all debug messages
const char dbgThread = 't';		// threads
const char dbgSynch = 's';		// locks, semaphores, condition vars
const char dbgInt = 'i'; 		// interrupt emulation
const char dbgMach = 'm'; 		// machine emulation (USER_PROGRAM)
const char dbgDisk = 'd'; 		// disk emulation (FILESYS)
const char dbgFile = 'f'; 		// file system (FILESYS)
const char dbgAddr = 'a'; 		// address spaces (USER_PROGRAM)
const char dbgNet = 'n'; 		// network emulation (NETWORK)
```

> I've add `c` for (random) Context Switch used in `threads/system.cc`.
> I've add `b` for Barrier related in `threads/synch.cc`
> I've add `w` for Reader-Writer Lock related in `threads/synch.cc`

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
* [NYU Nachos Resources](https://cs.nyu.edu/courses/spring04/V22.0202-001/nachos-resources.htm)
* [Nachos Overview](https://www.cs.odu.edu/~cs471/soumya/salsa.html)
  * [Nachos Details](https://www.cs.odu.edu/~cs471/soumya/details.html)

### UC Berkeley

* [CS162: Operating Systems and Systems Programming](https://cs162.eecs.berkeley.edu/)
  * Book Reading
    * Operating Systems: Principles and Practice 2nd Edition (Required)
    * Operating System Concepts 9th Edition (Recommended)
    * Understanding the Linux Kernel, Third Edition (Supplemental)
    * Linux Kernel Development 3rd Edition (Supplemental)
  * [Semester archives](https://inst.eecs.berkeley.edu/~cs162/archives.html)

### University of Waterloo

* [University of Waterloo CS350 Resources](https://www.student.cs.uwaterloo.ca/~cs350/common/)
* [**Nachos: Installation and Getting Started**](https://www.student.cs.uwaterloo.ca/~cs350/common/install.html)

#### C++

* [A Road Map Through Nachos](https://users.cs.duke.edu/~narten/110/nachos/main/main.html)
* [Github - Nachos 4.1](https://github.com/tfriedel/nachos)
* [University of Chicago Nachos](http://people.cs.uchicago.edu/~odonnell/OData/Courses/CS230/NACHOS/reading-code.html)
* [**Build Nachos 4.1**](http://web-ext.u-aizu.ac.jp/~yliu/teaching/os/lab01.html)

#### JAVA

* [A Guide to Nachos 5.0j](http://inst.eecs.berkeley.edu/~cs162/sp07/Nachos/walk/walk.html)
* [Github - Nachos for Java](https://github.com/thinkhy/CS162/tree/master/nachos)
* [Nachos Projects](https://people.eecs.berkeley.edu/~kubitron/courses/cs162-F05/Nachos/index.html)
