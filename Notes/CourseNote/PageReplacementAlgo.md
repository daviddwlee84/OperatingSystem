# Page Replacement Algorithm

## Overview

**Local Page Replacement Algorithm**: Selecting a page from a faulting process

* FIFO Page Replacement (simplest)
  * Second-Chance Algorithm (Additional-Reference-Bits Algorithm)
* LRU Page Replacement (closest to Optimal Page Replacement)
* LRU-Approximation Page Replacement
  * NFU Page Replacement (Counting-Based Page Replacement)
  * Aging 老化算法
* Page-Buffering Algorithms

## FIFO

## SCR Second-Chance Replacement Algorithm

> A modified form of the FIFO

It checks to see if its **referenced bit** (R, 訪問位) is set. If it is not set, the page is swapped out.
Otherwise, the referenced bit is cleared, the page is inserted at the back of the queue (as if it were a new page) and this process is repeated

## Clock (implementation)

> This is not a independent algorithm. But a kind of implementation that work with other algorithm.

keeps a circular list of pages in memory

## NRU Not Recently Used

Use **referenced bit (R)** and **modified bit (M)** to simulate the scenario.

OS divide pages into 4 classes:

* referenced, modified (3: (1, 1))
* referenced, not modified (2: (1, 0))
* not referenced, modified (1: (0, 1))
* not referenced, not modified (0: (0, 0))

Replace the smallest class number pages.

## LRU Least Recently Used

> Has the closest performance to OPT. But calculation cost is expensive. (pure LRU is not practical) => approximate with other algorithm (like NFU, aging, etc.)

### LRU hardware implementation

## NFU Not Frequently Used

> Approximate implementation of LRU

1. Use a **counter** for each page. (initial 0)
2. When timer interrupt. `counter += R` (R: referenced bit (0 or 1))
3. When page fault => replace the page with the lowest counter number.

## Aging

> Improved NFU. Also used to approximate LRU

1. Counter shift right 1 bit (i.e. ÷2). (use arithmetic shift operator)
2. Insert **R** at the left most bit.

> "Aging" the effect of the *previous reference*

## Working Set Model

> The working set of a process is the set of pages expected to be used by that process during some time interval.
>
> The "working set model" isn't a page replacement algorithm in the strict sense (it's actually a kind of medium-term scheduler)

* Locality of reference

> It's designed to gratly reduce the page fault rate

### Backbround - Page Fault

Factors which may cause page fault

* Page replacement algorithm
* Page size
* Programming
  * Follow the hardware design mechanism, take its advantage
* Allocation of physical page for each process

#### Relationship between page fault rate and frame allocated

![Pagefault and working set size](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTVSTtdidsrBxNvMZT3T3Nvz5mUzrfUA1pQN7yLjzDhhW_6zvgCjw)

![Figure 9.21 - Page-fault frequency.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_21_PageFaultFrequency.jpg)

### Backbround - Thrashing

> A program causing page faults every few instructions is said to be **thrashing**

If processes are utilizing all main memory and need additional memory pages, a cascade of severe cache misses known as page faults will occur, often leading to a noticeable lag in operating system responsiveness. This process together with the futile, repetitive page swapping that occurs are known as "thrashing".

> Spending lots of time on page replacement, even more than actual running time.

![Figure 9.18 - Thrashing](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_18_Thrashing.jpg)

Example: how many page fault?

for a array A `A[1024][1024]`.

* system store array in row
* page size 4KB

```c
for (int j = 0; j < 1024; j++)
  for (int i = 0; i < 1024; i++)
    A[i][j];
```

```c
for (int i = 0; i < 1024; i++)
  for (int j = 0; j < 1024; j++)
    A[i][j];
```

### Working Set

> Working set model is aiming to solve the **thrashing** problem.

Definitions:

* △ = working-set window = a fixed number of page references
  * e.g. 10000 instructions
  * it's a moving window
  * e.g. (in picture) working-set at t₁ is {1, 2, 4, 6, 7} and at t₂ is {3, 4}
    * (The number is the Process id)
* WSSᵢ (working set of Process Pᵢ) = total number of pages referenced in the most recent △ (varies in time)
  * if △ too small => will not encompass entire locality
  * if △ too large => will overlap several localities
  * if △ = ∞ => will encompass entire program
* m = memory = total available frames
* D = Σ WSSᵢ = total demand frames
  * Approximation of locality
  * if D > m => Thrashing

