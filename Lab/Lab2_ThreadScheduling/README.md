# Lab 2: Thread Scheduling

## Exercise 1: Research

> Research the CPU (Process/Thread) scheduling algorithm of Linux or Windows

### Linux

#### Completely Fair Scheduler (CFS)

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
*** current thread (uid=0, tid=0, pri=0 name=main) => Yield
Ready list contents:
lowest, lower, highest,

*** current thread (uid=0, tid=3, pri=38 name=lowest) => Yield
Ready list contents:
main, lower, highest,

--- Calling TS command ---
UID     TID     NAME    PRI     STATUS
0       0       main    0       RUNNING
0       1       lower   78      READY
0       2       highest 87      READY
0       3       lowest  38      READY
--- End of TS command ---

*** current thread (uid=0, tid=1, pri=78 name=lower) => Yield
Ready list contents:
highest,

*** current thread (uid=0, tid=2, pri=87 name=highest) => Yield
Ready list contents:
lower,
```

## Challenge 1: More expansion

> Implement Round Robin or Multilevel Feedback Queues. Or implement Linux or Windows's algorithm.

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

## Resources

### Book

* Data Structures and Algorithm Analysis in C++ 4ed.
  * 1.4 C++ Classes
    * 1.4.1 Basic class Syntax
    * 1.4.2 Extra Constructor Syntax and Accessors

### Example

* [CSDN - 線程調度實驗報告_Nachos Lab2](https://blog.csdn.net/superli90/article/details/29373593)
* [CSDN - nachos lab2-線程調度](https://blog.csdn.net/wyxpku/article/details/52076206)

### Article

* [Linux's Process Scheduler](https://notes.shichao.io/lkd/ch4/#linuxs-process-scheduler)
