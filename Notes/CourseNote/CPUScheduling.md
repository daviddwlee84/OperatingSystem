# CPU Scheduling

## Overview

* CPU Scheduling Questions
  * What: based on what principle to deploy resource to process/thread
    * Scheduling Algorithm
  * When: when to distribute CPU
    * Scheduling Timing
  * How: how to distribute CPU
    * Scheduling Progress (process context switching)

* [Wiki - Scheduling (computing)](https://en.wikipedia.org/wiki/Scheduling_(computing))

### Classic Events

* Create, Resume, Terminate
* I/O waiting, I/O interrupt
* Timeout interrupt
* Error abort

### Short-term, Medium-term, Long-term Scheduling

![x-term scheduling](https://ccsindian.files.wordpress.com/2017/03/wp-1490610519616.png)

* Long-term (Task, Macro) Scheduling
  * Determine whether put a process into "active set" when create it.
* Medium-term Scheduling (SWAP)
  * Swtiching a process between memory and disk
* Short-term (Micro) Scheduling
  * Pick a process in ready queue to CPU
  * Usually in millisecond
  * Must be efficient

### Scheduling Timing

* Events
  * Process control operations
    * Create
    * Wakeup
    * Terminate
  * Waiting for I/O (the I/O interrupt)
  * Time interrupt
    * Running out of the *time slices*
  * Some error happen when executing program
    * Abort exception
* Interrupt, Exception, Trap
* System call

## Process Context Switch

[Process Context Switch](Process#Context-Switch)

* Where is the information (context) of the thread?
  * The thread who is going to release CPU
    * From kernel (system) stack to PCB
  * The thread who is going to use CPU
    * In PCB

### Procedure of Context Switch

1. Switch global page table (to load a new address space)
2. Switch kernel stack and hardware context (i.e. CPU related registers)

### Cost of Context Switch

* Direct Cost
  * Store and recover register
  * Change address space (the related command is more expensive)
* Indirect Cost (because of using different address space)
  * Cache fail
  * Buffer Cache fail
  * TLB (Translation Look-aside Buffer) fail
    * We have to clean the buffer since we changed the address space when context switch

> About the indirect cost, just check out the [virtual memory notes](MemoryManagement.md)

* [Wiki - Translation lookaside buffer](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)
* [Youtube - Virtual Memory: 11 TLB Example](https://youtu.be/95QpHJX55bM)

## Scheduling Criteria

* Throughput
  * The finished process amount in a time unit
  * throughput = (# of tasks) / (total seconds)
* Turnaround Time (TT)
  * The total time from every thread request to finish
  * TT for each process/thread = summation of each thread finish time - request time
  * Average TT: (summation of all TT) / (total seconds)
* Response Time (RT)
  * The time from request to first response

Others

* CPU Utilization
  * Time ratio of CPU doing valid job
* Waiting Time
  * Process in *ready queue*

The different goals for the different OS types

* interactive process
* batch process
* real-time process

## Scheduling Concentration

* PCB information for scheduling
* priority and ready queue
* preemptive vs. non-preemptive
* I/O bound vs. CPU-bound process
* time slice

### Which information to store in PCB to help CPU scheduling

* Priority
  * static priority
    * determined during creation
  * dynamic priority
    * priority may change during execution
* State

### Priority and structure of ready queue

![multiple blocked queues](https://bethycotter.wdfiles.com/local--resized-images/scheduling/scheduling%20queue.png/medium.jpg)

### Preemptive vs. Nonpreemptive

Preemptive: a process with higher priority can preempt the CPU from other running process

Non-preemptive: CPU resource can only give up by the process itself

* [Wiki - Preemption (computing)](https://en.wikipedia.org/wiki/Preemption_(computing))

### I/O bound vs. CPU-bound

I/O-bound: doing I/O frequently

CPU-bound: need lots of CPU time for calculation

### Time Slice / Quantum

A time duration that a process can running on a CPU each scheduling

![quantums](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter6/6_04_quantums.jpg)

## Scheduling Algorithms

Determining *through put*, *turnaround time*, *CPU utilization*, *fairness* (*starvation*)

> Recall the [Scheduling Criteria](#Scheduling-Criteria)

* First Come First Service
* Shortest Job First
* Shortest Remain Time Next
* Highest Response Ratio Next

In interactive system:

* Round Robin
* Highest Priority First
* Multiple queues and Multiple feedback queue
* Shortest Process Next
* (others)
  * Fair-share
  * Guaranteed
  * Lottery

### FCFS, First Come First Service

> The most naive algorithm

### SJF, Shortest Job First

* Finish the shortest job => improve the turnaround time
* Pros and Cons
  * Cons
    * Not fair, may cause starvation
    * You can't not predict future

### SRTN, Shortest Remain Time Next

> The preemptive version of SJF

### HRRN, Highest Response Ratio Next

### RR, Round Robin

> To improve the average response time

* Time interrupt to trigger context switch

[Time Slice / Quantum](#Time-Slice-/-Quantum)

* too long
  * become FCFS
  * make the short process response time longer
* too short
  * too many context switch

#### Virtual RR

Auxiliary Queue

### HPF, Highest Priority First

* System process > User process
* Front-end process > Back-end process
* I/O-bound process > CPU-bound process

#### Priority Inversion Problem

If a low priority process occupy a resource that a high priority process need but the low priority process can't get the CPU after preemption. So the resource can't be release, and because the high priority process can't get the resource so it can't run either.

Effect

* System error
* Low performance (because the high priority process cannot run)

Solutions

* Priority ceiling protocol
* Priority inherit
* Use interrupt

### Multilevel Queue Scheduling

* There are multiple queues for different priority
* Each queue has a header and follow by some runnable processes

#### Multilevel Feedback Queues

> This is used by BSD (a kind of UNIX)

![Multilevel Feedback Queues](https://images.slideplayer.com/16/5138416/slides/slide_1.jpg)

### Conclusion of the Scheduling Algorithms

Algorithm|Selecting Function|Decision|Throughput|Response Time|Cost|Effect|Starvation
-|-|-|-|-|-|-|-
FCFS|max[w]|non-preemptive|-|may be high if the process time differ|smallest|bad for short, I/O-bound process|No
RR|constant|preemptive (when running out of time slice)|low (if time slice is low)|good for short process|smallest|fair|No
SJF|min[s]|non-preemptive|high|good for short process|higher|bad for long process|probabily
SRTN|min[s-e]|preemptive|high|good|higher|bad for long process|probabily
HRRN|max[(w+s)/s]|non-preemptive|high|good|higher|balance|No
Feedback|(skip)|preemptive|-|-|higher|better for I/O bound-process|probabily

In real world

* UNIX: Dynamic priority
* BSD (5.3): Multilevel feedback queue
* Windows: preemptive multi-task scheduling based on priority
* [Linux: preemptive scheduling](https://github.com/daviddwlee84/LinuxKernel/blob/master/Notes/LinuxProcessScheduling.md)
* Solaris: mixed scheduling algorithm

### Windows Scheduling

Balance set manager: higher the priority of the starved process

Idle thread

## Links

* [What is CPU Scheduling?](https://www.studytonight.com/operating-system/cpu-scheduling)
* [Operating System - Process Scheduling](https://www.tutorialspoint.com/operating_system/os_process_scheduling.htm)

### Book

Operating System Concepts 9ed.

* Notes
  * [CPU Scheduling](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/6_CPU_Scheduling.html)
