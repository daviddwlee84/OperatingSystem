# Deadlock and Starvation

## Concept

### Deadlock

#### 4 Necessary Condition to achieve deadlock

* **Mutual Exclusion**
  * At least one resource must be held in a non-sharable mode
  * If any other process requests this resource, then that process must wait for the resource to be released.
* **Hold and Wait**
  * A process must be simultaneously holding at least one resource and waiting for at least one resource that is currently being held by some other process.
* **No preemption**
  * Once a process is holding a resource ( i.e. once its request has been granted ), then that resource cannot be taken away from that process until the process voluntarily releases it.
* **Circular Wait**
  * A set of processes { P0, P1, P2, . . ., PN } must exist such that every P[ i ] is waiting for P[ ( i + 1 ) % ( N + 1 ) ]. ( Note that this condition implies the *hold-and-wait condition*, but it is easier to deal with the conditions if the four are considered separately. )

### Starvation

### Resource-Allocation Graph (RAG)

![Resource allocation graph](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter7/7_01_ResourceAllocation.jpg)

* a set of **Resource Categories**, { R1, R2, R3, . . ., RN } (which appear as square nodes on the graph)
  * Dots inside the resource nodes indicate specific instances of the resource
* a set of **Processes**, { P1, P2, P3, . . ., PN }
* Edge
  * **Request Edges**
  * **Assignment Edges**

![assignment edge](http://cse.csusb.edu/tongyu/courses/cs460/images/deadlock/dlock0.png)

![request edge](http://cse.csusb.edu/tongyu/courses/cs460/images/deadlock/dlock1.png)

![Resource allocation graph with a deadlock](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter7/7_02_Deadlock.jpg)

![Resource allocation graph with a cycle but no deadlock](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter7/7_03_CycleNoDeadlock.jpg)

#### Simplify RAG

## Solution

The way to not let deadlock happen (Do not allow the system to get into a deadlocked state)

* **Prevention** 預防: Static Strategy
  * Design a resource allocation algorithm
* **Avoidance** 避免: Dynamic Strategy
  * Keep track of the resource allocation status, and allocate the resource based on the decision

Other possible solution (but deadlock may happen)

* **Ignoring Deadlock**: Ostrich Algorithm 鴕鳥演算法
  * If deadlocks only occur once a year or so, it may be better to simply let them happen and *reboot as necessary* than to incur the constant overhead and system performance penalties associated with deadlock prevention or detection.
  * This is the approach that both Windows and UNIX take.
* **Detect and Recover**: Deadlock Detection Algorithm
  * Abort a process or preempt some resources when deadlocks are detected

### Prevention

> Deadlocks can be prevented by preventing at least one of the four required conditions

Key: To break one of the ["4 necessary condition"](#4-necessary-condition-to-achieve-deadlock)

### Avoidance

Safe State

![Safe, unsafe, and deadlocked state spaces](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter7/7_06_StateSpaces.jpg)

#### RAG Algorithm

TBD

#### Banker's Algorithm

TBD

### Detection and Recovery

TBD

Basic approaches to recovery from deadlock:

* Inform the system operator, and allow him/her to take manual intervention
* Terminate one or more processes involved in the deadlock
* Preempt resources

## Example

### The Dining-Philosophers Problem

![The Dining-Philosophers Problem](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_13_DiningPhilosophers.jpg)

![dine cycle](http://cse.csusb.edu/tongyu/courses/cs460/images/dining/dine-cycle.jpg)

![dine flow](http://cse.csusb.edu/tongyu/courses/cs460/images/dining/dine-flow.jpg)

![deadlock](http://cse.csusb.edu/tongyu/courses/cs460/images/dining/dine-deadlock.jpg)

![starvation](http://cse.csusb.edu/tongyu/courses/cs460/images/dining/dine-starve.jpg)

![The structure of philosopher i](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter5/5_14_Philosopher_i.jpg)

### Traffic

![traffic](http://cse.csusb.edu/tongyu/courses/cs460/images/deadlock/crash.gif)

![deadlock](http://cse.csusb.edu/tongyu/courses/cs460/images/deadlock/deadlock_possible.jpg)

## Resources

* [Wiki - Deadlock](https://en.wikipedia.org/wiki/Deadlock)
  * [Wiki - Ostrich algorithm](https://en.wikipedia.org/wiki/Ostrich_algorithm)
* [CSE 460 - Deadlocks](http://cse.csusb.edu/tongyu/courses/cs460/notes/deadlck.php)

### Book

Operating System Concepts 9ed.

* Ch7 Deadlock
  * Ch7.2 Deadlock Characterization
    * Ch7.2.1 Necessary Conditions
    * Ch7.2.2 Resource-Allocation Graph
  * Ch7.3 Methods for Handling Deadlocks
  * Ch7.4 Deadlock Prevention
  * Ch7.5 Deadlock Avoidance
    * Ch7.5.1 Safe State
    * Ch7.5.2 Resource-Allocation Graph Algorithm
    * Ch7.5.3 Banker's Algorithm
  * Ch7.6 Deadlock Detection
  * Ch7.7 Recovery From Deadlock
* Notes
  * [Deadlock](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/7_Deadlocks.html)
