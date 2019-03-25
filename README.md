# Operating System

PKU OS course project and notes based on Nachos and XV6.

* Nachos - An immature OS that we can try to add wings on it.
* XV6 - A much more functional OS that we can absorb its essence by tracing code.

## Schedule

### Nachos Lab

Lab  |Subject|Detail
-----|-------|------
Lab 0|[Build Nachos](Lab/Lab0_BuildNachos)|Setup 32bit cross compile dev. env.
Lab 1|[Thread Mechanism](Lab/Lab1_ThreadMechanism/README.md)|Multi-thread management
Lab 2|[Thread Scheduling](Lab/Lab2_ThreadScheduling/README.md)|CPU scheduling
Lab 3|[Synchronization Mechanism](Lab/Lab3_SynchronizationMechanism/README.md)|Concurrency, mutex lock and semaphore

> [**Quick start with docker!! (recommend)**](Lab/Lab0_BuildNachos/DevEnvironment.md#Use-Docker-with-32bit-image)

### XV6 Source Code Reading

1. [Process and Thread (with Scheduling)](Notes/XV6/XV6ProcessThread.md)

## Notes

* [Nachos](Notes/Nachos/Nachos.md)
  * [Debugging Nachos](Notes/Nachos/Nachos.md#Debugging-Nachos)
* [XV6](Notes/XV6/XV6.md)
* [Trace Code](Notes/TraceCode.md)

### Course Notes

* [Process](Notes/CourseNote/Process.md)
* [Thread](Notes/CourseNote/Thread.md)
* [CPU Scheduling](Notes/CourseNote/CPUScheduling.md)
* [Executing Environment](Notes/CourseNote/ExecutingEnvironment.md)
  * Interrupt/Exception/Trap
    * Timer Interrupt
  * System Call
* [Memory Management](Notes/CourseNote/MemoryManagement.md)

## Resources

### Books

* Modern Operating System 4ed.
* Operating System Concepts 9ed.
* Advanced Programming in the UNIX Environment 3ed.
  * [pdf](http://www.codeman.net/wp-content/uploads/2014/04/APUE-3rd.pdf)
  * [Shichao's Notes APUE](https://notes.shichao.io/apue/)
* Computer Systems: A Programmer's Perspective 3ed.

### MOOCs

* PKU Operating System Principle
  * [Coursera - PKU Operating Systems](https://www.coursera.org/learn/os-pku)
  * [Chinese MOOCS - PKU Operating System Principle](http://www.chinesemooc.org/mooc/4747)
* [CMU Introduction to Computer Systems (ICS)](https://www.cs.cmu.edu/~213/)

### Github

* [xmuliushuo/nachos](https://github.com/xmuliushuo/nachos)
* [MrOrz/nachos](https://github.com/MrOrz/nachos)
* [zhanglizeyi/CSE120](https://github.com/zhanglizeyi/CSE120)
  * [**Nachos中文教程.pdf**](https://github.com/zhanglizeyi/CSE120/blob/master/Nachos%E4%B8%AD%E6%96%87%E6%95%99%E7%A8%8B.pdf)

### Others

* [NachOS 3.4 Doxygen Documentation](https://web.ics.purdue.edu/~cs354/Nachos/index.html)
* [How can I merge a branch into master but continue working on the branch?](https://stackoverflow.com/questions/26024586/how-can-i-merge-a-branch-into-master-but-continue-working-on-the-branch)
