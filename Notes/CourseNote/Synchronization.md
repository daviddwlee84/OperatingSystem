# Process Synchronization and Interprocess Communication

## Overview

Things to consider

* Parallelism
  * multiple process run at the same time
* Shared Data
  * if two process read or write a data at the same time
* Uncertainty
  * you can't not make sure when will the OS switch the process

Mechanism Implementation

```txt
Application    Parallel Processes
               (like the classic problem)

System         Synchronization Mechanism
               (Semaphore, Lock, Condition Varialbe)
               (Monitor (this is support by Language not the system))

Hardware       Atomic Operation
               (load/store, enable/disable interrupt, TSL/XCHG instruction)
```

![Synchronization in Linux](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/in-5_1.jpg)

> kernel preemption (i.e. interrupt)

### Process Parallelism

> All the problem is caused by "parallelism"

Some time a single line of C code. Is multiple instruction in Assembly. => If the execution *under some scenario* being interrupt *on specific step* then may cause error.

#### Comparison between sequential system and parallel system

Sequential system: only one process is running and it owns all the resources

### Two Main Problem Caused by Parallelism

#### Mutual Exclusion 互斥 - Shared Memory/Data (Interprocess Communication)

[Race Condition](#race-condition)

* [Wiki - Mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion)

#### Synchronization 同步

For multiple processes they have some kinds of "sequence dependency"

## Race Condition

Sometimes race conditions are also very difficult to reproduce (happen only in a sepecial executing order)

### Example of Spooling - Print Job Request

![Example of Spooling](https://images.slideplayer.com/20/6005967/slides/slide_2.jpg)

* [Wiki - Spooling](https://en.wikipedia.org/wiki/Spooling)

## Critical Section/Region

A critical section is a segment of code that access the critical resources (shared data).

* [Wiki - Critical section](https://en.wikipedia.org/wiki/Critical_section)

Overview

* Approach
  * Software => Programming skill
    * Dekker
    * Peterson
  * Hardware
    * Block Interrupt
    * TSL(XCHG) Instruction

### 4 Condition of Using Critical Region

* If no process is in the critical section, then anyone else can enter
* Only one process in the group can be allowed to execute in their critical section at any one time
  * If one process is already executing their critical section and another process wishes to do so, then the second process must be made to wait until the first process has completed their critical section work.
* The process not int the critical section should not block other process to get in the critical section
  * or will cause Deadlock
* Should not let a process wait for the critical section forever
  * this is Starvation

![process and critical region](http://cse.csusb.edu/tongyu/courses/cs460/images/process/mutual.png)

### 3 Condition must statisfy for a Solution

* Mutual Exclusion
  * Only one process at a time can be executing in their critical section.
* Progress
  * If no process is currently executing in their critical section, and one or more processes want to execute their critical section, then only the processes not in their remainder sections can participate in the decision, and the decision cannot be postponed indefinitely. (i.e. processes cannot be blocked forever waiting to get into their critical sections.)
* Bounded Waiting
  * There exists a limit as to how many other processes can get into their critical sections after a process requests entry into their critical section and before that request is granted. (i.e. a process requesting entry into their critical section will get a turn eventually, and there is a limit as to how many other processes get to go first.)

### Software Solutions

#### Attemption 1

```c
// Initial value
free = false;
```

```c
/* P and Q */

// if free == true: someone is in critical region so wait
while(free);
free = true;

// critical region

free = false;
```

Result: both P and Q are in critical region

#### Attemption 2

```c
// Initial value
pturn = false;
qturn = false;
```

```c
/* P */

while(!turn);

// critical region

turn = false;
```

```c
/* Q */

while(turn);

// critical region

turn = true;
```

Result: disobey the first condition (P, Q must take turn or they will be stuck by each other)

#### Attemption 3

```c
/* P */

pturn = true;
while(!qturn);

// critical region
// (condition to enter: pturn && !qturn)

pturn = false;
```

```c
/* Q */

qturn = true;
while(!pturn);

// critical region
// (condition to enter: !pturn && qturn)

qturn = false;
```

Result: they may both block outside the critical region (alter you problem)

#### Dekker Solution

> Add variable "turn" based on Attemption 3

```c
/* P */

pturn = true;
while(qturn){
    if(turn == 2){
        pturn = false;
        while(turn == 2);
        pturn = true;
    }
}

// critical region

turn = 2;
pturn = false;
```

```c
/* Q */

qturn = true;
while(pturn){
    if(turn == 1){
        qturn = false;
        while(turn == 1);
        qturn = true;
    }
}

// critical region

turn = 1;
qturn = false;
```

The "turn" variable will decide "which turn" when both processes want to let the other to go

#### Peterson Solution

> Consider a scenario that all the process want to get into critical region

```c
// Initial value
#define N 2  // total processes number
int turn;    // which process's turn (e.g. index 0, 1, 2, ...)
bool flag[N] = {false}; // any process want to enter the critical region, set ith flag to TRUE
```

![The structure of process Pi in Peterson's solution.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_02_Petersons.jpg)

(Current process index is `i` and Arbitrary other process index is `j`)

### Hardware Solution

#### Block Interrupt

```c
while(true){
    /* disable interrupts */
    /* critical section */
    /* enable interrupts */
    /* remainder */
}
```

* Pros and Cons
  * Pros
    * Simple
  * Cons
    * Limit the parallelism ability of CPU (ciritical region size)
    * Only work with single processor CPU (block interrupt only affect one processor)
    * Block interrupt is privilege instruction (can't be used by user process)

#### Special Instruction

> A special instruction that is "atomic" and doing more steps than normal instruction

**TSL**: Test and Set Lock

![illustrate "test_and_set( )" function](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_0304_TestAndSet.jpg)

1. Check if the lock is locked
   1. If locked (*target == true) then loop
2. If not locked (*target == false), then get the key and enter critical region

**XCHG**: Exchange Data

### Summary

* Software solution
* Hardware solution
* Busy waiting
  * [Spinlock](#implementation---spinlock)
* Priority inversion

Must make sure the critical section as small as possible

#### Busy Waiting

In single CPU system

* Must prevent

In multiple CPU system

* Sometimes works better
  * If all the critical region is very short
  * Don't need context switch (process stay on the CPU)

## Semaphore

A structure with two members

* `count` (`value`): how many resources left, a non-negative number
  * `count` = 1: binary semaphore
* `queue` (`list`): store the process which sleeps in this semaphore

and two operations

* `P()` (proberen ("test" in Dutch)) (`wait`): request the semaphore
* `V()` (verhogen ("increment" in Dutch)) (`signal`): release the semaphore

![Semaphore](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_Semaphore2.jpg)

### P, V Operation

> Primitive / Atomic Action => Uninterruptable

#### P (down, semWait)

> don't get mislead with wait() of the condition varialbe

* `count`--
* check if `count` < 0
  * if so then sleep/block the process, (change its state to `BLOCK`)
  * insert the process into the back of the `queue`
  * rescheduling other process

#### V (up, semSignal)

> don't get mislead with signal() of the condition varialbe

* `count`++
* check if `count` <= 0
  * if so then wakeup a process which waits in the `queue`
  * change its state to `RUNNABLE` and insert it to scheduler's ready queue

## Monitor

> Nowaday can be implemented as a class (it was been invented before object-oriented concept)

* the Shared Data is declare in the monitor object (also the Condition Varialbe)
* all the procedure is "mutual enter" originally

![Syntax of a monitor](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_15_Monitor.jpg)

* The processes which can't get into the monitor will wait on the entry queue
* The processes which can't get the "conditions" will wait on the consition variable

![A Complete Picture of Monitor with Condition Variables](https://pages.mtu.edu/~shene/NSF-3/e-Book/MONITOR/DIAGRAM-monitor-reenter.jpg)

![Monitor with condition variables](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_17_MonitorConditions.jpg)

[Monitor Implementation](#monitor-implementation)

* Monitor is a "Language-based" Mechanism
  * support by the language itself and guarantee by the compiler
* If a language don't have Monitor, then it can be "simulate"
  * with Semaphore
  * with Condition Variable + Mutex Lock in a Class
    * But if a language support, you should still use Condition Variable because Mutex Lock is to maintain the "mutual exclusion" condition

[Condition Variable Wait and Signal Operation](#wait-and-signal-operation)

### Consider Parallelism Problem

**Mutual Exclusion** => Maintain by Compiler (or can be simulate by a Mutex Lock)

**Synchronization** => Maintain by Programmer (use Wait and Signal operation on Condition Variable)

### Different Detail Monitor Implementation

* e: entry queue
* s: signal queue
* a: condition variable A
* b: condition variable B

The condition variable may have the following operation set

* Hoare monitor: wait/signal
* Mesa monitor: wait/notify

Solution (feature) of each monitor (consider two process P and Q, and P "wakeup" another process)

* Hoare monitor: P wait and Q run
* Mesa monitor: P continue and Q wait (but notified)
* Hansen monitor: wakeup the last runnable operation

#### Hoare Monitor

![Wiki A Hoare style monitor with two condition variables a and b](https://upload.wikimedia.org/wikipedia/commons/thumb/d/db/Monitor_%28synchronization%29-SU.png/200px-Monitor_%28synchronization%29-SU.png)

Signal(): *signal and wait*

#### Mesa Monitor

Signal() -> Notify(): *signal and continue*

Notify(): inform a process who wait in the condition variable to queue in entry queue but keep continue running in the same process.

Note that in entry queue not means it guarantee the process can run, thus must recheck the condition (use while loop)

> Which will save two context switch compare with Signal()

![Wiki Mesa style monitor](https://upload.wikimedia.org/wikipedia/commons/thumb/1/15/Monitor_%28synchronization%29-Mesa.png/200px-Monitor_%28synchronization%29-Mesa.png)

> Hoare vs. Mesa Monitor
>
> * Mesa monitor will have less error

#### Hansen Monitor

> [Concurrent Pascal](https://en.wikipedia.org/wiki/Concurrent_Pascal)

### Solving Producer-Consumer Problem with Monitor

> need "while" in Mesa monitor

### Monitor Implementation

* Directly construct monitor -> Efficient but need support of programming language
* Indirectly construct monitor -> Use existing synchronization mechanism to implement
  * Semaphore
  * Condition Variable + Lock

#### Monitor in Java

![Wiki A Java style monitor](https://upload.wikimedia.org/wikipedia/commons/thumb/f/f5/Monitor_%28synchronization%29-Java.png/200px-Monitor_%28synchronization%29-Java.png)

## Mutex Lock and Condition Variable

> Pthread is a **user space** library. So you should assume OS don't know the thread that you're blocking.

[More notes about pthreads](../../Lab/Lab3_SynchronizationMechanism/README.md#Pthreads)

### Mutex Lock

The naive version of a Lock has two operations

* acquire() / lock(): request a lock
* release() / unlock(): release a lock

#### Why implement pthread_mutex_trylock

In some situation we just try if we can get the lock. If use trylock() but not get the lock, it will not wait on lock, sometimes it is better (higher performance).

#### Difference between a Lock and a Binary Semaphore

The Lock can only release by the process which acquired it beforehand. (the owner of the Lock)

But semaphore can be V() or Signal() by other process.

#### Implementation - Disable Interrupt + Sleep

If a process can't get the lock then sleep

#### Implementation - Spinlock

If a process can't get the lock then wait on the lock (while loop) => Busy waiting

### Condition Variable

Procedure

1. release a lock: in order not sleep with a lock
2. wait() until receive a signal() signal (by signal() or broadcast())
3. reacquire a lock

#### Wait and Signal and Broadcast Operation

* wait(): wait/sleep/block on a condition
* signal(): wakeup on a condition
* boradcast(): wakup all the process which wait in the queue

> notify() in [Mesa monitor](#Mesa-monitor)

#### Why use While instead of If on pthread_cond_wait

spurious wakeup: wakeup (signal) a process but the condition is not valid actually

* [Stackoverflow - Why do you need a while loop while waiting for a condition variable](https://stackoverflow.com/questions/7766057/why-do-you-need-a-while-loop-while-waiting-for-a-condition-variable)
* [Wiki - Spurious wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)

#### Semaphore vs. Condition Variable Operations

> wait() and signal() in semaphore is same as the P() and V() operation

Semaphores|Condition Variables
-|-
Can be used anywhere in a program, but should not be used in a monitor|Can only be used in monitors
Wait() does not always block the caller (i.e., when the semaphore counter is greater than zero).|Wait() always blocks the caller.
Signal() either releases a blocked thread, if there is one, or increases the semaphore counter.|Signal() either releases a blocked thread, if there is one, or the signal is lost as if it never happens.
If Signal() releases a blocked thread, the caller and the released thread both continue.|If Signal() releases a blocked thread, the caller yields the monitor (Hoare type) or continues (Mesa Type). Only one of the caller or the released thread can continue, but not both.

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
  * [wiki](https://en.wikipedia.org/wiki/Message_passing)
* Shared Memory 共享記憶體
* Pipeline 管道
* Sockets 套接字
  * [wiki](https://en.wikipedia.org/wiki/Network_socket)
* Remote Procedure Calls, RPC 遠程過程調用

![Communications models: (a) Message passing. (b) Shared memory](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter3/3_12_CommunicationsModels.jpg)

> (a) Message passing. (b) Shared memory

### Message Passing

1. Trap into kernel (the buffer is in the kernel space)
2. Send() and copy message into the buffer
3. Pass the message from the buffer into the information queue in the destination process's PCB
4. Receive() and copy the message from the buffer

Message Buffer/Queue structure

* Message header
  * message type
  * sending process PID
  * receiving process PID
  * message size
  * control information
* Message body
  * content

#### Send and Receive Operations

* send(destination, message)
* receive(source, message)

#### Buffering

* **Zero capacity** - Messages cannot be stored in the queue, so senders must block until receivers accept the messages.
* **Bounded capacity**- There is a certain pre-determined finite capacity in the queue. Senders must block if the queue is full, until space becomes available in the queue, but may be either blocking or non-blocking otherwise.
* **Unbounded capacity** - The queue has a theoretical infinite capacity, so senders are never forced to block.

### Shared Memory

> e.g. global variable

### Pipeline

Passing one's stdout as another's stdin

> `echo "haha" | cowsay`

![pipe](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter3/3_22_PipeFileDescriptors.jpg)

### Socket

![Communication using sockets](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter3/3_20_Sockets.jpg)

* Connection-oriented (**TCP**, Transmission Control Protocol)
  * connections emulate a telephone connection
* Connectionless (**UDP**, User Datagram Protocol)
  * emulate individual telegrams

### Remote Procedure Call

![RPC](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter3/3_23_RPC.jpg)

## Classic Problem

> Sometimes a problem can be "reduced" to the classic problem. Just keep an eye on the scenario.

* [Producer-Consumer](../../Lab/Lab3_SynchronizationMechanism/README.md#Producer-consumer-Problem-(Bounded-buffer-Problem))
* [Reading Room](../../Practice/Synchronization/ClassicalProblem/ReadingRoomMonitor.md)
* [Many more problem](../../Practice/Synchronization/ClassicalProblem/OtherProblems.md)
* [Operating Systems Study Guide - 4.3. Classic Synchronization Problems](http://faculty.salina.k-state.edu/tim/ossg/IPC_sync/classic_problems.html)

## Resources

* [Wiki - Monitor (synchronization)](https://en.wikipedia.org/wiki/Monitor_(synchronization))
* [CSE 460 - Interprocess Communication ( IPC )](http://cse.csusb.edu/tongyu/courses/cs460/notes/interprocess.php)

### Book

Operating System Concepts 9ed.

* Ch3 Processes
  * Ch3.4 Interprocess Communication
    * Ch3.4.1 Shared-Memory Systems
    * Ch3.4.2 Message-Passing Systems
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
* Notes
  * [Process](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/3_Processes.html)
  * [Process Synchronization](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/5_Synchronization.html)

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

* [Multithreaded Programming with ThreadMentor: A Tutorial](https://pages.mtu.edu/~shene/NSF-3/e-Book/index.html)
  * [Condition Variables](https://pages.mtu.edu/~shene/NSF-3/e-Book/MONITOR/CV.html)
  * [Monitor Types](https://pages.mtu.edu/~shene/NSF-3/e-Book/MONITOR/monitor-types.html)
  * [Semaphore vs. Monitor](https://pages.mtu.edu/~shene/NSF-3/e-Book/MONITOR/sema-vs-monitor.html)

### Library

* [Ice library - IceUtil::Monitor - The C++ Monitor Class](https://doc.zeroc.com/ice/3.7/language-mappings/c++98-mapping/the-c++98-utility-library/threads-and-concurrency-with-c++/the-c++-monitor-class)
