# Thread

## Overview

Why we need thread inside the process?

* Application
* Cost
* Performance

If without thread?

* Single service process
  * lower performance
  * sequencial programming (i.e. simple)
* Finite state machine
  * use non-blocking I/O
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

* sometimes called light-weight process

### Thread Attribute

* has *status*
* need to store *context* while not running
  * registers (e.g. program counter, etc.)
* has its own stack and stack pointer
    > return value of function is in stack
* share process address space and other resources

![OS concepts threads](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter4/4_01_ThreadDiagram.jpg)

## Implementation

* User level thread
  * Implement in user space
  * e.g. Use [POSIX Pthreads](https://en.wikipedia.org/wiki/POSIX_Threads) library
* Kernel level thread
  * Implement by modify kernel or OS
* Mix these two together

### User Level Thread

TBD

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

TBD

#### Windows Thread Library

## Resources

* [OS Course notes - Threads](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/4_Threads.html)
* [Kernel level thread and User level Thread](https://www.embhack.com/kernel-level-thread-and-user-level-thread/)