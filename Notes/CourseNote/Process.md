# Process

[Process Practice](../../Practice/Process)

## Basic Concepts

Definition of Process

* Is a *single running activity* of a program with independent function working on some data set
* Is a independent unit for system to **manage resources** and **scheduling** (maybe "Thread")
* Is a *abstraction* of CPU
* Every process has independent *address space*

> Alias: Task, Job

* [Wiki - Process (computing)](https://en.wikipedia.org/wiki/Process_(computing))

## Process Status

### 3 Basic Status

* **Running**
* **Ready**
* **Waiting/Blocked**

### 2 More Status

* **New/Created**
* **Terminated**

#### 5 Status Model

### 1 More Status

* **Suspend**

#### 7 Status Model

![7 Status](https://upload.wikimedia.org/wikipedia/commons/8/83/Process_states.svg)

### Linux Process Status

## Process Control Block (PCB)

* It's the only sign that system know a process existing

> Alias: Task Controlling Block, Entry of the Process Table, Task Struct, Switchframe

* PCB structure (wiki)
  * Process identification data
  * Process state data
  * Process control data
* PCB structure (Modern OS)
  * Process management
    * Process ID
    * Program counter (PC)
  * Memory management
  * File management
    * User ID
    * Group ID

PCB Table: A set for all the process's PCB

* [Wiki - Process control block](https://en.wikipedia.org/wiki/Process_control_block)

### Real-World Example

#### Linux TASK_STRUCT

#### Windows EPROCESS, KPROCESS, PEB

## Process Address Space

![Memory Layout](https://gabrieletolomei.files.wordpress.com/2013/10/memory_layout.jpg)

![virtual address space of a process](https://i.stack.imgur.com/CvITh.png)

* Kernel Space
* User Space
  * **Stack**
    * Stack Pointer (SP) pointing to its bottom
  * [Empty space]
  * **Heap**
    * [mmap](https://en.wikipedia.org/wiki/Mmap)
    * [malloc](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation)
  * **Data** (global data)
    * shared memory
    * shared libraries (e.g. DLLs)
  * **Code**

> For 32-bit OS, the process address space is 2³² = 4GB

![Sample 4GB process address space](https://documentation.progress.com/output/ua/OpenEdge_latest/dmadm/images/rfi1401805036469.image)

![In-Memory Layout of a Program (Process)](https://gabrieletolomei.files.wordpress.com/2013/10/program_in_memory2.png)

> For 64-bit OS, in the theory the process address space should be 2⁶⁴ but it's way too big.
> So in practice the system will give it 2⁴⁸ or 2³⁹. (The decision of number is by [**multi-level paging**](MemoryManagement.md#Multi-level-Paging))

### Virtual Address Space (VAS)

![Wiki Virtual address space](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Virtual_address_space_and_physical_address_space_relationship.svg/587px-Virtual_address_space_and_physical_address_space_relationship.svg.png)

* [Wiki - Virtual address space](https://en.wikipedia.org/wiki/Virtual_address_space)

## Context Switch

Cost for Context Switching: saving and loading registers and memory maps, updating various tables and lists, etc

Example of Linux Kernel: context switching involves switching registers, stack pointer, and program counter, but is independent of address space switching, though in a process switch an address space switch also happens.

* [Wiki - Context switch](https://en.wikipedia.org/wiki/Context_switch)

[CPU Scheduling](CPUScheduling.md#Process-Context-Switch)

## Process Queue

## Process Control and System Call

System Call

> Are primitive, atomic operations

* Create Process
  * Example
    * UNIX: fork/exec
    * Windows: CreateProcess
* Terminate Process
  * Example
    * UNIX: exit
    * Windows: ExitProcess
* Block Process
  * Example
    * UNIX: wait
    * Windows: WaitForSingleObject

### Example of UNIX Process System Calls

* fork()
  * Copy one page a time from parent process to child process's address space
    * Copy-on-write (COW) fork in Linux
  * Return 0 to child and return child process's pid to parent
* exec()
  * Use new code to override the original address space
* wait()
* exit()

* [Wiki - System call](https://en.wikipedia.org/wiki/System_call)

## Others

Process classification

* System Process
* User Process

- Frontground Process
- Background Process

+ CPU-bound Process
  + [Wiki - CPU-bound](https://en.wikipedia.org/wiki/CPU-bound)
+ I/O-bound Process
  + [Wiki - I/O bound](https://en.wikipedia.org/wiki/I/O_bound)

Process hierarchy structure

* UNIX Process family tree
  * `init` as root
* Windows
  * same position

## Resource

* [Wiki - Process management (computing)](https://en.wikipedia.org/wiki/Process_management_(computing))
* [Shichao's Notes Chapter 3. Process Management](https://notes.shichao.io/lkd/ch3/)
* [**In-Memory Layout of a Program (Process)**](https://gabrieletolomei.wordpress.com/miscellanea/operating-systems/in-memory-layout/)
