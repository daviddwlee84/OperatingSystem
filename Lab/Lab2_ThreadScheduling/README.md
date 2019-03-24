# Lab 2: Thread Scheduling

## Exercise 1: Research

> Research the CPU (Process/Thread) scheduling algorithm of Linux or Windows

### Linux scheduling algorithm

Linux has implemented three different scheduler class. Two for *real-time process*, one for conventional *time-shared process*

> O(1) scheduler: selects the process to run in constant time

Priority range:

* 0~99: real-time priorities
* 100~139: normal priorities

#### For Real-time process

> defined in [`kernel/sched/rt.c`](https://github.com/torvalds/linux/blob/master/kernel/sched/rt.c)

* SCHED_FIFO: **First-In, First-Out**
* SCHED_RR: **Round Robin**
  * SCHED_FIFO with timeslices

#### For Conventional Time-shared process

> defined in [`kernel/sched/fair.c`](https://github.com/torvalds/linux/blob/master/kernel/sched/fair.c)

* SCHED_NORMAL: **Completely Fair Scheduler (CFS)**
  * elegant handling of I/O and CPU bound processes

> Ideal **Fair**: N processes => each process can get 1/N of CPU time

* **Virtual Runtime** (`vruntime`): for each runnable process
  * process run t ms => vruntime += t
* **Priority** (i.e. **nice value**) used to weight the `vruntime`
  * process run t ms => vruntime += t * (weight based on nice of process)

Main idea of CFS (when the timer interrupt occurs)

* CFS uses a *Red-black tree* to maintain each runnable process instead of traditional ready queue
  * Self balancing
  * All operations are O(log n)
* Choose the task with the lowest `vruntime` (`cfs_rq->min_vruntime`)
  * This approach avoid the starvation
  * Pick process form the bottom left of the tree (min) then move toward the right part of the tree

## Exercise 2: Trace source code

> Read the following code, understand current Nachos thread mechanism
>
> * `code/threads/scheduler.h`
> * `code/threads/scheduler.cc`
> * `code/threads/switch.s`
> * `code/threads/timer.h`
> * `code/threads/timer.cc`

```cpp
class Scheduler {
  public:
    Scheduler();            // Initialize list of ready threads
    ~Scheduler();            // De-allocate ready list

    void ReadyToRun(Thread* thread);    // Thread can be dispatched.
    Thread* FindNextToRun();        // Dequeue first thread on the ready
                    // list, if any, and return thread.
    void Run(Thread* nextThread);    // Cause nextThread to start running
    void Print();            // Print contents of ready list

  private:
    List *readyList;          // queue of threads that are ready to run,
                // but not running
};
```

The original Nachos scheduling is simply FIFO without priority.

## Exercise 3: Expand Thread scheduling algorithm

> Implement preemption algorithm based on priority

### 1. Add priority property in Thread

Add priority and its set/get method in `Thread` class in `threads/threads.h`

```cpp
class Thread {
    private:
        // Lab2: priority of thread
        int priority;                       // Lab2: Add priority

    public:
        // Lab2: priority of thread
        int getPriority() { return (priority); }  // Lab2: Get priority
        void setPriority(int p) { priority = p; } // Lab2: Set priority
}
```

Initialize priority in initialization (use [constructor overloading](#C++-Constructor-Overloading))

1. Add `Thread(char* debugName, int p);		// Lab2: initialize a Thread with priority` in `threads/threads.h`
2. Modify original Thread constructor in `threads/threads.cc`

    ```cpp
    Thread::Thread(char* threadName, int p)
        : priority( p ) // Lab2: default priority
    {
        // The same...
        // ...

    }
    ```

    ```cpp
    // C++ Constructor Overloading
    Thread::Thread(char* threadName)
        : Thread(threadName, 0)
    {
        // do nothing special
    }
    ```

> (Optional) Extend TS command in Lab 1

Test (add the following test in `threads/threadtest.cc`) and as case 5

```cpp
//----------------------------------------------------------------------
// Lab2 Exercise3-1
// 	Fork some Thread with different ways to initial the priority
//----------------------------------------------------------------------

void
Lab2Exercise3_1()
{
    DEBUG('t', "Entering Lab2Exercise3_1");

    Thread *t1 = new Thread("with p", 87);

    Thread *t2 = new Thread("set p");
    t2->setPriority(100);

    Thread *t3 = new Thread("no p");

    t1->Fork(CustomThreadFunc, (void*)0);
    t2->Fork(CustomThreadFunc, (void*)0);
    t3->Fork(CustomThreadFunc, (void*)0);

    CustomThreadFunc(0); // Yield the current thread

    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n");
}
```

Result

```txt
$ threads/nachos -q 5
Lab2 Exercise3-1:
*** current thread (uid=0, tid=0, name=main) => Yield
*** current thread (uid=0, tid=1, name=with p) => Yield
*** current thread (uid=0, tid=2, name=set p) => Yield
*** current thread (uid=0, tid=3, name=no p) => Yield
--- Calling TS command ---
UID     TID     NAME    PRI     STATUS
0       0       main    0       RUNNING
0       1       with p  87      READY
0       2       set p   100     READY
0       3       no p    0       READY
--- End of TS command ---
```

### 2. Change insertion of readyList by using SortedInsert

Modify `Scheduler::ReadyToRun` in `threads/scheduler.cc`

```cpp
void
Scheduler::ReadyToRun (Thread *thread)
{

    // readyList->Append((void *)thread); // Orignal
    readyList->SortedInsert((void *)thread, thread->getPriority()); // Lab2: insert Thread with priority
}
```

Test (add the following test in `threads/threadtest.cc`) and as case 6

> I've also changed a little bit output of `CustomThreadFunc()`

```cpp
//----------------------------------------------------------------------
// Lab2 Exercise3-2
// 	Fork some Thread with different priority
//  and observe if the lower one will take over the CPU
//----------------------------------------------------------------------

void
Lab2Exercise3_2()
{
    DEBUG('t', "Entering Lab2Exercise3_2");

    Thread *t1 = new Thread("lower", 78);
    Thread *t2 = new Thread("highest", 87);
    // The lowest one will be put in front of the list
    // due to SortedInsert() in list.cc
    Thread *t3 = new Thread("lowest", 38);

    t1->Fork(CustomThreadFunc, (void*)0);
    t2->Fork(CustomThreadFunc, (void*)0);
    t3->Fork(CustomThreadFunc, (void*)0);

    CustomThreadFunc(0); // Yield the current thread

    // Because the main() Thread has priority 0
    // Then any process yield will make main keep running
    // Since 0 is the lowest number and it will be in front of the readyList
    // So the TS command will be called right after the first Yield()
    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n\n");
}
```

Result

```txt
$ threads/nachos -q 6
Lab2 Exercise3-2:
*** current thread (uid=0, tid=0, pri=0, name=main) => Yield
Ready list contents:
lowest, lower, highest,

*** current thread (uid=0, tid=3, pri=38, name=lowest) => Yield
Ready list contents:
main, lower, highest,

--- Calling TS command ---
UID     TID     NAME    PRI     STATUS
0       0       main    0       RUNNING
0       1       lower   78      READY
0       2       highest 87      READY
0       3       lowest  38      READY
--- End of TS command ---

*** current thread (uid=0, tid=1, pri=78, name=lower) => Yield
Ready list contents:
highest,

*** current thread (uid=0, tid=2, pri=87, name=highest) => Yield
Ready list contents:
lower,
```

## Challenge 1: More expansion: Round Robin

> Implement Round Robin or Multilevel Feedback Queues. Or implement Linux or Windows's algorithm.

Tracing code

* In the original Nachos system. If you give `-rs` argument, then in `threads/system.cc`, it will enable random timer interrupt.
  * calling `TimerInterruptHandler()` and it invokes `interrupt->YieldOnReturn()` will *cause a context switch* (in `machine/interrupt.cc`)
  * `interrupt->YieldOnReturn()` is simply raise `yieldOnReturn` flag. And the thread will be yielded in `Interrupt::OneTick()`.
    * Comment said
        ```c
        //	Two things can cause OneTick to be called:
        //		interrupts are re-enabled
        //		a user instruction is executed
        ```
    * `Interrupt::SetLevel()`
* In the `machine/stats.h` Nachos has defined
  * `#define TimerTicks 100 // (average) time between timer interrupt` (時間片).
  * public variabe `int totalTicks; // Total time running Nachos` of `class Statistics`
    * `Statistics *stats; // performance metrics` define in `threads/system.cc`
* Nachos will invoke the interrupt handler for timer in `Timer::TimerExpired()` in `machine/timer.cc`

Thus, first yield the Thread when the time is up.
Second, we need to start a timer.
Third, re-enabled the interrupt make the system calls `Interrupt::OneTick()` to make system time move forward.

> I just want to use RR only in this Lab. I don't really want to modify the entire system (i.e. change `FindNextToRun()`).

### 1. Modify scheduler

Add funciton and variable in `class Scheduler` in `threads/scheduler.h` and `threads/scheduler.cc`

```cpp
class Scheduler {
  public:
    // Lab2: RR rescheduling
    int lastSwitchTick; // Lab2: record the last context switch time
}

// Lab2: RR rescheduling
extern void RRHandler(int dummy); // Lab2: re-order the readyList when timer interrupt
```

> `RRHandler()` is imitating `TimerInterruptHandler()`

```cpp
//----------------------------------------------------------------------
// Scheduler::RRHandler (Lab2)
// 	Round Robin Rescheduling, by reordering the ready list.
//	(Called when the timer interrupt triggered)
//----------------------------------------------------------------------
static void
RRHandler(int dummy)
{
    int timeDuration = stats->totalTicks - scheduler->lastSwitchTick;
    printf("\nTimer interrupt with duration: %d", timeDuration);
    if (timeDuration >= TimerTicks) {
        if (interrupt->getStatus() != IdleMode) { // IdleMode == readyList empty
            printf(" (Determine to Context switch)\n");
            interrupt->YieldOnReturn();
            scheduler->lastSwitchTick = stats->totalTicks; // update lastSwitchTick
        } else {
            printf(" (readyList is Empty)\n");
        }
    } else {
        printf("\n");
    }
}
```

> "dummy" is because every interrupt handler takes one argument, whether it needs it or not.

### 2. Activate timer

Add `-rr` argument in `threads/system.cc` to active timer for `Scheduler::RRHandler()`

```cpp
bool roundRobin = FALSE; // Lab2: Round robin

if (!strcmp(*argv, "-rr")) { // Lab2: activate RR timer
    ASSERT(argc > 1);
    roundRobin = TRUE;
    argCount = 2;
}

if (roundRobin) // Lab2: start the RR timer
    timer = new Timer(scheduler->RRHandler, 0, FALSE);
```

### 3. Test with calling OneTick()

In `threads/threadtest.cc` we invoke `OneTick()` to advance simulated time.

> Each OneTick() will move 10 ticks (`SystemTick`)

```cpp
//----------------------------------------------------------------------
// ThreadWithTicks
//  Re-enable the interrupt to invoke OnTick() make system time moving forward
//----------------------------------------------------------------------

void
ThreadWithTicks(int runningTime)
{
    int num;
    
    for (num = 0; num < runningTime * SystemTick; num++) {
        printf("*** thread with running time %d looped %d times (stats->totalTicks: %d)\n", runningTime, num+1, stats->totalTicks);
        interrupt->OneTick(); // make system time moving forward (advance simulated time)
        // Switch interrupt on and off to invoke OneTick() (not necessary...)
        // interrupt->SetLevel(IntOn);
        // interrupt->SetLevel(IntOff);
    }
    currentThread->Finish();
}
```

Add the Lab2ChallengeRR as case 7

```cpp
//----------------------------------------------------------------------
// Lab2 Challenge Round Robin
// 	Fork some Thread with different priority
//  and observe if the lower one will take over the CPU
//----------------------------------------------------------------------

void
Lab2ChallengeRR()
{
    DEBUG('t', "Entering Lab2ChallengeRR");

    printf("\nSystem initial ticks:\tsystem=%d, user=%d, total=%d\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    Thread *t1 = new Thread("7");
    Thread *t2 = new Thread("2");
    Thread *t3 = new Thread("5");

    printf("\nAfter new Thread ticks:\tsystem=%d, user=%d, total=%d\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    t1->Fork(ThreadWithTicks, (void*)7);
    t2->Fork(ThreadWithTicks, (void*)2);
    t3->Fork(ThreadWithTicks, (void*)5);

    printf("\nAfter 3 fork() ticks:\tsystem=%d, user=%d, total=%d\n\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    // update the lastSwitchTick
    // (according to previous test, it will start from 50)
    scheduler->lastSwitchTick = stats->totalTicks;
    currentThread->Yield(); // Yield the main thread
}
```

Create three threads with different running time. And see if the system switch them when the time is up!

#### Result

```sh
threads/nachos -rr -q 7
```

> It seems like Conext Switch need 10 ticks.
> And system need 10 ticks to boot up and 10 ticks for each fork()

```txt
Lab2 Challenge RR:
(don't forget to add `-rr` argument to activate timer)

System initial ticks:   system=10, user=0, total=10

After new Thread ticks: system=10, user=0, total=10

After 3 fork() ticks:   system=40, user=0, total=40

*** thread with running time 7 looped 1 times (stats->totalTicks: 50)
*** thread with running time 7 looped 2 times (stats->totalTicks: 60)
*** thread with running time 7 looped 3 times (stats->totalTicks: 70)
*** thread with running time 7 looped 4 times (stats->totalTicks: 80)
*** thread with running time 7 looped 5 times (stats->totalTicks: 90)

Timer interrupt with duration: 60
*** thread with running time 7 looped 6 times (stats->totalTicks: 100)
*** thread with running time 7 looped 7 times (stats->totalTicks: 110)
*** thread with running time 7 looped 8 times (stats->totalTicks: 120)
*** thread with running time 7 looped 9 times (stats->totalTicks: 130)
*** thread with running time 7 looped 10 times (stats->totalTicks: 140)
*** thread with running time 7 looped 11 times (stats->totalTicks: 150)
*** thread with running time 7 looped 12 times (stats->totalTicks: 160)
*** thread with running time 7 looped 13 times (stats->totalTicks: 170)
*** thread with running time 7 looped 14 times (stats->totalTicks: 180)
*** thread with running time 7 looped 15 times (stats->totalTicks: 190)

Timer interrupt with duration: 160 (Determine to Context switch)
*** thread with running time 2 looped 1 times (stats->totalTicks: 210)
*** thread with running time 2 looped 2 times (stats->totalTicks: 220)
*** thread with running time 2 looped 3 times (stats->totalTicks: 230)
*** thread with running time 2 looped 4 times (stats->totalTicks: 240)
*** thread with running time 2 looped 5 times (stats->totalTicks: 250)
*** thread with running time 2 looped 6 times (stats->totalTicks: 260)
*** thread with running time 2 looped 7 times (stats->totalTicks: 270)
*** thread with running time 2 looped 8 times (stats->totalTicks: 280)
*** thread with running time 2 looped 9 times (stats->totalTicks: 290)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 5 looped 1 times (stats->totalTicks: 310)
*** thread with running time 5 looped 2 times (stats->totalTicks: 320)
*** thread with running time 5 looped 3 times (stats->totalTicks: 330)
*** thread with running time 5 looped 4 times (stats->totalTicks: 340)
*** thread with running time 5 looped 5 times (stats->totalTicks: 350)
*** thread with running time 5 looped 6 times (stats->totalTicks: 360)
*** thread with running time 5 looped 7 times (stats->totalTicks: 370)
*** thread with running time 5 looped 8 times (stats->totalTicks: 380)
*** thread with running time 5 looped 9 times (stats->totalTicks: 390)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 16 times (stats->totalTicks: 420)
*** thread with running time 7 looped 17 times (stats->totalTicks: 430)
*** thread with running time 7 looped 18 times (stats->totalTicks: 440)
*** thread with running time 7 looped 19 times (stats->totalTicks: 450)
*** thread with running time 7 looped 20 times (stats->totalTicks: 460)
*** thread with running time 7 looped 21 times (stats->totalTicks: 470)
*** thread with running time 7 looped 22 times (stats->totalTicks: 480)
*** thread with running time 7 looped 23 times (stats->totalTicks: 490)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 2 looped 10 times (stats->totalTicks: 510)
*** thread with running time 2 looped 11 times (stats->totalTicks: 520)
*** thread with running time 2 looped 12 times (stats->totalTicks: 530)
*** thread with running time 2 looped 13 times (stats->totalTicks: 540)
*** thread with running time 2 looped 14 times (stats->totalTicks: 550)
*** thread with running time 2 looped 15 times (stats->totalTicks: 560)
*** thread with running time 2 looped 16 times (stats->totalTicks: 570)
*** thread with running time 2 looped 17 times (stats->totalTicks: 580)
*** thread with running time 2 looped 18 times (stats->totalTicks: 590)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 5 looped 10 times (stats->totalTicks: 610)
*** thread with running time 5 looped 11 times (stats->totalTicks: 620)
*** thread with running time 5 looped 12 times (stats->totalTicks: 630)
*** thread with running time 5 looped 13 times (stats->totalTicks: 640)
*** thread with running time 5 looped 14 times (stats->totalTicks: 650)
*** thread with running time 5 looped 15 times (stats->totalTicks: 660)
*** thread with running time 5 looped 16 times (stats->totalTicks: 670)
*** thread with running time 5 looped 17 times (stats->totalTicks: 680)
*** thread with running time 5 looped 18 times (stats->totalTicks: 690)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 24 times (stats->totalTicks: 710)
*** thread with running time 7 looped 25 times (stats->totalTicks: 720)
*** thread with running time 7 looped 26 times (stats->totalTicks: 730)
*** thread with running time 7 looped 27 times (stats->totalTicks: 740)
*** thread with running time 7 looped 28 times (stats->totalTicks: 750)
*** thread with running time 7 looped 29 times (stats->totalTicks: 760)
*** thread with running time 7 looped 30 times (stats->totalTicks: 770)
*** thread with running time 7 looped 31 times (stats->totalTicks: 780)
*** thread with running time 7 looped 32 times (stats->totalTicks: 790)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 2 looped 19 times (stats->totalTicks: 810)
*** thread with running time 2 looped 20 times (stats->totalTicks: 820)
*** thread with running time 5 looped 19 times (stats->totalTicks: 840)
*** thread with running time 5 looped 20 times (stats->totalTicks: 850)
*** thread with running time 5 looped 21 times (stats->totalTicks: 860)
*** thread with running time 5 looped 22 times (stats->totalTicks: 870)
*** thread with running time 5 looped 23 times (stats->totalTicks: 880)
*** thread with running time 5 looped 24 times (stats->totalTicks: 890)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 33 times (stats->totalTicks: 910)
*** thread with running time 7 looped 34 times (stats->totalTicks: 920)
*** thread with running time 7 looped 35 times (stats->totalTicks: 930)
*** thread with running time 7 looped 36 times (stats->totalTicks: 940)
*** thread with running time 7 looped 37 times (stats->totalTicks: 950)
*** thread with running time 7 looped 38 times (stats->totalTicks: 960)
*** thread with running time 7 looped 39 times (stats->totalTicks: 970)
*** thread with running time 7 looped 40 times (stats->totalTicks: 980)
*** thread with running time 7 looped 41 times (stats->totalTicks: 990)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 5 looped 25 times (stats->totalTicks: 1010)
*** thread with running time 5 looped 26 times (stats->totalTicks: 1020)
*** thread with running time 5 looped 27 times (stats->totalTicks: 1030)
*** thread with running time 5 looped 28 times (stats->totalTicks: 1040)
*** thread with running time 5 looped 29 times (stats->totalTicks: 1050)
*** thread with running time 5 looped 30 times (stats->totalTicks: 1060)
*** thread with running time 5 looped 31 times (stats->totalTicks: 1070)
*** thread with running time 5 looped 32 times (stats->totalTicks: 1080)
*** thread with running time 5 looped 33 times (stats->totalTicks: 1090)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 42 times (stats->totalTicks: 1110)
*** thread with running time 7 looped 43 times (stats->totalTicks: 1120)
*** thread with running time 7 looped 44 times (stats->totalTicks: 1130)
*** thread with running time 7 looped 45 times (stats->totalTicks: 1140)
*** thread with running time 7 looped 46 times (stats->totalTicks: 1150)
*** thread with running time 7 looped 47 times (stats->totalTicks: 1160)
*** thread with running time 7 looped 48 times (stats->totalTicks: 1170)
*** thread with running time 7 looped 49 times (stats->totalTicks: 1180)
*** thread with running time 7 looped 50 times (stats->totalTicks: 1190)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 5 looped 34 times (stats->totalTicks: 1210)
*** thread with running time 5 looped 35 times (stats->totalTicks: 1220)
*** thread with running time 5 looped 36 times (stats->totalTicks: 1230)
*** thread with running time 5 looped 37 times (stats->totalTicks: 1240)
*** thread with running time 5 looped 38 times (stats->totalTicks: 1250)
*** thread with running time 5 looped 39 times (stats->totalTicks: 1260)
*** thread with running time 5 looped 40 times (stats->totalTicks: 1270)
*** thread with running time 5 looped 41 times (stats->totalTicks: 1280)
*** thread with running time 5 looped 42 times (stats->totalTicks: 1290)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 51 times (stats->totalTicks: 1310)
*** thread with running time 7 looped 52 times (stats->totalTicks: 1320)
*** thread with running time 7 looped 53 times (stats->totalTicks: 1330)
*** thread with running time 7 looped 54 times (stats->totalTicks: 1340)
*** thread with running time 7 looped 55 times (stats->totalTicks: 1350)
*** thread with running time 7 looped 56 times (stats->totalTicks: 1360)
*** thread with running time 7 looped 57 times (stats->totalTicks: 1370)
*** thread with running time 7 looped 58 times (stats->totalTicks: 1380)
*** thread with running time 7 looped 59 times (stats->totalTicks: 1390)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 5 looped 43 times (stats->totalTicks: 1410)
*** thread with running time 5 looped 44 times (stats->totalTicks: 1420)
*** thread with running time 5 looped 45 times (stats->totalTicks: 1430)
*** thread with running time 5 looped 46 times (stats->totalTicks: 1440)
*** thread with running time 5 looped 47 times (stats->totalTicks: 1450)
*** thread with running time 5 looped 48 times (stats->totalTicks: 1460)
*** thread with running time 5 looped 49 times (stats->totalTicks: 1470)
*** thread with running time 5 looped 50 times (stats->totalTicks: 1480)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 60 times (stats->totalTicks: 1510)
*** thread with running time 7 looped 61 times (stats->totalTicks: 1520)
*** thread with running time 7 looped 62 times (stats->totalTicks: 1530)
*** thread with running time 7 looped 63 times (stats->totalTicks: 1540)
*** thread with running time 7 looped 64 times (stats->totalTicks: 1550)
*** thread with running time 7 looped 65 times (stats->totalTicks: 1560)
*** thread with running time 7 looped 66 times (stats->totalTicks: 1570)
*** thread with running time 7 looped 67 times (stats->totalTicks: 1580)
*** thread with running time 7 looped 68 times (stats->totalTicks: 1590)

Timer interrupt with duration: 100 (Determine to Context switch)
*** thread with running time 7 looped 69 times (stats->totalTicks: 1610)
*** thread with running time 7 looped 70 times (stats->totalTicks: 1620)
```

> weird thing is `stats-totalTicks` ticks from 390 to 420...

## Notes

### C++ Constructor Overloading

Constructor can be overloaded in a similar way as [function overloading](https://www.programiz.com/cpp-programming/function-overloading).

Overloaded constructors have the same name (name of the class) but different number of arguments.

Depending upon the number and type of arguments passed, specific constructor is called.

Since, there are multiple constructors present, argument to the constructor should also be passed while creating an object.

* [GeeksforGeeks - Constructor Overloading in C++](https://www.geeksforgeeks.org/constructor-overloading-c/)

### C++ 11

Compile

* [Stackoverflow - Compiling C++11 with g++](https://stackoverflow.com/questions/10363646/compiling-c11-with-g)

```sh
g++ -std=c++11 your_file.cpp -o your_program
```

or

```sh
g++ -std=c++0x your_file.cpp -o your_program
```

> the compiled binary can't work when add C++ 11 flag in `Makefile.common` after `CFLAGS =`.

### Passing a Function in C++

> I was ment to put `RRHandler()` in `class Scheduler` and `lastSwitchTick` as private member.
> But in C++. passing a non-static object member isn't as simple as passing a [normal funcion pointer](#Funciton-Pointer-In-Nachos).

* [**Stackoverflow - How can I pass a member function where a free function is expected?**](https://stackoverflow.com/questions/12662891/how-can-i-pass-a-member-function-where-a-free-function-is-expected)

#### Funciton Pointer In Nachos

`utility.h`

```cpp
// This declares the type "VoidFunctionPtr" to be a "pointer to a
// function taking an integer argument and returning nothing".  With
// such a function pointer (say it is "func"), we can call it like this:
//
//	(*func) (17);
//
// This is used by Thread::Fork and for interrupt handlers, as well
// as a couple of other places.

typedef void (*VoidFunctionPtr)(int arg);
typedef void (*VoidNoArgFunctionPtr)();
```

## Resources

### Book

* Data Structures and Algorithm Analysis in C++ 4ed.
  * Ch1.4 C++ Classes
    * Ch1.4.1 Basic class Syntax
    * Ch1.4.2 Extra Constructor Syntax and Accessors
* Nachos 中文教程 Ch3 線程管理系統 第五節 實現實例 4.2 對線程調度的改進

### Example

* [CSDN - 線程調度實驗報告_Nachos Lab2](https://blog.csdn.net/superli90/article/details/29373593)
* [CSDN - nachos lab2-線程調度](https://blog.csdn.net/wyxpku/article/details/52076206)
* [NTUST OS_HOMEWORK_2](http://neuron.csie.ntust.edu.tw/homework/99/OS/homework/homework2/B9715046-hw2-1/)

### Article

* [**Shichao's Notes Chapter 4. Process Scheduling - Linux's Process Scheduler**](https://notes.shichao.io/lkd/ch4/#linuxs-process-scheduler)
* [Linux Kernel: Process Scheduling](https://medium.com/hungys-blog/linux-kernel-process-scheduling-8ce05939fabd)

### Video

* [Youtube - Operating System #22 Completely Fair Scheduling (CFS)](https://youtu.be/scfDOof9pww)
