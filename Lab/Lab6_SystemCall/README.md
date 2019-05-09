# Lab 6: System Call

## I. Understanding Nachos System Call

### Exercise 1: Trace code

> Read the system call related code, understand current Nachos system call mechanism
>
> * `code/userprog/syscall.h`
> * `code/userprog/exception.cc`
> * `code/test/start.s`

## II. File System Related System Call

### Exercise 2: Implement file system system call

> On the analogy of the Halt implementation, complete the following system calls
>
> * Create
> * Open
> * Close
> * Write
> * Read
>
> There are basic description of the system calls in `code/userprog/syscall.h`

#### Create

#### Open

#### Read

#### Write

#### Close

### Exercise 3: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 2 and test the correctness.

## III. Executing User Program Related system call

### Exercise 4: Implement user program system call

> Implement the following system calls
>
> * Exec
> * Fork
> * Yield
> * Join
> * Exit
>
> There are basic description of the system calls in `code/userprog/syscall.h`

#### Exit

#### Exec

#### Join

#### Fork

#### Yield

### Exercise 5: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 4 and test the correctness.

#### Test Exec, Exit, Join

![Nachos Exec/Exit/Join Example](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/img005.gif)

## Resources

CSC546 - Operating Systems

* [Lecture 4](http://condor.depaul.edu/glancast/546class/docs/lec4.html)
  * **Exit**
    * [System Calls Need to Adjust the PC Registers](http://condor.depaul.edu/glancast/546class/docs/adjustRegs.html)
  * Adjust Registers
  * Unix File System
  * Nachos File System
  * fileSystem
  * Nachos Write system call
* [**Lecture 9**](http://condor.depaul.edu/glancast/546class/docs/lec9.html)
  * Join
  * **Thread Class Additions**
  * List Class Additions
  * rcsmerge
  * Join Details
  * Exec Changes
  * **Exit Changes**

[CPS 110 / EE 153 Operating Systems (Duke University)](https://users.cs.duke.edu/~chase/cps110/)

* [**Nachos Exec/Exit/Join Example**](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld005.htm) - [Home - Process, Unix and IPC](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld001.htm)
  * [pdf - The Classical OS Model in Unix](https://users.cs.duke.edu/~chase/cps210-archive/slides/ux6.pdf)

### Example

Others

* [System call definitions](https://users.cs.duke.edu/~carla/syscall_def.html)

Yet Another

* [srijanshetty/nachos-syscalls](https://github.com/srijanshetty/nachos-syscalls)
* [manikantareddyd/NACHOS-syscall-implementation](https://github.com/manikantareddyd/NACHOS-syscall-implementation/)
