# Lab 3: Synchronization Mechanism

## Exercise 1: Research

> Research the synchronize mechanism implement in Linux

* [Chapter 3. Locking in the Linux Kernel](https://www.kernel.org/doc/htmldocs/kernel-locking/locks.html)

### mutex

* [`include/linux`]
  * [`mutex.h`](https://github.com/torvalds/linux/blob/master/include/linux/mutex.h)
  * [`rwsem-spinlock.h`](https://github.com/torvalds/linux/blob/master/include/linux/rwsem-spinlock.h)
  * [`bit_spinlock.h`](https://github.com/torvalds/linux/blob/master/include/linux/bit_spinlock.h)
  * [`hwspinlock.h`](https://github.com/torvalds/linux/blob/master/include/linux/hwspinlock.h)

In `mutex.h` line 53

```c
struct mutex {
	atomic_long_t		owner;
	spinlock_t		wait_lock;
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
	struct optimistic_spin_queue osq; /* Spinner MCS lock */
#endif
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_MUTEXES
	void			*magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};
```

* [`kernel/locking`](https://github.com/torvalds/linux/tree/master/kernel/locking)
  * [`mutex.c`](https://github.com/torvalds/linux/blob/master/kernel/locking/mutex.c)

Acquire the mutex (in `mutex.c` line 252)

```c
void __sched mutex_lock(struct mutex *lock)
{
	might_sleep();

	if (!__mutex_trylock_fast(lock))
		__mutex_lock_slowpath(lock);
}
```

```c
static int __sched
__mutex_lock(struct mutex *lock, long state, unsigned int subclass,
	     struct lockdep_map *nest_lock, unsigned long ip)
{
	return __mutex_lock_common(lock, state, subclass, nest_lock, ip, NULL, false);
}

static int __sched
__ww_mutex_lock(struct mutex *lock, long state, unsigned int subclass,
		struct lockdep_map *nest_lock, unsigned long ip,
		struct ww_acquire_ctx *ww_ctx)
{
	return __mutex_lock_common(lock, state, subclass, nest_lock, ip, ww_ctx, true);
}
```

Every called the `__mutex_lock_common` function (in `mutex.c` line 899)

### Other lock

* rwlock
* Condition
* spinlock
* Hardware related lock...

### Conclusion

Linux made a lots of different locks for its kernel. Typically, it classified into software and hardware usage.

But for user/programmer, I found in the most case they use Pthread library.

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

* `P()`
  * When `value` == 0
    * Put current Thread into waiting queue
    * Sleep current Thread and switch to other Thread
  * When `value` > 0
    * `value--`
* `V()`
  * If there is a Thread waiting for this semaphore
    * Pick one up and set to READY state
  * `value++`

## Exercise 3: Implement (Mutex) Lock and Condition Variable

> Either use primitive *sleep* and *wakeup* (notice to disable the system interrupt),
> or use *Semaphore* as the only *primitive* (then you won't need to handle interrupt by yourself)

Disable interrupt in the beginning and re-enable it in the end to make the
routine *atomic* or make it become *primitive*.

```c
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled

IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
(void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
```

### Pthreads

> Pthreads offers two synchronization mechanism: **mutex** and **condition variable**

* POSIX Threads Programming
  * [Mutex Variables](https://computing.llnl.gov/tutorials/pthreads/#Mutexes)
  * [Condition Variables](https://computing.llnl.gov/tutorials/pthreads/#ConditionVariables)

Some of the Pthreads calls relating to mutexes

* `Pthread_mutex_init`: Create a mutex
* `Pthread_mutex_destroy`: Destroy an existing mutex
* `Pthread_mutex_lock`: Acquire a lock or block
* `Pthread_mutex_trylock`: Acquire a lock or fail
* `Pthread_mutex_unlock`: Release a lock

Some of the Pthreads calls relating to condition variables

* `Pthread_cond_init`: Create a condition variable
* `Pthread_cond_destroy`: Destroy a condition variable
* `Pthread_cond_wait` (primary): Block waiting for a signal
  * Using a WHILE loop instead of an IF statement to check the waited for condition can help deal with several potential problems
    * If several threads are waiting for the same wake up signal, they will take turns acquiring the mutex, and any one of them can then modify the condition they all waited for.
    * If the thread received the signal in error due to a program bug.
    * The Pthreads library is permitted to issue spurious wake ups to a waiting thread without violating the standard.
* `Pthread_cond_signal` (primary): Signal another thread and wake it up
* `Pthread_cond_broadcast`: Signal multiple threads and wake all of them
  * Called when there are multiple threads potentially all blocked and waiting for the same signal

Example

* [Oracle Multithreaded Programming Guide - Chapter 4 Programming with Synchronization Objects](https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032m/index.html)
  * [Using Mutual Exclusion Locks](https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032p/index.html)
  * [Using Condition Variables](https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html)
    * [The Producer/Consumer Problem (using both Pthreads mutex and condition variable)](https://docs.oracle.com/cd/E19455-01/806-5257/sync-31/index.html)

### 3-1 Lock

> The Binary Semaphore with Ownership Concept!

Nachos has a initial template for Lock. And I use `Semaphore` to implement it.

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
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    DEBUG('s', "Lock \"%s\" Acquired by Thread \"%s\"\n", name, currentThread->getName());
    semaphore->P();
    holderThread = currentThread;

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
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
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    DEBUG('s', "Lock \"%s\" Released by Thread \"%s\"\n", name, currentThread->getName());
    // make sure the owner of this lock is currentThread
    ASSERT(this->isHeldByCurrentThread());
    holderThread = NULL;
    semaphore->V();

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

Other trivial code implementation just checkout `threads/synch.cc`.

And the test of `Lock` I'll using Exercise 4 as an example.

### 3-2 Condition Variable

> Condition variables should be used as a place to wait and be notified.
> They are not the condition itself and they are not events.
> The condition is contained in the surrounding programming logic.

Nachos has a initial template for Condition. And I use `Lock` (Exercise 3-1) to implement it.

I've create a *waiting queue* as a private variable.

```cpp
class Condition {
  private:
    List* waitQueue; // Waiting queue for the Thread blocked by this condition
};
```

Notes that all the condition operation has a input of `conditionLock`. That's because the Thread which is using the condition must have held the lock.

In addition, when calling `Condition::Wait()`, the lock must be locked. And wait must be wrapped with a loop.

> BUT in Nachos, warp wait in a loop will CAUSE SERIOUS PROBLEM that `Condition::Wait()` will sleep/block the threads again right after they have been waken up. So don't do that (i.e. in [Barrier](#Implementation---The-Barrier-class)).

```cpp
//----------------------------------------------------------------------
// Condition::Wait
//  Wait blocks the calling thread until the specified condition is signalled.
//  This routine should be called while mutex is locked, and it will
//  automatically release the mutex while it waits.
//  After signal is received and thread is awakened,
//  mutex will be automatically locked for use by the thread.
//  The programmer is then responsible for unlocking mutex when the thread
//  is finished with it.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Wait(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the currentThread
    ASSERT(conditionLock->isHeldByCurrentThread())
    // conditionLock must be locked
    ASSERT(conditionLock->isLocked());

    // 1. Release the lock while it waits
    conditionLock->Release();

    // 2. Append into waitQueue and sleep
    waitQueue->Append(currentThread);
    currentThread->Sleep();

    // Awake by Signal...

    // 3. Reclaim lock while awake
    conditionLock->Acquire();

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

We have two operation that can wake up the Thread which is waiting for this condition. `Condition::Signal()` to wakeup single Thread; `Condition::Broadcast` to wakeup all the Thread (in waitQueue)

```cpp
//----------------------------------------------------------------------
// Condition::Signal
//  Signal is used to signal (or wake up) another thread which is waiting
//  on the condition variable. It should be called after mutex is locked,
//  and must unlock mutex in order for Condition::Wait() routine to complete.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Signal(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the current Thread
    ASSERT(conditionLock->isHeldByCurrentThread())

    if (!waitQueue->IsEmpty()) {
        // Putting thread from the front of waitQueue onto ready list
        Thread* thread = (Thread*) waitQueue->Remove();
        scheduler->ReadyToRun(thread);
    }

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Condition::Broadcast
//  Wakeup all the threads waiting on this condition.
//  Brodcast should be used instead of Condition::Signal() if more than
//  one thread is in a blocking wait state.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Broadcast(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the current Thread
    ASSERT(conditionLock->isHeldByCurrentThread())

    DEBUG('b', "Condition \"%s\" Broadcasting: ", name);
    while (!waitQueue->IsEmpty()) {
        // Putting all the threads on ready list
        Thread* thread = (Thread*) waitQueue->Remove();
        DEBUG('b', "Thread \"%s\", ", thread->getName());
        scheduler->ReadyToRun(thread);
    }
    DEBUG('b', "\n");

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

Other trivial code implementation just checkout `threads/synch.cc`.

And the test of `Lock` I'll using Challenge 1 as an example.

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

> Basically followed the pseudocode in Wikipedia.

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

### Background Knowledge

* [Wiki - Barrier (computer science)](https://en.wikipedia.org/wiki/Barrier_(computer_science))
* [Latches And Barriers](http://www.modernescpp.com/index.php/latches-and-barriers)
* [Stackoverflow - What is the best way to realize a synchronization barrier between threads](https://stackoverflow.com/questions/38999911/what-is-the-best-way-to-realize-a-synchronization-barrier-between-threads)

Pthreads

* Pthreads barriers (with prefix `pthread_barrier_`)
  * `pthread_barrier_destroy`
  * `pthread_barrier_init`
  * `pthread_barrier_wait`
  * `pthread_barrierattr_destroy`
  * `pthread_barrierattr_getpshared`
  * `pthread_barrierattr_init`
  * `pthread_barrierattr_setpshared`

C++ Standard Library

* [`std::latch`](https://en.cppreference.com/w/cpp/experimental/latch)
  * Unlike `std::barrier` can be decremented by a participating thread more than once.
* [`std::barrier`](https://en.cppreference.com/w/cpp/experimental/barrier)
* [`std::flex_barrier`](https://en.cppreference.com/w/cpp/experimental/flex_barrier)

Example using Pthread mutex and condition varialbe

* [angrave/SystemProgramming - Synchronization, Part 6: Implementing a barrier](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-6:-Implementing-a-barrier)
  * [angrave/SystemProgramming Wiki](https://github.com/angrave/SystemProgramming/wiki)

Example using Pthread barrier

* [angrave/SystemProgramming - Sample program using pthread barriers](https://github.com/angrave/SystemProgramming/wiki/Sample-program-using-pthread-barriers)

### Implementation - The Barrier class

I've imitiate the `arrive_and_wait` of `std::barrier` and build a Barrier class

```cpp
class Barrier {
  public:
    Barrier(char* debugName, int num); // initialize barrier
    ~Barrier(); // deallocate the barrier
    char* getName() { return (name); } // debugging assist

    void ArrivedAndWait(); // Sleep the Thread until all the Threads arrived

  private:
    char* name;             // useful for debugging
    int remain;             // How many Threads have not arrived
    int num_threads;        // Total Threads
    Lock* mutex;            // Lock for "remain"
    Condition* condition;   // Used to sleep the Thread and wake them up
};
```

The idea is simple.

If there is anyone haven't reach the barrier (remain > 0). Sleep the current Thread (using Condition)

Else broadcast to wake everyone up (who sleeping in the Condition). (And reset the barrier).

```cpp
//----------------------------------------------------------------------
// Barrier::ArrivedAndWait
// 	Allows a single thread to indicate that it has arrived at a synchronization point.
//  The thread will block until the synchronization condition has been reached.
//  May be called repeatedly by a given thread.
//----------------------------------------------------------------------

void
Barrier::ArrivedAndWait()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // Use mutex to ensure that only one thread modifies remain at a time
    mutex->Acquire();
    remain--;
    DEBUG('b', "Thread %s enter barrier %s with remain=%d\n", currentThread->getName(), name, remain);
    if (remain == 0) {
        DEBUG('b', "Everyone reached the Barrier!!\n");
        condition->Broadcast(mutex);
        // Reset barrier
        remain = num_threads;
    } else {
        // while (remain != 0) // While will sleep the waken Thread sleep again!
        condition->Wait(mutex);
    }
    mutex->Release();

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

Other trivial code implementation just checkout `threads/synch.cc`.

### Testing

I've made the following scenario

* Shared memory: `matrix[24][10]`
* 4 Threads. Each thread manipulate 6 rows (24/4).

There are three parts of calculation. Each of them will wait at the barrier until every body complete.

1. Threads do first calculation
   * `matrix[x][y] += (x+1)*(y+1)`
2. Barrier!
3. Threads do second calculation
   * `matrix[x][y] /= startRow+1`
4. Barrier!
5. Threads do third calculation
   * `matrix[x][y] -= startRow/N_ROWS`

Because the code is quite long.
Checkout `Lab3Barrier()` and `CalcThread` in `threads/threadtest.cc`.
(And it's as case 9)

### Result with debug flag

> `-d b` for debugging message related to barrier

```txt
$ threads/nachos -d b -q 9
Lab3 Challenge1: Barrier
main() is ready.
Thread "Calc 1" finish First Calculation
Thread Calc 1 enter barrier Matrix Calc with remain=4
Thread "Calc 2" finish First Calculation
Thread Calc 2 enter barrier Matrix Calc with remain=3
Thread "Calc 3" finish First Calculation
Thread Calc 3 enter barrier Matrix Calc with remain=2
Thread "Calc 4" finish First Calculation
Thread Calc 4 enter barrier Matrix Calc with remain=1
Thread main enter barrier Matrix Calc with remain=0
Everyone reached the Barrier!!
Condition "Barrier Condition" Broadcasting: Thread "Calc 1", Thread "Calc 2", Thread "Calc 3", Thread "Calc 4", 
main() is going!
Thread "Calc 1" finish Second Calculation
Thread Calc 1 enter barrier Matrix Calc with remain=4
Thread "Calc 2" finish Second Calculation
Thread Calc 2 enter barrier Matrix Calc with remain=3
Thread "Calc 3" finish Second Calculation
Thread Calc 3 enter barrier Matrix Calc with remain=2
Thread "Calc 4" finish Second Calculation
Thread Calc 4 enter barrier Matrix Calc with remain=1
Thread main enter barrier Matrix Calc with remain=0
Everyone reached the Barrier!!
Condition "Barrier Condition" Broadcasting: Thread "Calc 1", Thread "Calc 2", Thread "Calc 3", Thread "Calc 4", 
main() is going again!
Thread "Calc 1" finish Third Calculation
Thread "Calc 2" finish Third Calculation
Thread "Calc 3" finish Third Calculation
Thread "Calc 4" finish Third Calculation
Result of data:
 1.00  2.00  3.00  4.00  5.00  6.00  7.00  8.00  9.00 10.00 
 2.00  4.00  6.00  8.00 10.00 12.00 14.00 16.00 18.00 20.00 
 3.00  6.00  9.00 12.00 15.00 18.00 21.00 24.00 27.00 30.00 
 4.00  8.00 12.00 16.00 20.00 24.00 28.00 32.00 36.00 40.00 
 5.00 10.00 15.00 20.00 25.00 30.00 35.00 40.00 45.00 50.00 
 6.00 12.00 18.00 24.00 30.00 36.00 42.00 48.00 54.00 60.00 
 0.00  1.00  2.00  3.00  4.00  5.00  6.00  7.00  8.00  9.00 
 0.14  1.29  2.43  3.57  4.71  5.86  7.00  8.14  9.29 10.43 
 0.29  1.57  2.86  4.14  5.43  6.71  8.00  9.29 10.57 11.86 
 0.43  1.86  3.29  4.71  6.14  7.57  9.00 10.43 11.86 13.29 
 0.57  2.14  3.71  5.29  6.86  8.43 10.00 11.57 13.14 14.71 
 0.71  2.43  4.14  5.86  7.57  9.29 11.00 12.71 14.43 16.14 
-1.00  0.00  1.00  2.00  3.00  4.00  5.00  6.00  7.00  8.00 
-0.92  0.15  1.23  2.31  3.38  4.46  5.54  6.62  7.69  8.77 
-0.85  0.31  1.46  2.62  3.77  4.92  6.08  7.23  8.38  9.54 
-0.77  0.46  1.69  2.92  4.15  5.38  6.62  7.85  9.08 10.31 
-0.69  0.62  1.92  3.23  4.54  5.85  7.15  8.46  9.77 11.08 
-0.62  0.77  2.15  3.54  4.92  6.31  7.69  9.08 10.46 11.85 
-2.00 -1.00  0.00  1.00  2.00  3.00  4.00  5.00  6.00  7.00 
-1.95 -0.89  0.16  1.21  2.26  3.32  4.37  5.42  6.47  7.53 
-1.89 -0.79  0.32  1.42  2.53  3.63  4.74  5.84  6.95  8.05 
-1.84 -0.68  0.47  1.63  2.79  3.95  5.11  6.26  7.42  8.58 
-1.79 -0.58  0.63  1.84  3.05  4.26  5.47  6.68  7.89  9.11 
-1.74 -0.47  0.79  2.05  3.32  4.58  5.84  7.11  8.37  9.63 
```

## Challenge 2: Read/Write Lock

> Based on lock (`synch.h` and `synch.cc`) made by Nachos.
> Implement read/write lock, such that a certain amount of thread can read the shared data at the same time.
> But can only be a single thread writing the shared data at a moment.

The Reader-writer lock is a solution for readers-writers problem.
And it has been generalized as a special lock on some system.

* [**Synchronization, Part 7: The Reader Writer Problem**](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-7%3A-The-Reader-Writer-Problem)
* [Wiki - Readers–writer lock](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock) - I've followed the pseudocode of first implementation (Using two mutexes)
* [Wiki - Readers–writers problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem)

Pthreads

* `pthread_rwlock`
  * `pthread_rwlock_init`
  * `pthread_rwlock_destroy`
  * `pthread_rwlock_tryrdlock`
  * `pthread_rwlock_timedrdlock`
  * `pthread_rwlock_rdlock`
  * `pthread_rwlock_timedrdlock`
  * `pthread_rwlock_tryrdlock`
  * `pthread_rwlock_trywrlock`
  * `pthread_rwlock_timedwrlock`
  * `pthread_rwlock_wrlock`
  * `pthread_rwlock_unlock`

There are some different readers-writers problem

1. The first readers-writers problem (simplest)
    * Requires that no reader be kept waiting unless a writer has already obtained permission to use the shared object.
    * i.e. No reader should wait for other readers to finish simply because a writer is waiting.
2. The second readers-writers problem
    * Requires that once a writer is ready, that writer perform its write as soon as possible.
    * i.e. If a writer is waiting to access the object, no new readers may start reading.

A soluiton to either problem may result in starvation

1. Writers may starve (read-preferring solution)
2. Readers may starve (write-preferring solution)

In application

* It's easy to identify which processes only read shared data and which processes only write shared data.
* Usually have more readers than writers.

### The read-preferring Readers-writer Lock

Acquiring a reader-writer lock requires specifying the mode of the lock: either *read* or *write* access.

I'm using two mutexes to implemet this.

Note that, one of the mutex is only used by readers. The other one is a "global" mutual exclusion of writer. This requires that a mutex *acquired by one thread can be released by another*. (So I'll use binary simaphore here)

```cpp
class ReaderWriterLock {
  public:
    ReaderWriterLock(char* debugName); // initialize barrier
    ~ReaderWriterLock(); // deallocate the barrier
    char* getName() { return (name); } // debugging assist/

    // For Reader
    void ReaderAcquire();
    void ReaderRelease();
    // For Writer
    void WriterAcquire();
    void WriterRelease();
  
  private:
    char* name;             // useful for debugging
    int blockingReader;     // counts for blocking readers

    // "Global" mutex lock for writer that can be release by other
    // Alias g in comment
    Semaphore* binary_semaphore_writer;
    // Lock used by reader to protect number "blockingReader"
    // Alias r in comment
    Lock* mutex_reader;
};
```

#### Reader

```cpp
//----------------------------------------------------------------------
// ReaderWriterLock::ReaderAcquire
//----------------------------------------------------------------------

void
ReaderWriterLock::ReaderAcquire()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts


    mutex_reader->Acquire(); // Lock r
    blockingReader++;
    DEBUG('w', "Reader \"%s\" comming in  \t(blockingReader=%d)\n", currentThread->getName(), blockingReader);
    if (blockingReader == 1) {
        binary_semaphore_writer->P(); // Lock g
    }
    mutex_reader->Release(); // Unlock r

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

```cpp
//----------------------------------------------------------------------
// ReaderWriterLock::ReaderRelease
//----------------------------------------------------------------------

void ReaderWriterLock::ReaderRelease()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    mutex_reader->Acquire(); // Lock r
    blockingReader--;
    DEBUG('w', "Reader \"%s\" getting out \t(blockingReader=%d)\n", currentThread->getName(), blockingReader);
    if (blockingReader == 0) {
        binary_semaphore_writer->V(); // Unlock g
    }
    mutex_reader->Release(); // Unlock r

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
```

#### Writer

```cpp
//----------------------------------------------------------------------
// ReaderWriterLock::WriterAcquire
//----------------------------------------------------------------------

void ReaderWriterLock::WriterAcquire()
{
    DEBUG('w', "Writer \"%s\" comming in  \t(blockingReader=%d)\n", currentThread->getName(), blockingReader);
    binary_semaphore_writer->P(); // Lock g
}
```

```cpp
//----------------------------------------------------------------------
// ReaderWriterLock::WriterRelease
//----------------------------------------------------------------------

void ReaderWriterLock::WriterRelease()
{
    DEBUG('w', "Writer \"%s\" getting out \t(blockingReader=%d)\n", currentThread->getName(), blockingReader);
    binary_semaphore_writer->V(); // Unlock g
}
```

Other trivial code implementation just checkout `threads/synch.cc`.

### The First Readers-writer Problem

I've made the problem, that

* The Writer is going to write the quote of Albert Einstein into `char quote[SENTENCE_LENGTH][MAX_CHAR]`.
  * Insanity: doing the same thing over and over again and expecting different results
* The Readers are going to read the quote repeatedly until writer has finish the quote.

The problem is written in `threads/threadtest.cc` as case 10.

#### Reader Thread

```cpp
//----------------------------------------------------------------------
// Reader Thread
//  Keeping trying to read the quote. Until writer has finished the
//  quote. (i.e. the words is match the SENTENCE_LENGTH)
//----------------------------------------------------------------------

void
ReaderThread(int reader_id)
{
    do {
        RWLock->ReaderAcquire();

        printf("Reader %d is reading: ", reader_id);
        for (int i = 0; i < words; i++) {
            printf("%s ", quote[i]);
        }
        printf("\n");

        RWLock->ReaderRelease();
    } while (words < SENTENCE_LENGTH);
}
```

#### Writer Thread

```cpp
//----------------------------------------------------------------------
// Writer Thread
//  Trying hard to write the quote...
//----------------------------------------------------------------------

void
WriterThread(int dummy)
{
    while (words < SENTENCE_LENGTH) {
        RWLock->WriterAcquire();

        strcpy(quote[words], AlbertEinstein[words]); // composing...
        words++;

        printf("Writer is writting: ");
        for (int i = 0; i < words; i++) {
            printf("%s ", quote[i]);
        }
        printf("\n");

        RWLock->WriterRelease();
    }
}
```

### Result with random context switch

> `-d w` for reader-writer lock; `-d c` for context switch;
> `-rs` for random context switch timer interrupt

This is the result with 3 readers.

> We need some luck to see the "phenomenon".
> Threre are some possible scenario during random context switch
>
> 1. The thread has done its job.
> 2. Writer is still writing
> 3. Readers are still reading
>
> If you got lots of readers, you may take a long time for writer to finsh composing.
> But sometime writer is just happen to writting...

#### With 2 readers (without reader-writer messages)

```txt
$ threads/nachos -d c -rs -q 10
Lab3 Challenge2: Reader-Writer
(add `-d wc -rs` argument to show "Context Switch", RWLock message and activate random timer)
Writer is writting: Insanity: 
Writer is writting: Insanity: doing 
Writer is writting: Insanity: doing the 
Writer is writting: Insanity: doing the same 
Writer is writting: Insanity: doing the same thing 
Writer is writting: Insanity: doing the same thing over 
Writer is writting: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 190) >>
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 280) >>
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 460) >>
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 580) >>
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 780) >>
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
Reader 1 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 920) >>
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
Reader 2 is reading: Insanity: doing the same thing over and 
 << random Context Switch (stats->totalTicks = 1110) >>
Writer is writting: Insanity: doing the same thing over and over 
Writer is writting: Insanity: doing the same thing over and over again 
Writer is writting: Insanity: doing the same thing over and over again and 
Writer is writting: Insanity: doing the same thing over and over again and expecting 
Writer is writting: Insanity: doing the same thing over and over again and expecting different 
 << random Context Switch (stats->totalTicks = 1210) >>
Reader 1 is reading: Insanity: doing the same thing over and over again and expecting different 
Reader 1 is reading: Insanity: doing the same thing over and over again and expecting different 
 << random Context Switch (stats->totalTicks = 1260) >>
Reader 2 is reading: Insanity: doing the same thing over and over again and expecting different 
 << random Context Switch (stats->totalTicks = 1290) >>
Writer is writting: Insanity: doing the same thing over and over again and expecting different results. 
```

> The `quote` is taken control continuously by Reader 1 and 2...

#### With 3 readers (with reader-writer messages)

```txt
$ threads/nachos -d wc -rs -q 10
Lab3 Challenge2: Reader-Writer
(add `-d wc -rs` argument to show "Context Switch", RWLock message and activate random timer)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: doing 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: doing the 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: doing the same 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: doing the same thing 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
Writer is writting: Insanity: doing the same thing over 
Writer "Writer" getting out     (blockingReader=0)
Writer "Writer" comming in      (blockingReader=0)
 << random Context Switch (stats->totalTicks = 190) >>
Reader "Reader 1" comming in    (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and over 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
 << random Context Switch (stats->totalTicks = 280) >>
Writer is writting: Insanity: doing the same thing over and over again 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and over again and 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and over again and expecting 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and over again and expecting different 
Writer "Writer" getting out     (blockingReader=1)
Writer "Writer" comming in      (blockingReader=1)
Writer is writting: Insanity: doing the same thing over and over again and expecting different results. 
Writer "Writer" getting out     (blockingReader=1)
Reader 1 is reading: Insanity: doing the same thing over and over again and expecting different results. 
Reader "Reader 1" getting out   (blockingReader=0)
Reader "Reader 2" comming in    (blockingReader=1)
Reader 2 is reading: Insanity: doing the same thing over and over again and expecting different results. 
Reader "Reader 2" getting out   (blockingReader=0)
Reader "Reader 3" comming in    (blockingReader=1)
Reader 3 is reading: Insanity: doing the same thing over and over again and expecting different results. 
Reader "Reader 3" getting out   (blockingReader=0)
```

> The `quote` is luckly taken control by Writer...

## Challenge 3: Implement Linux's kfifo

> Research if Linux's kfifo module can be merge into Nachos as a new module.

[`include/linux/kfifo.h`](https://github.com/torvalds/linux/blob/master/include/linux/kfifo.h)

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

* [Debugging Segmentation Faults and Pointer Problems](https://www.cprogramming.com/debugging/segfaults.html)

Common mistakes:

1. dereferencing NULL
2. dereferencing an uninitialized pointer
3. dereferencing a pointer that has been freed (or deleted, in C++) or that has gone out of scope (in the case of arrays declared in functions)
4. writing off the end of an array.

### Warning deprecated conversion from string constant to ‘char*’

> So many warning message makes me feel annoying..

* [How to get rid of `deprecated conversion from string constant to ‘char*’` warnings in GCC?](https://stackoverflow.com/questions/59670/how-to-get-rid-of-deprecated-conversion-from-string-constant-to-char-warnin)

1. Passing `-Wno-write-strings` to gcc and g++
    > I've add it in `Makefile.common` in CFLAGS
2. Use `char const *` as the type instead of `char*`

### strcat() a integer, strcpy() and strdup()

```c
char string[STRING_SIZE] = "text "
char buffer[BUFFER_SIZE];
sprintf(buffer, "%d", number);
strcat(string, buffer)
```

#### copy

```c
char * strncpy(char * dst, const char * src, size_t len));
```

```c
char str[80];
strcpy (str,"these ");
strcat (str,"strings ");
strcat (str,"are ");
strcat (str,"concatenated.");
puts (str);
```

#### duplicate string

```c
char * strndup(const char *s1, size_t n);
```

## Resources

* [Wiki - Concurrency (computer science)](https://en.wikipedia.org/wiki/Concurrency_(computer_science))
* [Wiki - Monitor (synchronization) - Condition variables](https://en.wikipedia.org/wiki/Monitor_(synchronization)#Condition_variables)
* [Stackoverflow - When to use pthread condition variables?](https://stackoverflow.com/questions/20772476/when-to-use-pthread-condition-variables)
* [C++ Core Guidelines: Be Aware of the Traps of Condition Variables](http://www.modernescpp.com/index.php/c-core-guidelines-be-aware-of-the-traps-of-condition-variables)
* [GeeksforGeeks - Array of Strings in C++ (3 Different Ways to Create)](https://www.geeksforgeeks.org/array-strings-c-3-different-ways-create/)

### Book

> Bold the chapter with more reference.

Operating System Concept 9ed.

* Ch3 Processes
  * Ch3.4 Interprocess Communication
    * **Ch3.4.1** Shared-Memory Systems => Producer-consumer bounded buffer
* Ch5 Process Synchronization
  * **Ch5.1** Background => Producer-consumer
  * Ch5.2 The Critical-Section Problem
  * Ch5.3 Peterson's Solution
  * Ch5.4 Synchronization Hardware
  * Ch5.5 Mutex Locks 互斥鎖（鎖）
  * Ch5.6 Semaphores 號誌（信號量）
  * Ch5.7 Classic Problems of Synchronization
    * **Ch5.7.1** The Bounded-Buffer Problem => Producer-consumer semaphore
    * **Ch5.7.2** The Readers-Writers Problem
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
  * **Ch2.3.9** Barriers

### Example

* [CSDN - 同步機制實驗報告_Nachos Lab3](https://blog.csdn.net/superli90/article/details/29376171)
* [CSDN - nachos lab3-線程同步](https://blog.csdn.net/wyxpku/article/details/52076209)
