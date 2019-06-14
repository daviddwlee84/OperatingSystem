# XV6 Source Code Reading -- Synchronization

## Requirement

Code to read

* Locks
  * [ ] `spinlock.h` (p.15)
  * [ ] `spinlock.c` (p.15)

## Resources

xchg

* [XCHG — Exchange Register/Memory with Register](https://www.felixcloutier.com/x86/xchg)
* [X86組合語言/基本指令集/IA32指令:xchg](https://zh.wikibooks.org/zh-tw/X86%E7%B5%84%E5%90%88%E8%AA%9E%E8%A8%80/%E5%9F%BA%E6%9C%AC%E6%8C%87%E4%BB%A4%E9%9B%86/IA32%E6%8C%87%E4%BB%A4:xchg)

### Implementation

implement semaphore using spinlock

implement readers-writer lock using spinlock

* [Readers–writer lock - Implementation](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock#Implementation)
* [How would you implement your own reader/writer lock in C++11?](https://stackoverflow.com/questions/12033188/how-would-you-implement-your-own-reader-writer-lock-in-c11)
