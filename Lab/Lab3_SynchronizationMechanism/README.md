# Lab 3: Synchronization Mechanism

## Exercise 1: Research

> Research the synchronize mechanism implement in Linux

## Exercise 2: Trace source code

> Read the following code, understand current Nachos synchronize mechanism
>
> * `code/threads/synch.h`
> * `code/threads/synch.cc`
> * `code/threads/synchlist.h`
> * `code/threads/synchlist.cc`

## Exercise 3: Implement (Mutex) Lock and Condtion Variable

> Either use primitive *sleep* and *wakeup* (notice to disable the system interrupt),
> or use *Semaphore* as the only *primitive* (then you won't need to handle interrupt by yourself)

## Exercise 4: Implement Synchronous Mutual Instance

> Based on Semaphore, (Mutex) Lock and Condition Variable. Use two different ways to implement synchronization and mutual mechanism application (one of them must using Condition Variable).
>
> Candidate list: (or any other classic concurrency problem)
>
> * [Producer–consumer problem](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)
> * [Readers–writers problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem)
> * [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
> * [Sleeping barber problem](https://en.wikipedia.org/wiki/Sleeping_barber_problem)
> * [Cigarette smokers problem](https://en.wikipedia.org/wiki/Cigarette_smokers_problem)

## Challenge 1: Implement Barrier

> You can use synchronization mechanism offered by Nachos (e.g. condition variable) to implement barrier.
> Such that the program can continue if and only if a certain amount of thread reach the same point.

* [Wiki - Barrier (computer science)](https://en.wikipedia.org/wiki/Barrier_(computer_science))

## Challenge 2: Read/Write Lock

> Based on lock (`synch.h` and `synch.cc`) made by Nachos.
> Implement read/write lock, such that a certain amount of thread can read the shared data at the same time.
> But can only be a single thread writing the shared data at a moment.

## Challenge 3: Implement Linux's kfifo

> Research if Linux's kfifo module can be merge into Nachos as a new module.

## Resources

* [Wiki - Concurrency (computer science)](https://en.wikipedia.org/wiki/Concurrency_(computer_science))

### Book

Operating System Concept 9ed.

* Ch5 Process Synchronization
  * Ch5.2 The Critical-Section Problem
  * Ch5.3 Peterson's Solution
  * Ch5.4 Synchronization Hardware
  * Ch5.5 Mutex Locks 互斥鎖（鎖）
  * Ch5.6 Semaphores 號誌（信號量）
  * Ch5.7 Classic Problems of Synchronization
    * Ch5.7.1 The Bounded-Buffer Problem
    * Ch5.7.2 The Readers-Writers Problem
    * Ch5.7.3 The Dining-Philosophers Problem
  * Ch5.8 Monitors
  * Ch5.9 Synchronization Examples
    * Ch5.9.2 Synchronization in Linux

Modern Operating Systems 4ed.

* Ch2.3 Interprocess Communication
  * Ch2.3.4 Sleep and Wakeup
  * Ch2.3.5 Semaphores
  * Ch2.3.6 Mutexes
  * Ch2.3.7 Monitors
  * Ch2.3.8 Message Passing
  * Ch2.3.9 Barriers

### Example

* [CSDN - 同步機制實驗報告_Nachos Lab3](https://blog.csdn.net/superli90/article/details/29376171)
* [CSDN - nachos lab3-線程同步](https://blog.csdn.net/wyxpku/article/details/52076209)