Policy: if D > m, then suspend or swap out one of the processes

![Figure 9.20 - Working-set model.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_20_WorkingSetModel.jpg)

Approximate working-set model with a **interval timer** + a **reference bit**

* Example: △ = 10000 references
  * Timer interrupts after every 5000 time units
  * Keep 2 *in memory* bits for each page
  * Whenever a timer interrupts copy and sets the values of all reference bits to 0
  * If one of the bits *in memory* = 1 => page in working set

### More direct approach

Establish "acceptable" page-fault frequency (PFF) rate and use local replacement policy

* If actual rate too low, process loses frame
* If actual rate too high, process gains frame

![page-fault rate and the working-set](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_22_PageFaultRate.jpg)

> There is a direct relationship between the page-fault rate and the working-set, as a process moves from one locality to another:

* Working set changes over time
* Peaks an valleys over time

### Working-set based Implementation

TBD (Modern Operating System 4ed. P.218)

![Figure 3-19. The working set algorithm](https://slideplayer.com/slide/1659396/7/images/24/Working+set+algorithm+age+%3D+current+virtual+time+%E2%80%93+time+of+last+use.jpg)

## Summary and Conclusion

| Algorithm     | Comment                                                        |
| ------------- | -------------------------------------------------------------- |
| Optimal       | Not implementable, but useful as a bench mark                  |
| NRU           | Very crude approximation of LRU                                |
| FIFO          | Might throw out important pages                                |
| Second chance | Big improvement over FIFO                                      |
| Clock         | Realistic                                                      |
| LRU           | Excellent (almost optimal), but difficult to implement exactly |
| NFU           | Fairly crude approximation to LRU                              |
| Aging         | Efficient algorithm that approximates LRU well                 |
| Working set   | Somewhat expensive to implement                                |
| WSClock       | Good efficient algorithm                                       |

### Missing Page Exception Handling

### Conclusion

It's all about **Demand Paging**. Because we load the page only when we need it. So when we demand the page in first time, it will cause a page fault (a page that is currently not in memory is requested).

Because loading page from disk (secondary memory) to available page frame in memory is very slow. We need some efficient algorithm for page replacement.

## Resources

### Book

Modern Operating System 4ed.

* Ch3 Memory Management
  * Ch3.4 Page Replacement Algorithms
    * Ch3.4.1 The Optimal Page Replacement Algorithm
    * Ch3.4.2 The Not Recently Used Page Replacement Algorithm
    * Ch3.4.3 The First-In, First-Out (FIFO) Page Replacement Algorithm
    * Ch3.4.4 The Second-Chance Page Replacement Algorithm
    * Ch3.4.5 The Clock Page Replacement Algorithm
    * Ch3.4.6 The Least Recently Used (LRU) Page Replacement Algorithm
    * Ch3.4.7 Simulating LRU in Software
    * Ch3.4.8 The Working Set Page Replacement Algorithm
    * Ch3.4.9 The WSClock Page Replacement Algorithm

Operating System Concepts 9ed. Ch9 Virtual Memory

* Ch9.4 Page Replacement
  * Ch9.4.1 Basic Page Replacement
  * Ch9.4.2 FIFO Page Replacement
  * Ch9.4.3 Optmal Page Replacement
  * Ch9.4.4 LRU Page Replacement
  * Ch9.4.5 LRU-Approximation Page Replacement
    * Ch9.4.5.1 Additional-Reference-Bits Algorithm
    * Ch9.4.5.2 Second-Chance Algorithm
    * Ch9.4.5.3 Enhanced Second-Chance Algorithm
  * Ch9.4.6 Counting-Based Page Replacement
  * Ch9.4.7 Page-Buffering Algorithms
* Ch9.6 Thrashing
  * Ch9.6.1 Cause of Thrashing
  * Ch9.6.2 Working-Set Model
  * Ch9.6.3 Page-Fault Frequency

### Wikipedia

* [Page replacement algorithm](https://en.wikipedia.org/wiki/Page_replacement_algorithm)
* [Thrashing (computer science)](https://en.wikipedia.org/wiki/Thrashing_(computer_science))
* [Working set](https://en.wikipedia.org/wiki/Working_set)
  * [Working set size](https://en.wikipedia.org/wiki/Working_set_size)
  * [Resident set size](https://en.wikipedia.org/wiki/Resident_set_size)

### Video

* [**Youtube - M4U10L10 The Working Set Model**](https://youtu.be/GQ7BjDIuRcg)
