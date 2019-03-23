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

#### Infrastructure - The Bounded-buffer and Product

The shared memory between Threads, the buffer class object called `shared_buffer`.

```cpp
//----------------------------------------------------------------------
// Bounded buffer
//  Condumer must wait if the buffer is empty,
//  and the producer must wait if the buffer is full
//  (no malloc in Nachos?! so use define)
//----------------------------------------------------------------------

#define BUFFER_SIZE 10

class buffer {
    public:
        buffer() {
            fillCount = new Semaphore("Fill Count", 0);
            emptyCount = new Semaphore("Empty Count", BUFFER_SIZE);
            buffer_mutex = new Lock("Buffer mutex");
            count = 0;
        };
        ~buffer() {
            delete list;
        }
        void putItemIntoBuffer(product* item) {
            emptyCount->P(); // down
            buffer_mutex->Acquire();

            /* Critical Section */
            list[count++] = *item;
            /********************/

            buffer_mutex->Release();
            fillCount->V(); // up
        };
        product* removeItemFromBuffer() {
            fillCount->P(); // down
            buffer_mutex->Acquire();

            /* Critical Section */
            product* item = &list[count-- -1];
            /********************/

            buffer_mutex->Release();
            emptyCount->V(); // up

            return item;
        };
        void printBuffer() {
            printf("Buffer: [", BUFFER_SIZE, count);
            int i;
            for (i = 0; i < count; i++) {
                printf("%d, ", list[i].value);
            }
            for (; i < BUFFER_SIZE; i++) {
                printf("__, ");
            }
            printf("]\n");
        }
    private:
        int count;
        Lock* buffer_mutex;
        Semaphore* fillCount;
        Semaphore* emptyCount;
        product list[BUFFER_SIZE];
} *shared_buffer;
```

Product is simply a struct with a value.

```cpp
//----------------------------------------------------------------------
// Product
//  Product with value
//----------------------------------------------------------------------

typedef struct PRODUCT {
    int value;
} product;
```

#### Building the Problem

I've invoked `interrupt->OneTick()` to make system time moving forward.
So the random context switch (`-rs`) will work.

```cpp
//----------------------------------------------------------------------
// Produce Item
//  Generate prodoct with value
//----------------------------------------------------------------------

product*
produceItem(int value)
{
    printf("Producing item with value %d!!\n", value);
    product item;
    item.value = value;
    return &item;
}

//----------------------------------------------------------------------
// Consume Item
//  Delete product
//----------------------------------------------------------------------

void
consumeItem(product* item)
{
    printf("Consuming item with value %d!!\n", item->value);
}

//----------------------------------------------------------------------
// Producer
//  generate data, put it into the buffer, and start again. 
//----------------------------------------------------------------------

void
ProducerThread(int iterNum)
{
    for (int i = 0; i < iterNum; i++) {
        printf("## %s ##: ", currentThread->getName());
        product* item = produceItem(i);
        shared_buffer->putItemIntoBuffer(item);

        interrupt->OneTick();
    }
}

//----------------------------------------------------------------------
// Consumer
//  consuming the data, one piece at a time.
//----------------------------------------------------------------------

void
ConsumerThread(int iterNum)
{
    for (int i = 0; i < iterNum; i++) {
        printf("$$ %s $$: ", currentThread->getName());
        product* item = shared_buffer->removeItemFromBuffer();
        consumeItem(item);

        interrupt->OneTick();
    }
}
```

> Notes:
>
> 1. Because the mutex and semaphore is built in buffer. So `printBuffer()` may be interrupt and make the result much mess.
> 2. Only delete item when using linked list.

#### Testing the Problem

I've create 2 producer and 2 consumer. Each has will produce/consume the following amount of items.

* `Producer 1`: 8
* `Producer 2`: 7
* `Consumer 1`: 6
* `Consumer 2`: 9

And the calling order is `Producer 1` -> `Consumer 1` -> `Consumer 2` -> `Producer 2`

Add the following test in `threads/threadtest.cc` and as case 8.

```cpp
//----------------------------------------------------------------------
// Lab3 Exercise 4 Producer-consumer problem (Bounded-buffer problem)
//  The problem describes two processes, the producer and the consumer,
//  who share a common, fixed-size buffer used as a queue.
//  The producer's job is to generate data, put it into the buffer,
//  and start again. 
//  At the same time, the consumer is consuming the data
//  (i.e., removing it from the buffer), one piece at a time.
//  The problem is to make sure that the producer won't try to add data
//  into the buffer if it's full and that the consumer won't try to
//  remove data from an empty buffer.
//----------------------------------------------------------------------

void
Lab3ProducerConsumer()
{
    DEBUG('t', "Entering Lab3ProducerConsumer");

    shared_buffer = new buffer();

    Thread *producer1 = new Thread("Producer 1");
    Thread *producer2 = new Thread("Producer 2");
    Thread *consumer1 = new Thread("Consumer 1");
    Thread *consumer2 = new Thread("Consumer 2");

    producer1->Fork(ProducerThread, (void*)8);
    consumer1->Fork(ConsumerThread, (void*)6);
    consumer2->Fork(ConsumerThread, (void*)9);
    producer2->Fork(ProducerThread, (void*)7);

    currentThread->Yield(); // Yield the main thread
}
```

