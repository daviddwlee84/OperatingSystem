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

## Process Context Switch

[Process Context Switch](Process#Context-Switch)

* Where is the information (context) of the thread?
  * The thread who is going to release CPU
    * From kernel (system) stack to PCB
  * The thread who is going to use CPU
    * In PCB

### Cost of Context Switch

* Direct Cost
  * Store and recover register
  * Change address space (the related command is more expensive)
* Indirect Cost
  * Cache fail
  * Buffer Cache fail
  * TLB (Translation Look-aside Buffer) fail
    * We have to clean the buffer since we changed the address space when context switch

* [Wiki - Translation lookaside buffer](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)
* [Youtube - Virtual Memory: 11 TLB Example](https://youtu.be/95QpHJX55bM)

## Scheduling Criteria

* Throughput
* Turnaround Time (TT)
* Response Time (RT)

Others

* CPU Utilization
* Waiting Time
  * Process in *ready queue*

The difference goal for different type OS

* interactive process
* batch process
* real-time process

## Scheduling Concentration

### Which information to store in PCB to help CPU scheduling

### Priority and structure of ready queue

### Preemptive vs. Nonpreemptive

* [Wiki - Preemption (computing)](https://en.wikipedia.org/wiki/Preemption_(computing))

### I/O bound vs. CPU-bound

### Time Slice

## Scheduling Algirhtms

TBD

### RR, Round Robin

#### Virtual RR

Auxiliary Queue

### HPF, Highest Priority First

#### Priority Inversion Problem

### Multilevel Queue Scheduling

#### Multilevel Feedback Queues

## Links

* [What is CPU Scheduling?](https://www.studytonight.com/operating-system/cpu-scheduling)
