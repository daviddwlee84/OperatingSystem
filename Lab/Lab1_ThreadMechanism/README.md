# Lab 1: Thread Mechanism

## Exercise 1: Research

> Difference between Linux PCB and Nachos mechanism

## Exercise 2: Trace source code

> Read the following code, understand current Nachos thread mechanism
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

3. Initialize it in function `void Initialize()` in file `threads/system.cc`

    ```cpp
    void Initialize(int argc, char **argv) {
        // Lab1: Initialize thread variable
        for (int i = 0; i < 128; i++) {
            tid_flag[i] = FALSE;
        }
    }
    ```

4. Add the thread ID allocation mechanism in `Thread::Thread` in file `threads/threads.cc`

    ```cpp
    Thread::Thread(char* threadName)
    {
        // Lab1: Allocate thread id for current thread
        for (int i = 0; i < 128; i++) { // sequential search
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

## Exercise 4: Add global Thread management mechanism

> 1. Make Nachos can handle maximum 128 threads at the same time.
> 2. Imitate Linux *ps* command. Add a *ts* (Threads Status) command which is able to show all the threads' information and status.

## Trouble Shooting

### Undefined Reference to Extern Variable

* [Stackoverflow - undefined reference when using extern](https://stackoverflow.com/questions/3658490/undefined-reference-when-using-extern/17694703)

## Resources

### Example

* [CSDN - nachos lab1-線程機制](https://blog.csdn.net/wyxpku/article/details/52076236)
* [CSDN - 線程機制實習報告_Nachos Lab1](https://blog.csdn.net/superli90/article/details/29369909)

- [CSDN 操作系統課程設計 -nachos- lab-new1](https://blog.csdn.net/wangshang4133/article/details/78627714)
