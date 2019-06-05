# Thread

## Overview

Why we need thread inside the process?

* Application
* Cost
* Performance

If **without** thread?

* **Single service process**
  * lower performance
  * sequencial programming (i.e. simple)
* **Finite state machine**
  * use **non-blocking I/O**
    * [Wiki - Asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O)
  * complex programming model

Three ways to construct a server (Modern OS)

Model|Feature
-----|-------
Threads|Parallelism, blocking system calls
Single-threaded process|No parallelism, blocking system calls
Finite-state machine|Parallelism, nonblocking system calls, interrupts

## Basic Concept

> Process: the Recources owner

Thread: a instance of a process, the scheduling unit of CPU

* sometimes called **light-weight process**

### Thread Attribute

* has *status*
* need to store *context* while not running
  * registers (e.g. program counter, etc.)
* has its own stack and stack pointer
    > return value of function is in the stack
* share process address space and other resources

![OS concepts threads](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter4/4_01_ThreadDiagram.jpg)

## Implementation

* User level thread
  * Implement in user space
  * e.g. Use [POSIX Pthreads](https://en.wikipedia.org/wiki/POSIX_Threads) library
* Kernel level thread
  * Implement by modifying kernel or OS
* Mix these two together
  * Creating thread in user space and scheduling in kernel space
  * Many user threads share (run on) a kernel thread
  * e.g. Solaris

### User Level Thread

* Pros and Cons
  * Pros
    * It can run on any OS (just need the library)
  * Cons
    * Most of the system call is blocking, thus if kernel block the "process" then all the thread the process has will be blocked too. (because in the aspect of OS, there is only one process)

#### POSIX Pthreads

> POSIX stands for Portable Operating System Interface

Thread call|Description
-----------|-----------
Pthread_create|Create a new thread
Pthread_exit|Terminate the calling thread
Pthread_join|Wait for a specific thread to exit
Pthread_yield|Release the CPU to let another thread run
Pthread_attr_init|Create and initialize a thread's attribute structure
Pthread_attr_destroy|Remove a thread's attribute structure

#### Blocking System Call Management

* Modify system to non-blocking
* Reimplement the I/O library system call function

#### Pros and Cons of User Level Thread

Kernel don't know the existence of threads

TBD

### Kernel Level Thread

Example in real world

#### Linux Thread

Linux does not distinguish between processes and threads - It uses the more generic term "tasks"

Check out the `task_struct`

* `fork()`
* `clone()`

#### Windows Thread Library

![Windows thread](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter4/4_14_WindowsThreadStructures.jpg)

## Conclusion of Thread and Process

Process is the "owner of resources" (e.g. Memory); Thread is the deploy unit of CPU

## Resources

* [OS Course notes - Threads](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/4_Threads.html)
* [Kernel level thread and User level Thread](https://www.embhack.com/kernel-level-thread-and-user-level-thread/)

### Book

Operating System Concepts 9ed.

* Notes
  * [Threads](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/4_Threads.html)
