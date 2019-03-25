# Memory

## Terminology

> Translate English to two different wording of Traditional Chinese and Simplified Chinese...

* Memory 記憶體(內存)
* Cache 快取(高速緩存)
* Disk 硬碟(磁盤)

Memory Management Hardware

* [PTBR (PDBR)](#PTBR-(PDBR)): Page-table (Page-directory) Base Register
* [MMU](#MMU): Memory Management Unit 記憶體(內存)管理單元
  * [TLB](#TLB): Translate Look-aside Buffer 快表

[Paging](#Paging) 頁式儲存管理

* Page 頁
  * Page Number 頁號
  * Page Offset 頁內地址 (頁內偏移)
* [(Page) Frame](#(Page)-Frame) 頁框
  * PFN: Page Frame Number
* [Page Table](#Page-Table) 頁表
  * [PTE](#Page-Table-Entry): Page Table Entry 頁表項
  * PDE: Page Directory Entry 頁目錄項
* Page Directory 頁目錄
* [Inverted Page Table](#Inverted-Page-Table) 反轉(反置,反向)頁表

> *Page Directory* is a *Page Table* for the next level Page Table in [Multi-level Paging](#Multi-level-Paging) (Hierarchical Paging)

* [Address Binding](#Address-Binding) ([Memory Mapping](#Memory-Mapping)) 地址重定位
* Page Fault 頁面錯誤
  * 缺頁異常
* [RSS](#RSS) Resident Set Size 駐留集大小

Abbreviations about paging

* Basic Parameters
  * N = $2^n$: Number of addresses in Virtual Address Space
  * M = $2^m$: Number of addresses in Physical Address Space
  * P = $2^p$: Page size
* components of the [Virtual Address (VA)](#Logical-Address-(Relative-Address,-Virtual-Address)) (邏輯地址, 相對地址, 虛擬地址)
  * TLBI: TLB Index
  * TLBT: TLB Tag
  * VPO Virtual Page Offset
  * VPN Virtual Page Number
* components of the [Physical Address (PA)](#Physical-Address-(Real-Address)) (物理地址, 絕對地址, 實地址)
  * PPO: Physical Page Offset (same as VPO)
    * because the block size of VA space and PA space are the same
  * PPN: Physical Page Number
  * CO: Byte offset within cache line
  * CI: Cache index
  * CT: Cache tag

> No **segment**, **allocation of frames** and **thrashing** in current notes

## Overview of the Memory

* Register
* L1 Cache (SRAM)
* L2 Cache (SRAM)
* L3 Cache (SRAM)
* Memory   (RAM)
* Disk (Secondary Memory)

1. We need to address the memory address from VA to PA.
2. We need address protection
   * authority
   * interprocess

Virtual address space is usually greater than Physical address space.
And we need to map our page to physical memory.

Recap the Process address space (This is *virtual memory space*)

* Kernel address space
* User address space
  * Heap
  * Stack
  * Data
  * Code

![Figure 9.2 - Virtual address space](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_02_VirtualAddressSpace.jpg)

Note that the process address space is stored in the PCB as a *balanced binary tree*. And the unused data (page) won't be put in the tree.

### When to link the address from instruction, data to VA

![Figure 8.3 shows the various stages of the binding processes and the units involved in each stage](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_03_MultistepProcessing.jpg)

* Compile
  * multiple source code -> object code
* Link
  * multiple object code -> single module (in the disk)
  * other object code    /
* Load
  * module --> module (in the memory)
  * library /
* Execute

> Later will be better, but usually in "Link" stage

### Address Binding

> Redirecting the Address

```txt
Logical Address ------ Binding (Redirecting) --------> Physical Address
```

In order to let CPU executing instruction can access right memory unit.

#### Logical Address (Relative Address, Virtual Address)

The address start from 0.
And the rest of the instruction address are record with the *offset*. (relative to the starting address)

> In some system, logical address not direct equal to virtual address (linear address)
>
> ```txt
> Logical Address ---(segment maybe)--> Linear Address ---(paging)---> Physical Address
> ```

#### Physical Address (Real Address)

The address *in the memory*

#### Static Relocation

> Static Address Redirection

Translate the address at once when loading the program into memory.

> We can do this by software only

#### Dynamic Relocation

> Dynamic Address Redirection

Translate the address while executing the program.

i.e. map the address instruction by instruction.

> We need the support of hardware (i.e. [MMU](#MMU))

![Figure 8.4 - Dynamic relocation using a relocation register](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_04_DynamicRelocation.jpg)

### Address Protection

* Base address register 基地址
* Limit address register 界線地址

> These registers are loaded by the OS by using special privilege instruction

![Figure 8.1 - A base and a limit register define a logical addresss space](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_01_LogicalAddressSpace.jpg)

![Figure 8.2 - Hardware address protection with base and limit registers](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_02_HardwareAddressProtection.jpg)

![Figure 8.6 - Hardware support for relocation and limit registers](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_06_HardwareSupport.jpg)

If the address over the bound => *Trap* the operating system!

### Goals of memory management

* Allocate memory to process => the address space
* Load content into memory => Address binding
* Storage protection => Address protection
* Manage shared memory
* Minimize the access time

## Memory Management

### Physical Memory Management

#### Data Structure

* Split in same size
* Split in different size

#### Algorithm

* First fit
  * Search the list of holes until one is found that is big enough to satisfy the request, and assign a portion of that hole to that process.
* Next fit
* Best fit
  * Allocate the smallest hole that is big enough to satisfy the request.
* Worst fit
  * Allocate the largest hole available, thereby increasing the likelihood that the remaining portion will be usable for satisfying future requests.

> First and best fits are about equal in terms of storage utilization, but first fit is faster.

#### Fragmentation

All the memory allocation strategies suffer from **external fragmentation**

> **External fragmentation** means that the available memory is broken up into lots of little pieces, none of which is big enough to satisfy the next memory requirement, although the sum total could.

* The external fragmentation problem can be reduced via **compaction**, i.e. moving all processes down to one end of physical memory. (This only involves updating the relocation register for each process, as all internal work is done using logical addresses.)

**Internal fragmentation** also occurs, with all memory allocation strategies.

> **Internal fragmentation** is caused by the fact that memory is allocated in blocks of a fixed size, whereas the actual memory needed will rarely be that exact size.

#### Recycling Problem

#### Buddy System (夥伴系統)

* [Wiki - Buddy memory allocation](https://en.wikipedia.org/wiki/Buddy_memory_allocation)

### Fixed and Dynamic Partition (Blocking)

#### Fixed Partition

allocating memory in equal sized blocks

#### Dynamic Partition

#### Problem

* [Fragmentation](#Fragmentation)
  * solution => Memory Compaction 緊縮技術

### Paging

Paging is a memory management scheme that allows processes physical memory to be discontinuous,

* Divide physical memory into equal sized blocks => **frame**
* Divide logical memory into blocks of the same size => **page**

#### Page Table

[Wiki - Page table](https://en.wikipedia.org/wiki/Page_table)

![Figure 8.11 - Paging model of logical and physical memory](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_11_PagingModel.jpg)

* Page Table
  * support by hardware
  * for each process has its own page table
  * Position
    * in Memory => PCB
    * on CPU => CR3 Register (Storing the starting position of the Page Table (Page Directory))
* Page Table Entry
  * Logical page number <--> Frame number
  * Entries in either level of page tables have the same format. (but not among all the system)

> CPU get *logical address* --> split into *page number* and *page offset*

![PageNumberOffset](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_11A_PageNumberOffset.jpg)

```txt
Page Number --(search the Page Table)--> Frame Number + Offset --> Physical Address!
```

![Figure 8.10 - Paging hardware](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_10_PagingHardware.jpg)

Note that page table is also stored in memory.

#### (Page) Frame

A page frame is a 4K-byte unit of contiguous addresses of physical memory. Pages begin onbyte boundaries and are fixed in size.

> i.e. with 12 bits offset! (4K = $2^12$)

#### Multi-level Paging

Hierarchical Paging

> It's like the tree data structure

**Example of a 32-bit system (IA-32)**:

```txt
Format of a linear address

 31                 22 21                 12 11                 0
+---------------------+---------------------+--------------------+
|         DIR         |        PAGE         |       OFFSET       |
+---------------------+---------------------+--------------------+
```

![IA-32 linear address](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_22A_TwoLevelPageNumberOffset.jpg)

We have 32 bits of a virtual address length.

And with the 12 bits offset for each [page frame](#(Page)-Frame)

> In a page directory, the page frame address is the address of a page table.
>
> In a second-level page table, the page frame address is the address of the page frame that contains the desired memory operand.

We have 1024 pages so we need 10 bits for the second-level page offset.

And we have 1024 page table so we need another 10 bits for the page directory.

Thats how our 32-bit address form.

**Example of a 64-bit system**:

* 8 bytes to store page entries
* 4KB for a single page frame

So there are $2^9$ entries in a single page.

That is need 9 bits to locate.

4 level paging scheme => 48 bits

```txt
Format of a linear address

 48          40 39          31 30          22 21           12 11           0
+--------------+--------------+--------------+--------------+--------------+
|   1st PAGE   |   2nd PAGE   |   3rd PAGE   |   4th PAGE   |    OFFSET    |
+--------------+--------------+--------------+--------------+--------------+
```

![Figure 8.25 - x86-64 linear address.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_25_x86-64LinearAddress.jpg)

3 level paging scheme => 39 bits

```txt
Format of a linear address

 39          31 30          22 21          12 11           0
+--------------+--------------+--------------+--------------+
|   1st PAGE   |   2nd PAGE   |   3rd PAGE   |    OFFSET    |
+--------------+--------------+--------------+--------------+
```

#### PTBR (PDBR)

> The page directory base register in i386 is called the CR3 register

```txt
Page Translation

                                                              PAGE FRAME
              +-----------+-----------+----------+         +---------------+
              |    DIR    |   PAGE    |  OFFSET  |         |               |
              +-----+-----+-----+-----+-----+----+         |               |
                    |           |           |              |               |
      +-------------+           |           +------------->|    PHYSICAL   |
      |                         |                          |    ADDRESS    |
      |   PAGE DIRECTORY        |      PAGE TABLE          |               |
      |  +---------------+      |   +---------------+      |               |
      |  |               |      |   |               |      +---------------+
      |  |               |      |   |---------------|              ^
      |  |               |      +-->| PG TBL ENTRY  |--------------+
      |  |---------------|          |---------------|
      +->|   DIR ENTRY   |--+       |               |
         |---------------|  |       |               |
         |               |  |       |               |
         +---------------+  |       +---------------+
                 ^          |               ^
+-------+        |          +---------------+
|  CR3  |--------+
+-------+
```

![Figure 8.23 - Paging in the IA-32 architecture.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_23_PentiumPaging.jpg)

![Figure 8.24 - Page address extensions.
](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_24_PageAddressExtensions.jpg)

The physical address of the current page directory is stored in the CPU register CR3

![Wiki x86 paging](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/X86_Paging_4K.svg/576px-X86_Paging_4K.svg.png)

#### Page Table Entry

```txt
Format of a Page Table Entry

 31                                  12 11                      0
+--------------------------------------+-------+---+-+-+---+-+-+-+
|                                      |       |   | | |   |U|R| |
|      PAGE FRAME ADDRESS 31..12       | AVAIL |0 0|D|A|0 0|/|/|P|
|                                      |       |   | | |   |S|W| |
+--------------------------------------+-------+---+-+-+---+-+-+-+

          P      - PRESENT
          R/W    - READ/WRITE
          U/S    - USER/SUPERVISOR
          D      - DIRTY
          AVAIL  - AVAILABLE FOR SYSTEMS PROGRAMMER USE

          NOTE: 0 INDICATES INTEL RESERVED. DO NOT DEFINE.
```

```txt
Invalid Page Table Entry

 31                                                           1 0
+--------------------------------------------------------------+-+
|                                                              | |
|                            AVAILABLE                         |0|
|                                                              | |
+--------------------------------------------------------------+-+
```

Common PTE (differ from system)

* P Present 有效位
  * Address end with
    * even number => invalid
    * odd number => valid
* R/W Read/Write 只讀/可讀寫
* U/S User/Supervisor 用戶/內核
* D Dirty 修改位
* PCD (CD) Page Cache Disable 禁止緩存
* PWT Page Write Through 緩存寫策略

## Memory "Expension" - by Software Technique

* Memory Compaction 內存緊湊
  * e.g. adjusting blocks
* Overlaying 覆蓋技術
* Swapping 交換技術
* Virtual Memory

[Wiki - Physical Address Extension](https://en.wikipedia.org/wiki/Physical_Address_Extension)

### Overlaying

* Hard to programming (handling by programmer)
* More execution time

> used in the earlier OS

### Swapping

**Backing store** some processses who are not currently using the CPU (blocking). (or when out of the memory.)

> used in the small time-sharing system at first

![Figure 8.5 - Swapping of two processes using a disk as a backing store](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_05_ProcessSwapping.jpg)

* Only swap the things producing during runtime
  * stack & heap (no need for data and code section)
* Where does the swapping things goes => the **SWAP** space

### Virtual (Paging) Memory Management

> Use "CPU **Time** and Disk Space" exchange "Memory **Space**"

* Terms
  * Virtual Memory
    * combine the space of disk and memory
    * space size restricted by the system scheme (e.g. 32-bit, 64-bit)
  * Virtual Address
  * Virtual Address Space
    * allocate process virtual memory
  * Virtual Storage Mechanism

Idea of virtual paging:

* Only load part of the pages into memory when execute it
* If the page is not in memory (i.e. page fault) then load the page dynamically
* Swap the unused page into disk if necessary

Paging method

* Demand Paging 請求調頁
* Prepaging 預先調頁

#### Demand Paging

The basic idea behind demand paging is that when a process is swapped in, its pages are not swapped in all at once. Rather they are swapped in only when the process needs them. ( on demand. )

> Termed a **lazy swapper** or **pager**

#### Inverted Page Table

> "Inverted" the original idea of page table. We don't start from the aspect of VA but PA instead!

Instead of a table listing all of the pages for a particular process, an inverted page table lists all of the pages currently loaded in memory, for all processes.

That is, there is one entry per frame instead of one entry per page.

* [GeeksforGeeks - Operating System | Inverted Page Table](https://www.geeksforgeeks.org/operating-system-inverted-page-table/)

![GeeksforGeeks Inverted Page Table](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/33-6.png)

![Figure 8.20 - Inverted page table](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_20_InvertedPageTable.jpg)

Improvement => Using Hash Table

![Figure 8.19 - Hashed page table](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_19_HashedPageTable.jpg)

## MMU

```txt
CPU -- passing VA --> MMU -- translating to PA --> Memory or Disk
```

* [Wiki - Memory management unit](https://en.wikipedia.org/wiki/Memory_management_unit)

### Memory Mapping

![Figure 9.6 - Steps in handling a page fault](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_06_PageFaultSteps.jpg)

If **page fault**

1. Hardware generate exception
   * CR2 register store the return position (right on the line of the original position)
2. Trap the OS
3. Execute page fault handler

Else

1. VFN = PageTable[VPN] (Get page number from page table by VPN)
2. Physical Address = VFN + VPO

> Possible reason of page fault
>
> * Virtual page don't exist in memory
> * Page illegal
> * Page is been protected

### TLB

> Built with SRAM too. (same as Cache)

* [Wiki - Translation lookaside buffer](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)

Why TLB?

* Accessing page table too many times (especially in multi-level paging system)

When searching in TLB it is *parallelism* (support by hardware).

![Figure 8.14 - Paging hardware with TLB](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter8/8_14_PagingHardware.jpg)

Three possible result of looking TLB

1. Looking for TLB
   1. **TLB hit** (great, get the PA)
   2. TLB miss => Looking for Page Table
      1. **Page Table hit** (also great, get the PA)
      2. Page Table miss => **Page fault** (gotta load page from disk to memory)

#### TLB cooperate with Cache

> TLB is not equal to Cache. (different thing storing different stuff)

TLB vs. Cache|TLB|Cache
-------------|---|-----
Stored Content|Page table #|Instruction or Data
Access with|Virtual Address|Physical Address

#### RSS

> The pages in memory => *memory resident pages*

* [Resident set size](https://en.wikipedia.org/wiki/Resident_set_size)

> RSS: how many page frame we need to give to a process?

![Figure 9.11 - Graph of page faults versus number of frames.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter9/9_11_PageFaultGraph.jpg)

### Page Replacement

* Locked Page Frame
  * in PTE the CD bit (Cache Disable)
  * Main reason is the cost of page replacement will make the running time uncertain
    * e.g. OS core code, key data structure, I/O buffer...

#### Page Replacement Algorithm

* FIFO Page Replacement
* Optimal Page Replacement
* LRU Page Replacement
* LRU-Approximation Page Replacement
  * Additional-Reference-Bits Algorithm
  * Second-Chance Algorithm
  * Enhanced Second-Chance Algorithm
* Counting-Based Page Replacement
* Page-Buffering Algorithms

## A Memory System Example

> The CMU 2015 Fall Lecture 18

The Scenario

* Addressing
  * 14-bit VA
    * VPN: 6~13 bit
    * *VPO: 0~5 bit*
  * 12-bit PA
    * PPN: 6~11 bit
    * *PPO: 0~5 bit*
  * Page size = 64 bytes = $2^6$
    * *So offset need 6 bit*
* TLB
  * 16 entries
  * *4-way* associative
    * TLBI: 0~1 bit of VPN (*2-bit*)
    * TLBT: the rest of VPN
* Page Table
    VPN|PPN|Valid
    ---|---|-----
    test|test|test
* Cache
  * **16 lines**, *4-byte block size*
    * CO: 0~1 bit (*2 offset bits*)
    * CI: 2~5 bit (**4 index bits**)
    * CT: the remaining bits (not necessary to equal to the size of PPN)
  * Physically addressed
  * Direct mapped

The example #1

VA: 0x03D4

## Resources

* [5.2 Page Translation](http://www.logix.cz/michal/doc/i386/chp05-02.htm)

### Book

Modern Operating System 4ed.

* Ch3 Memory Management
  * Ch3.2 A Memory Abstraction: Address Space
    * Ch3.2.2 Swapping
  * Ch3.3 Virtual Memory
    * Ch3.3.1 Paging
    * Ch3.3.2 Page Tables
  * Ch3.4 Page Replacement Algorithms
  * Ch3.6 Implementation Issues
    * Ch3.6.2 Page Fault Handling
    * Ch3.6.4 Locking Pages in Memory

Operating System Concepts 9ed. Part 3 Memory Management

* Ch8 Main Memory
  * Ch8.1 Background
    * Ch8.1.1 Basic Hardware - **Hardware address protection**
    * **Ch8.1.2 Address Binding**
    * **Ch8.1.3 Logical Versus Physical Address Space**
    * Ch8.1.4 Dynamic Loading
    * Ch8.1.5 Dynamic Linking and Shared Librarie
  * Ch8.2 Swapping
  * Ch8.3 Contiguous Memory Allocation
    * **Ch8.3.1 Memory Protection**
    * Ch8.3.2 Memory Allocation
    * **Ch8.3.3. Fragmentation**
  * **Ch8.5 Paging**
    * Ch8.5.1 Basic Method
    * Ch8.5.2 Hardware Support - TLB, PTBR
    * Ch8.5.3 Protection
  * Ch8.6 Structure of the Page Table
    * **Ch8.6.1 Hierarchical Paging**
    * Ch8.6.2 Hashed Page Tables
    * **Ch8.6.3 Inverted Page Tables**
  * Ch8.7 Example: Intel 32 and 64-bit Architectures ( Optional )
    * Ch8.7.1.2 IA-32 Paging
    * Ch8.7.2 x86-64
* Ch9 Virtual Memory
  * Ch9.2 Demand Paging
  * **Ch9.4 Page Replacement** - The algorithms...
* Notes
  * [Main Memory](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/8_MainMemory.html)
  * [Virtual Memory](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/9_VirtualMemory.html)

### MOOC

* CMU - [2015 Fall: 15-213 Introduction to Computer Systems](https://scs.hosted.panopto.com/Panopto/Pages/Sessions/List.aspx#folderID=%22b96d90ae-9871-4fae-91e2-b1627b43e25e%22&view=0)
  * [Lecture 17: Virtual Memory: Concepts](https://scs.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=80cd47f9-2eba-4e7b-bbab-a2867f87b7fa)
  * [**Lecture 18: Virtual Memory: Systems**](https://scs.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=d3ac31f4-331d-469c-91bf-6ac6692f0159)
    * [**slides**](https://www.cs.cmu.edu/~213/lectures/18-vm-systems.pdf) - Example of Intel Core i7
