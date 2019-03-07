# Lab 1: Thread Mechanism

## Exercise 1: Research

> Difference between Linux PCB and Nachos mechanism

## Exercise 2: Trace source code

> Read the following code, understand current Nachos thread mechanism
>
> * `code/threads/main.cc`
> * `code/threads/threadtest.cc`
> * `code/threads/thread.h`
> * `code/threads/thread.cc`

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
> 2. Imitate Linux *ps* command. Add a *ts* (Threads Status) command which is able to show all the threads' information and status.

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
