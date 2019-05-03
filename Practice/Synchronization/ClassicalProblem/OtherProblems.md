# Classical Synchronization Problem

Table of Content

| English                                                                                     | Chinese                    | Detail |
| ------------------------------------------------------------------------------------------- | -------------------------- | ------ |
| [Pure synchronization problem](#Pure-Synchronization-Problem)                               | 純同步問題                 |
| [Another PV operation problem](#Another-PV-Operation-Problem)                               | 另類PV操作問題             |
| [Reading Room problem](ReadingRoomMonitor.md)                                               | 閱覽室問題                 |
| [Food supply problem](#Food-Supply-Problem)                                                 | 食品供應問題               |
| [Three Gorges Dam problem](#Three-Gorges-Dam-Problem)                                       | 三峽大壩問題               |
| [Sleeping barber problem](#Sleeping-Barber-Problem)                                         | 睡眠理髮師問題             |
| [Second readers-writers problem](#Second-Readers-Writers-Problem)                           | 第二類讀者寫者問題         |
| [Complex message buffer problem](#Complex-Message-Buffer-Problem)                           | 複雜的消息緩衝問題         |
| [Examinaiton room problem](#Examinaiton-Room-Problem)                                       | 考場問題                   |
| [Monkey across valley problem](#Monkey-Across-Valley-Problem)                               | 狒狒過峽谷問題             |
| [Bank teller problem](#Bank-Teller-Problem)                                                 | 銀行櫃員問題               |
| [Resource management problem using semaphore](#Resource-Management-Problem-using-Semaphore) | 利用信號量解決資源管理問題 |

## Pure Synchronization Problem

There are many process with a sort of sequence/dependency.

The relationship can be represent as a directed acyclic graph.

### Solution for Pure Synchronization Problem

#### Passing Semaphore

Consider each vertex.

The processes which are waiting on the vetex are waiting all the processes which are going to go in to the vertex.

#### Main Thread

Use main thread/process to manipulate which children thread can go.

### Deadlock

If the directed graph is not acyclic (there is a cycle).

That is, for any vertex, the semaphore that it release become the semaphore that it waits.

## Another PV Operation Problem

The operation of semaphore P is the same.

```txt
P(semaphore):
    semaphore.count -= 1
    if semaphore < 0:
        Put current thread to the end of the waiting queue

V(semaphore):
    semaphore.count += 1
    if semaphore <= 0:
        Wake a process from the "tail of the waiting queue"
```

1. Is there any problem with this definiiton?
   * Yes, this will cause starvation
2. Implement a mutual exclusion mechanism for N processes share/competing a shared variable.
3. Is there any other method than previous solution that is more efficiency? Time complexity?

### Solution for 2

#### Use Semaphore Array (Queue)

#### Use N-1 Semaphore Array

### Solution for 3

Use only $\log_2 N$ semaphore

## Food Supply Problem

* There are two kinds of food a, b produced by company A and B (producer)
* There is a store that will import and keep the foods from both company but if $|a-b| < k$ then must stop import more food from the more-amount one.
* Customer will go to the store buy food (consumer)

> This problem is a sort of producer-consumer problem

### Solution for Food Supply Problem using Semaphore

There are 6 synchronization problem => thus need 6 semaphore.

* Company A with Store: 2
* Company B with Store: 2
* (a-b) < k: 1
* (a-b) > k: 1

## Three Gorges Dam Problem

> Complex version of [Monkey Across Valley Problem](#Monkey-Across-Valley-Problem)

* There are 5 gates.
* Each time can only pass one direction.
* Each gate can only contain one ship.

> This problem is a sort of second readers-writers problem
>
> * Two reader:
>   1. from up to down
>   2. from down to up

### Solution for Three Gorges Dam Problem using Semaphore

This solution need 7 semaphore

* For each gate: 5
* Direction: 1
* Protect the direction counter: 2 (top to bottom and bottom to top)

> The soluiton cannot prevent starvation. (need to add more condiiton to solve it.)

## Sleeping Barber Problem

* There is a barber. A barber's char in the cutting room. N chairs in waiting room.
* If no customer then barber go to sleep.
* The first customer need to wake the barber up.
* If a customer come while the barber is cutting hair
  * If there is an empty seat then sit and wait
  * If not then leave

* [Wiki - Sleeping barber problem](https://en.wikipedia.org/wiki/Sleeping_barber_problem)

### Solution for Sleeping Barber Problem using Semaphore

## Second Readers-Writers Problem

> i.e. The writer preffered solution

## Complex Message Buffer Problem

* There are k buffers. m sending processes (producer). n receiving processes (consumer).
* Each receiving processing need to access all the received message.

## Examinaiton Room Problem

* There is a teacher and N student.
* The door of examinaiton room can only enter/exit one person at a time. First arrived first in.
* The teacher can give out the test paper only when all the student is in the room.
* Each student can leave the room when he finish the exam.
* Teach can leave the room when receiving all the test papers.

## Monkey Across Valley Problem

> Simplify version of [Three Gorges Dam Problem](#Three-Gorges-Dam-Problem)

* At a time, monkey can only pass the valley in one direction.
* Each time there can be multiple mokey combin on the rope.

1. Find a solution without deadlock
2. Find a solution that can prevent starvation

## Bank Teller Problem

* There are N bank tellers.
* Each customer enter the bank need to get a number and waiting for the number to be announced.
* If a teller is idle then announce next number.

## Resource Management Problem using Semaphore

Consider the following feature of the shared resource

* When the process using the shared resorces require less than 3 hours. Then the process which is asking for the resource can get the resource immediately.
* The shared resource can be used by 3 process at a time.
* When resources are occupied by 3 process. Only when all the process release the resource, the other processes can request the resource.

Original version pseudocode:

```c
semaphore mutex = 1, block = 0;
int active = 0, waiting = 0;
boolean must_wait = false;

P(mutex);
if (must_wait) {
    waiting++;
    V(mutex);
    P(block);
    P(mutex);
    waiting--;
}
active++;
must_wait = (active == 3)
V(mutex);

// Critical Region

P(mutex);
active--;
if (active == 0) {
    int n;
    if (waiting < 3) n = waiting;
    else n = 3;
    while (n > 0) {
        V(block);
        n--;
    }
    must_wait = false;
}
V(mutex);
```

Question

1. There is some error in this pseudocode
2. If replaced the `if(must_wait)` to `while(must_wait)`, will it solve the problem? Is there any problem left?

### Solution: I'll Do it for you

### Solution: Pass the Baton
