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

Nachos has implemented Semaphore in `threads/synch.h`

```c
class Semaphore {
  public:
    void P();	 // these are the only operations on a semaphore
    void V();	 // they are both *atomic*

  private:
    int value;         // semaphore value, always >= 0
    List *queue;       // threads waiting in P() for the value to be > 0
};
```

Parameter

* `int value`: threshold
* `List *queue`: threads which are waiting for this semaphore

(Atomic) Operation

* P()
  * When `value` == 0
    * Put current Thread into waiting queue
    * Sleep current Thread and switch to other Thread
  * When `value` > 0
    * `value--`
* V()
  * If there is a Thread waiting for this semaphore
    * Pick one up and set to READY state
  * `value++`

## Exercise 3: Implement (Mutex) Lock and Condtion Variable

> Either use primitive *sleep* and *wakeup* (notice to disable the system interrupt),
> or use *Semaphore* as the only *primitive* (then you won't need to handle interrupt by yourself)

### 3-1 Lock

> The Binary Semaphore with Ownership Concept!

Nachos has a initial template for Lock

```cpp
class Lock {
  public:
    Lock(char* debugName);  		// initialize lock to be FREE
    ~Lock();				// deallocate lock
    char* getName() { return name; }	// debugging assist

    void Acquire(); // these are the only operations on a lock
    void Release(); // they are both *atomic*

    bool isHeldByCurrentThread();	// true if the current thread
					// holds this lock.  Useful for
					// checking in Release, and in
					// Condition variable ops below.

  private:
    char* name;				// for debugging
    // plus some other stuff you'll need to define
};
```

I use `Semaphore` to implement it.

It will be much easier because I don't have to handle *interrupt* and *sleeping thread* problem.

> I was tried to implement Lock from scratch.
> But I found I was making "spinlock" which may lead deadlock or starvation maybe.
> So I'm using semaphore to avoid *sleepgin thread* problem...

I've created two private variable.

```cpp
class Lock {
  private:
    Thread* holderThread; // Thread which is holding this lock
    Semaphore* semaphore; // Use semaphore to implement lock
};
```

And the most important thing is implementing Lock using **Semaphore with the value of 1**! (**binary semaphore**)

```cpp
//----------------------------------------------------------------------
// Lock::Lock
// 	Initialize a lock, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------
Lock::Lock(char* debugName)
{
    name = debugName;
    semaphore = new Semaphore("Lock", 1);
}
```

Assign the `holderThread` to `currentThread` when the `currentThread`
has got the Lock successfully.

```cpp
//----------------------------------------------------------------------
// Lock::Acquire
// 	Acquire Mutex Lock.
//	* When lock is BUSY, enter SLEEP state. (if loop and wait => spinlock)
//	* When lock is FREE, current Thread get lock and keep running.
//----------------------------------------------------------------------
void
Lock::Acquire()
{
    DEBUG('s', "Lock \"%s\" Acquired by Thread \"%s\"\n", name, currentThread->getName());
    semaphore->P();
    holderThread = currentThread;
}
```

Release the Lock only when the `currentThread` is the owner itself.

> This is the biggest difference between a mutex lock and a binary semaphore!
> Because lock has *ownership concept*.
>
> (Note: Ownership means that mutex can only be "incremented" back (set to 1) by the same process that "decremented" it (set to 0), and all other tasks wait until mutex is available for decrement (effectively meaning that resource is available), which ensures mutual exclusivity and avoids deadlock.)

```cpp
//----------------------------------------------------------------------
// Lock::Release
// 	Release Mutex Lock.
//  (Note: Only the Thread which own this lock can release lock)
//	Set lock status to FREE. If any other Thread is waiting this lock,
//  wake one of them up, enter READY state.
//----------------------------------------------------------------------
void
Lock::Release()
{
    DEBUG('s', "Lock \"%s\" Released by Thread \"%s\"\n", name, currentThread->getName());
    // make sure the owner of this lock is currentThread
    ASSERT(this->isHeldByCurrentThread());
    holderThread = NULL;
    semaphore->V();
}
```

Other trivial code implementation just checkout `threads/synch.cc`.

And the test of `Lock` I'll using Exercise 4 as an example.

### 3-2 Condtion Variable

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

### Producer-consumer Problem (Bounded-buffer Problem)

#### Infrastructure

#### Building the Problem

#### Result

Without random context switch

```sh
threads/nachos -q 8
```

With random context switch

> I've add the debug message in `threads/system.cc` the `TimerInterruptHandler`.
> So you can use `-d c` to show the debug message.
>
> ```cpp
> DEBUG('c', " << random Context Switch (stats->totalTicks = %d) >>\n", stats->totalTicks);
> ```

```sh
threads/nachos -d c -rs -q 8
```

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

## Trouble Shooting

### C struct vs. typedef struct

* [**Stackoverflow - typedef struct vs struct definitions**](https://stackoverflow.com/questions/1675351/typedef-struct-vs-struct-definitions)
* [Stackoverflow - Why should we typedef a struct so often in C?](https://stackoverflow.com/questions/252780/why-should-we-typedef-a-struct-so-often-in-c)

Struct

* [GeeksforGeeks - Structures in C](https://www.geeksforgeeks.org/structures-c/)

### Crosses initialization of Class object and Struct

* [programmertech - Resolve C++ cross initialization error in switch case](http://programmertech.com/techtip/resove-cross-initialization-error-in-c-plus-plus)
* [Stackoverflow - Getting a bunch of crosses initialization error](https://stackoverflow.com/a/11578973/3682187)

#### Class

In C++ when we declare & initialize variable in switch case directly except first case then c++ compiler throws **jump to case label crosses initialization** error; because cases considered as jump & c++ does not create scope for switch cases except first switch case; to resolve this error we have two options.

1. Declare & initialize variable before switch and reassign value to that variable inside switch case.
2. Declare & initialize variable inside curley braces {} or block and used within that block in switch case. this block of code is now create scope within switch cases i.e. variable scope is inside that block only when block completes then scope of variable finished.

#### Struct

```cpp
struct PRODUCT item;

// X
case 8:
  struct PRODUCT *new_item = &item;

// O
case 8:
  struct PRODUCT *new_item
  new_item = &item;
```

### SIGSEGV SIGV(-11) Signal 11

**Segmentation Fault**!

> It may caused by you declare a struct pointer and then you access it.
> You should declare a struct normally. And pass it with `&` if you wan't to pass in address.

## Resources

* [Wiki - Concurrency (computer science)](https://en.wikipedia.org/wiki/Concurrency_(computer_science))

### Book

Operating System Concept 9ed.

* Ch3 Processes
  * Ch3.4 Interprocess Communication
    * Ch3.4.1 Shared-Memory Systems => Producer-consumer bounded buffer
* Ch5 Process Synchronization
  * Ch5.1 Background => Producer-consumer
  * Ch5.2 The Critical-Section Problem
  * Ch5.3 Peterson's Solution
  * Ch5.4 Synchronization Hardware
  * Ch5.5 Mutex Locks 互斥鎖（鎖）
  * Ch5.6 Semaphores 號誌（信號量）
  * Ch5.7 Classic Problems of Synchronization
    * Ch5.7.1 The Bounded-Buffer Problem => Producer-consumer semaphore
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
