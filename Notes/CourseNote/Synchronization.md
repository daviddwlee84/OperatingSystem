# Process Synchronization and Interprocess Communication

## Overview

### Process Parallelism

> All the problem is caused by "parallelism"

Some time a single line of C code. Is multiple instruction in Assembly. => If the execution *under some scenario* being interrupt *on specific step* then may cause error.

#### Comparison between sequential system and parallel system

### Two Main Problem Caused by Parallelism

#### Mutual Exclusion 互斥 - Shared Memory/Data (Interprocess Communication)

* [Wiki - Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

#### Synchronization 同步

## Race Condition

### Example of Spooling - Print Job Request

![Example of Spooling](https://images.slideplayer.com/20/6005967/slides/slide_2.jpg)

* [Wiki - Spooling](https://en.wikipedia.org/wiki/Spooling)

## Critical Section/Region

Overview

* Approach
  * Software => Programming skill
    * Dekker
    * Peterson
  * Hardware
    * Block Interrupt
    * TSL(XCHG) Instruction

* [Wiki - Critical section](https://en.wikipedia.org/wiki/Critical_section)

### 4 Condition of Using Critical Region

* 
* 
* 
* 

### Software Solutions

#### Attemption 1

#### Attemption 2

#### Attemption 3

#### Dekker Solution

#### Peterson Solution

### Hardware Solution

#### Block Interrupt

#### Special Instruction

**TSL**: Test And Set Lock

**XCHG**: Exchange Data

### Summary - Spinlock

#### Busy Waiting

In single CPU system

* Must prevent

In multiple CPU system

* Sometimes works better
  * If all the critical region is very short
  * Don't need context switch (process stay on the CPU)

## Semaphore

### P, V Operation

> Primitive / Atomic Action => Uninterruptable

#### P

#### V

## Monitor

> Nowaday can be implemented as a class (it was been invented before object-oriented concept)
>
> Monitor is a "Language-based" Mechanism

### Wait and Signal Operation

### Consider Parallelism Problem

Mutual Exclusion => Maintain by Compiler

Synchronization => Maintain by Programmer (use Wait and Signal operation)

### Different Detail Monitor Implementation

#### Hoare Monitor

![Wiki A Hoare style monitor with two condition variables a and b](https://upload.wikimedia.org/wikipedia/commons/thumb/d/db/Monitor_%28synchronization%29-SU.png/200px-Monitor_%28synchronization%29-SU.png)

#### MESA Monitor

Signal() -> Notify()

![Wiki MESA style monitor](https://upload.wikimedia.org/wikipedia/commons/thumb/1/15/Monitor_%28synchronization%29-Mesa.png/200px-Monitor_%28synchronization%29-Mesa.png)

#### Hansen Monitor

### Solving Producer-Consumer Problem with Monitor

### Monitor Implementation

* Directly construct monitor -> Efficient but need support of programming language
* Indirectly construct monitor -> Use existing synchronization mechanism to implement
  * Semaphore

#### Monitor in Java

![Wiki A Java style monitor](https://upload.wikimedia.org/wikipedia/commons/thumb/f/f5/Monitor_%28synchronization%29-Java.png/200px-Monitor_%28synchronization%29-Java.png)

## Mutex Lock and Condition Variable using Pthread

> Pthread is a **user space** library. So you should assume OS don't know the thread that you're blocking.

### Mutex Lock

#### Why implement pthread_mutex_trylock

### Condition Variable

#### Why use While instead of If on pthread_cond_wait

* [Stackoverflow - Why do you need a while loop while waiting for a condition variable](https://stackoverflow.com/questions/7766057/why-do-you-need-a-while-loop-while-waiting-for-a-condition-variable)
* [Wiki - Spurious wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)

### Wait and Signal

## Implementation

* Application
  * Parallel process
* System
  * Synchronization Mechanism
    * Semaphore
    * Monitor
    * Lock
    * Condition Variable
* Hardware
  * Atomic Operation
    * Load/Store
    * Enable/Disable Interrupt
    * Test & Set
    * ...

## Interprocess Communication (IPC)

* Message Passing 消息傳遞
* Shared Memory 共享記憶體
* Pipeline 管道
* 套接字
* RPC 遠程過程調用

### Message Passing

### Shared Memory

### Pipeline

## Classic Problem

* [Producer-Consumer](../../Lab/Lab3_SynchronizationMechanism/README.md#Producer-consumer-Problem-(Bounded-buffer-Problem))
* [Operating Systems Study Guide - 4.3. Classic Synchronization Problems](http://faculty.salina.k-state.edu/tim/ossg/IPC_sync/classic_problems.html)

## Resources

* [Wiki - Monitor (synchronization)](https://en.wikipedia.org/wiki/Monitor_(synchronization))

### Book

* Ch3 Processes
  * Ch3.4 Interprocess Communication
    * Ch3.4.1 Shared-Memory Systems
* Ch5 Process Synchronization
  * Ch5.2 The Critical-Section Problem
  * Ch5.3 Peterson's Solution
  * Ch5.4 Synchronization Hardware
  * Ch5.5 Mutex Locks
  * Ch5.6 Semaphores
  * Ch5.7 Classic Problems of Synchronization
    * Ch5.7.1 The Bounded-Buffer Problem
    * Ch5.7.2 The Readers-Writers Problem
    * Ch5.7.3 The Dining-Philosophers Problem
  * Ch5.8 Monitors
  * Ch5.9 Synchronization Examples
    * Ch5.9.2 Synchronization in Linux

Modern Operating Systems 4ed.

* Ch2.3 Interprocess Communication
  * Ch2.3.1 Race Conditinos
  * Ch2.3.2 Critical Regions
  * Ch2.3.3 Mutual Exclusion with Busy Waiting
  * Ch2.3.4 Sleep and Wakeup
  * Ch2.3.5 Semaphores
  * Ch2.3.6 Mutexes
  * Ch2.3.7 Monitors 管程
  * Ch2.3.8 Message Passing
  * Ch2.3.9 Barriers

### Library

* [Ice library - IceUtil::Monitor - The C++ Monitor Class](https://doc.zeroc.com/ice/3.7/language-mappings/c++98-mapping/the-c++98-utility-library/threads-and-concurrency-with-c++/the-c++-monitor-class)