#### Result

Without random context switch

```sh
threads/nachos -q 8
```

```txt
Lab3 Exercise4: Producer-consumer problem (Bounded-buffer problem)
(add `-d c -rs` argument to show "Context Switch" and activate random timer
## Producer 1 ##: Producing item with value 0!!
## Producer 1 ##: Producing item with value 1!!
## Producer 1 ##: Producing item with value 2!!
## Producer 1 ##: Producing item with value 3!!
## Producer 1 ##: Producing item with value 4!!
## Producer 1 ##: Producing item with value 5!!
## Producer 1 ##: Producing item with value 6!!
## Producer 1 ##: Producing item with value 7!!
$$ Consumer 1 $$: Consuming item with value 7!!
$$ Consumer 1 $$: Consuming item with value 6!!
$$ Consumer 1 $$: Consuming item with value 5!!
$$ Consumer 1 $$: Consuming item with value 4!!
$$ Consumer 1 $$: Consuming item with value 3!!
$$ Consumer 1 $$: Consuming item with value 2!!
$$ Consumer 2 $$: Consuming item with value 1!!
$$ Consumer 2 $$: Consuming item with value 0!!
$$ Consumer 2 $$: ## Producer 2 ##: Producing item with value 0!!
## Producer 2 ##: Producing item with value 1!!
## Producer 2 ##: Producing item with value 2!!
## Producer 2 ##: Producing item with value 3!!
## Producer 2 ##: Producing item with value 4!!
## Producer 2 ##: Producing item with value 5!!
## Producer 2 ##: Producing item with value 6!!
Consuming item with value 6!!
$$ Consumer 2 $$: Consuming item with value 5!!
$$ Consumer 2 $$: Consuming item with value 4!!
$$ Consumer 2 $$: Consuming item with value 3!!
$$ Consumer 2 $$: Consuming item with value 2!!
$$ Consumer 2 $$: Consuming item with value 1!!
$$ Consumer 2 $$: Consuming item with value 0!!
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

```txt
Lab3 Exercise4: Producer-consumer problem (Bounded-buffer problem)
(add `-d c -rs` argument to show "Context Switch" and activate random timer)
## Producer 1 ##: Producing item with value 0!!
## Producer 1 ##: Producing item with value 1!!
## Producer 1 ##: Producing item with value 2!!
 << random Context Switch (stats->totalTicks = 190) >>
$$ Consumer 1 $$: Consuming item with value 2!!
$$ Consumer 1 $$:  << random Context Switch (stats->totalTicks = 280) >>
$$ Consumer 2 $$: ## Producer 2 ##: Producing item with value 0!!
## Producer 2 ##: Producing item with value 1!!
## Producer 2 ##: Producing item with value 2!!
## Producer 2 ##: Producing item with value 3!!
 << random Context Switch (stats->totalTicks = 460) >>
## Producer 1 ##: Producing item with value 3!!
## Producer 1 ##: Producing item with value 4!!
 << random Context Switch (stats->totalTicks = 580) >>
Consuming item with value 0!!
$$ Consumer 1 $$: Consuming item with value 4!!
$$ Consumer 1 $$: Consuming item with value 3!!
$$ Consumer 1 $$: Consuming item with value 2!!
$$ Consumer 1 $$:  << random Context Switch (stats->totalTicks = 780) >>
## Producer 1 ##: Producing item with value 5!!
Consuming item with value 1!!
Consuming item with value 0!!
 << random Context Switch (stats->totalTicks = 920) >>
## Producer 2 ##: Producing item with value 4!!
## Producer 2 ##: Producing item with value 5!!
## Producer 2 ##: Producing item with value 6!!
 << random Context Switch (stats->totalTicks = 1110) >>
$$ Consumer 2 $$: Consuming item with value 6!!
$$ Consumer 2 $$:  << random Context Switch (stats->totalTicks = 1210) >>
## Producer 1 ##: Producing item with value 6!!
 << random Context Switch (stats->totalTicks = 1260) >>
Consuming item with value 5!!
 << random Context Switch (stats->totalTicks = 1290) >>
## Producer 1 ##: Producing item with value 7!!
$$ Consumer 2 $$: Consuming item with value 7!!
$$ Consumer 2 $$:  << random Context Switch (stats->totalTicks = 1460) >>
 << random Context Switch (stats->totalTicks = 1490) >>
Consuming item with value 6!!
$$ Consumer 2 $$: Consuming item with value 5!!
$$ Consumer 2 $$:  << random Context Switch (stats->totalTicks = 1590) >>
Consuming item with value 4!!
$$ Consumer 2 $$:  << random Context Switch (stats->totalTicks = 1650) >>
Consuming item with value 3!!
$$ Consumer 2 $$: Consuming item with value 0!!
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

### Warning deprecated conversion from string constant to ‘char*’

> So many warning message makes me feel annoying..

* [How to get rid of `deprecated conversion from string constant to ‘char*’` warnings in GCC?](https://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warnin)

1. Passing `-Wno-write-strings` to gcc and g++
    > I've add it in `Makefile.common` in CFLAGS
2. Use `char const *` as the type instead of `char*`

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
