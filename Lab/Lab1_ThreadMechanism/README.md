# Lab 1: Thread Mechanism

## Exercise 1: Research

> Difference between Linux PCB and Nachos mechanism

* Linux has only concept of Process and Nachos has only Thread. (as scheduling unit)

### Linux

Basically defined in [include/linux/sched.h](https://github.com/torvalds/linux/blob/master/include/linux/sched.h)

The `task_struct` (process descriptor), is an element in the task list.

* Open files
* Pending signals
* Internal kernel data
* Processor state
* Memory address space with one or more memory mappings
* Thread(s) of execution
  * Program counter
  * Process stack
  * Set of processor registers
* Data section containing global variables
* Program code (text section)

### Nachos

That is the `class Thread`. In initial status, Nachos Thread only contain the following information.

* `int* stackTop; // the current stack pointer`
* `void *machineState[MachineStateSize]; // all registers except for stackTop`
* `int* stack; // Bottom of the stack`
* `ThreadStatus status; // ready, running or blocked`
* `char* name;`

#### Links

* [Nachos Threads](https://users.cs.duke.edu/~narten/110/nachos/main/node12.html)
* [Nachos Threads](https://www.ida.liu.se/~TDDI04/material/begguide/roadmap/node12.html)

## Exercise 2: Trace source code

> Read the following code, understand current Nachos thread mechanism
>
> * `code/threads/main.cc`
> * `code/threads/threadtest.cc`
> * `code/threads/thread.h`
> * `code/threads/thread.cc`

Nachos Thread has 4 states (as `enum ThreadStatus`)

* JUST_CREATED
* RUNNING
* READY
* BLOCKED

And default operations

* `void Fork(VoidFunctionPtr func, void *arg); // Make thread run (*func)(arg)`
* `void Yield(); // Relinquish the CPU if any other thread is runnable`
* `void Sleep(); // Put the thread to sleep and relinquish the processor`
* `void Finish(); // The thread is done executing`

## Exercise 3: Expand Thread data structure

> Add two member, *UID* (user id) and *TID* (thread id).
> Maintain these two number using existing Nachos thread management mechanism.

1. Add the **Additional member** and its methods in class `Thread` in file `threads/threads.h`

    ```cpp
    class Thread {
        private:
            // Lab1: Add thread ID and user ID
            int tid;                            // Lab1: Add thread ID
            int uid;                            // Lab1: Add user ID

        public:
            // Lab1: methods to manipulate tid and uid
            int getThreadId() { return (tid); }         // Lab1: Get thread ID
            int getUserId() { return (uid); }           // Lab1: Get user ID
            void setUserId(int userId) { uid = userId; }// Lab1: Set user ID
    }
    ```

2. Declare external variable in `threads/system.h`

    ```cpp
    // Lab1: Thread manipulation variable
    #define MAX_THREAD_NUM 128
    extern bool tid_flag[MAX_THREAD_NUM];
    ```

    > This variable is used to check which ID is taken up by a thread.
    >
    > the MAX_THREAD_NUM is according to Exercise 4

3. Initialize it in function `void Initialize()` in file `threads/system.cc`

    ```cpp
    void Initialize(int argc, char **argv) {
        // Lab1: Initialize thread variable
        for (int i = 0; i < MAX_THREAD_NUM; i++) {
            tid_flag[i] = FALSE;
        }
    }
    ```

4. Add the thread ID allocation mechanism in `Thread::Thread` in file `threads/threads.cc`

    ```cpp
    Thread::Thread(char* threadName)
    {
        // Lab1: Allocate thread id for current thread
        for (int i = 0; i < MAX_THREAD_NUM; i++) { // sequential search
            if (!tid_flag[i]) { // if found an empty space
                this->tid = i;
                tid_flag[i] = TRUE;
                break;
            }
        }
    }
    ```

5. Also don't forget to release flag in `Thread::~Thread()`

    ```cpp
    Thread::~Thread()
    {
        // Lab1: Release thread flag
        tid_flag[this->tid] = FALSE;
    }
    ```

### Test Result

1. In `threads/threadtest.cc` add two functions

    ```cpp
    //----------------------------------------------------------------------
    // Lab1Exercise3Thread
    // 	Loop 5 times, yielding the CPU to another ready thread 
    //	each iteration.
    //
    //	"which" is simply a number identifying the thread, for debugging
    //	purposes.
    //---------------------------------------------------------------------

    void
    Lab1Exercise3Thread(int which)
    {
        int num;

        for (num = 0; num < 5; num++) {
        printf("*** thread %d (uid=%d, tid=%d) looped %d times\n", which, currentThread->getUserId(), currentThread->getThreadId(), num);
            currentThread->Yield();
        }
    }
    ```

    ```cpp
    //----------------------------------------------------------------------
    // Lab1 Exercise3
    // 	Create multi-threads and show its uid and tid
    //----------------------------------------------------------------------

    void
    Lab1Exercise3()
    {
        DEBUG('t', "Entering Lab1Exercise3");

        const int max_threads = 5;
        const int test_uid = 87;

        for (int i = 0; i < max_threads; i++) {
            // Generate a Thread object
            Thread *t = new Thread("forked thread");
            t->setUserId(test_uid); // set uid
            // Define a new thread's function and its parameter
            t->Fork(Lab1Exercise3Thread, (void*)t->getThreadId());
        }

        Lab1Exercise3Thread(0);
    }
    ```

2. Add `Lab1Exercise3()` into `ThreadTest()` (as case 2)

**Result**:

* `uid` is assigned to 87
* `tid` is auto allocated

```sh
$ threads/nachos -q 2
Lab1 Exercise3:
*** thread 0 (uid=0, tid=0) looped 0 times
*** thread 1 (uid=87, tid=1) looped 0 times
*** thread 2 (uid=87, tid=2) looped 0 times
*** thread 3 (uid=87, tid=3) looped 0 times
*** thread 4 (uid=87, tid=4) looped 0 times
*** thread 5 (uid=87, tid=5) looped 0 times
*** thread 0 (uid=0, tid=0) looped 1 times
*** thread 1 (uid=87, tid=1) looped 1 times
*** thread 2 (uid=87, tid=2) looped 1 times
*** thread 3 (uid=87, tid=3) looped 1 times
*** thread 4 (uid=87, tid=4) looped 1 times
*** thread 5 (uid=87, tid=5) looped 1 times
*** thread 0 (uid=0, tid=0) looped 2 times
*** thread 1 (uid=87, tid=1) looped 2 times
*** thread 2 (uid=87, tid=2) looped 2 times
*** thread 3 (uid=87, tid=3) looped 2 times
*** thread 4 (uid=87, tid=4) looped 2 times
*** thread 5 (uid=87, tid=5) looped 2 times
*** thread 0 (uid=0, tid=0) looped 3 times
*** thread 1 (uid=87, tid=1) looped 3 times
*** thread 2 (uid=87, tid=2) looped 3 times
*** thread 3 (uid=87, tid=3) looped 3 times
*** thread 4 (uid=87, tid=4) looped 3 times
*** thread 5 (uid=87, tid=5) looped 3 times
*** thread 0 (uid=0, tid=0) looped 4 times
*** thread 1 (uid=87, tid=1) looped 4 times
*** thread 2 (uid=87, tid=2) looped 4 times
*** thread 3 (uid=87, tid=3) looped 4 times
*** thread 4 (uid=87, tid=4) looped 4 times
*** thread 5 (uid=87, tid=5) looped 4 times
```

## Exercise 4: Add global Thread management mechanism

> 1. Make Nachos able to handle maximum 128 threads at the same time.
> 2. Imitate Linux *ps* (Process Status) command. Add a *ts* (Threads Status) command which is able to show all the threads' information and status.

### 4-1 Maximum threads limit

Change `Thread::Thread` in file `threads/threads.cc` into this

```cpp
Thread::Thread(char* threadName)
{
    // Lab1: Allocate thread id for current thread
    bool success_allocate = FALSE;
    for (int i = 0; i < MAX_THREAD_NUM; i++) { // sequential search
        if (!tid_flag[i]) { // if found an empty space
            this->tid = i;
            tid_flag[i] = TRUE;
            success_allocate = TRUE;
            break;
        }
    }
    if (!success_allocate) {
        printf("Reach maximum threads number %d, unable to allocate!!", MAX_THREAD_NUM);
    }
    ASSERT(success_allocate); // abort if unable to allocate new thread

    name = threadName;
    stackTop = NULL;
    stack = NULL;
    status = JUST_CREATED;
```

**Testing time**:

Add the following funciton in `threads/threadtest.cc` and don't forget to add `Lab1Exercise4_1()` into `ThreadTest()` (as case 3)

```cpp
//----------------------------------------------------------------------
// Lab1 Exercise4-1
// 	Create many threads until reach the maximum MAX_THREAD_NUM
//----------------------------------------------------------------------

void
Lab1Exercise4_1()
{
    DEBUG('t', "Entering Lab1Exercise4_1");

    const int max_threads = 130;

    for (int i = 0; i < max_threads; i++) {
        // Generate a Thread object
        Thread *t = new Thread("forked thread");
        printf("*** thread name %s (tid=%d)\n", t->getName(), t->getThreadId());
    }
}
```

```sh
$ threads/nachos -q 3
Lab1 Exercise4-1:
*** thread name forked thread (tid=1)
*** thread name forked thread (tid=2)
*** thread name forked thread (tid=3)
*** thread name forked thread (tid=4)

...

*** thread name forked thread (tid=124)
*** thread name forked thread (tid=125)
*** thread name forked thread (tid=126)
*** thread name forked thread (tid=127)
Reach maximum threads number 128, unable to allocate!!
Assertion failed: line 50, file "../threads/thread.cc"
```

### 4-2 ps-like ts command

Some clue

* *Ready queue* is defined in `threads/scheduler.cc` as a private List variable `List *readyList`

1. Add `tid_pointer` array with `Thread*` type to record all the threads' pointer in `threads/system.h` and `threads/system.c`. It's similar with `tid_flag` in Exercise 3.
2. Add `getThreadStatus()` public method in `threads/thread.h`. It's similar with step 1 in Exercise 3.
3. The TS Function

    ```cpp
    //----------------------------------------------------------------------
    // TS command
    // 	Showing current threads' status (like ps in Linux)
    //----------------------------------------------------------------------

    void
    TS()
    {
        DEBUG('t', "Entering TS");

        const char* TStoString[] = {"JUST_CREATED", "RUNNING", "READY", "BLOCKED"};

        printf("UID\tTID\tNAME\tSTATUS\n");
        for (int i = 0; i < MAX_THREAD_NUM; i++) { // check pid flag
            if (tid_flag[i]) {
                printf("%d\t%d\t%s\t%s\n", tid_pointer[i]->getUserId(), tid_pointer[i]->getThreadId(), tid_pointer[i]->getName(), TStoString[tid_pointer[i]->getThreadStatus()]);
            }
        }
    }
    ```

**Testing time**:

Add the following funciton in `threads/threadtest.cc` and don't forget to add `Lab1Exercise4_2()` into `ThreadTest()` (as case 4)

```cpp
//----------------------------------------------------------------------
// CustomThreadFunc
//
//	"which" is simply a number identifying the operation to do on current thread
//----------------------------------------------------------------------

void
CustomThreadFunc(int which)
{
    printf("*** current thread (uid=%d, tid=%d, pri=%d name=%s) => ", currentThread->getUserId(), currentThread->getThreadId(), currentThread->getPriority(), currentThread->getName());
    IntStatus oldLevel; // for case 1 sleep (avoid cross initialization problem of switch case)
    switch (which)
    {
        case 0:
            printf("Yield\n");
            scheduler->Print();
            printf("\n\n");
            currentThread->Yield();
            break;
        case 1:
            printf("Sleep\n");
            oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
            currentThread->Sleep();
            (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
            break;
        case 2:
            printf("Finish\n");
            currentThread->Finish();
            break;
        default:
            printf("Yield (default)\n");
            currentThread->Yield();
            break;
    }
}
```

```cpp
//----------------------------------------------------------------------
// Lab1 Exercise4-2
// 	Create some threads and use TS to show the status
//----------------------------------------------------------------------

void
Lab1Exercise4_2()
{
    DEBUG('t', "Entering Lab1Exercise4_2");

    Thread *t1 = new Thread("fork 1");
    Thread *t2 = new Thread("fork 2");
    Thread *t3 = new Thread("fork 3");

    t1->Fork(CustomThreadFunc, (void*)0);
    t2->Fork(CustomThreadFunc, (void*)1);
    t3->Fork(CustomThreadFunc, (void*)2);

    Thread *t4 = new Thread("fork 4");

    CustomThreadFunc(0); // Yield the current thread (i.e. main which is defined in system.cc)

    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n");
}
```

```sh
$ threads/nachos -q 4
Lab1 Exercise4-2:
*** current thread (uid=0, tid=0, name=main) => Yield
*** current thread (uid=0, tid=1, name=fork 1) => Yield
*** current thread (uid=0, tid=2, name=fork 2) => Sleep
*** current thread (uid=0, tid=3, name=fork 3) => Finish
--- Calling TS command ---
UID     TID     NAME    STATUS
0       0       main    RUNNING
0       1       fork 1  READY
0       2       fork 2  BLOCKED
0       4       fork 4  JUST_CREATED
--- End of TS command ---
```

> Result may be a little bit different because of
> Lab2 has added the priority to Thread and has shared same funciton.
> (Thread handler & TS command)

## Trouble Shooting

### Undefined Reference to Extern Variable

***Variable is declared but not defined***!!

As an exception, when an extern variable is declared with initialization, it is taken as the definition of the variable as well

* [**GeeksforGeeks - Understanding “extern” keyword in C**](https://www.geeksforgeeks.org/understanding-extern-keyword-in-c/)
* [**Stackoverflow - Create extern char array in C**](https://stackoverflow.com/questions/7670816/create-extern-char-array-in-c)
* [Stackoverflow - undefined reference when using extern](https://stackoverflow.com/questions/3658490/undefined-reference-when-using-extern/17694703)

## Resources

### Example

* [CSDN - nachos lab1-線程機制](https://blog.csdn.net/wyxpku/article/details/52076236)
* [CSDN - 線程機制實習報告_Nachos Lab1](https://blog.csdn.net/superli90/article/details/29369909)

- [CSDN 操作系統課程設計 -nachos- lab-new1](https://blog.csdn.net/wangshang4133/article/details/78627714)

### Article

* [Shichao's Notes - Chapter 3. Process Management - The Process](https://notes.shichao.io/lkd/ch3/#the-process)
