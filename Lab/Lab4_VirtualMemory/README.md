# Lab 4: Virtual Memory

> **Hints for this Lab**
>
> 1. Character sequencial order
>    * VMware virtual machine and normal PC is using [Little-Endian](https://en.wikipedia.org/wiki/Endianness#Little-endian)
>    * In Nachos simulator processor is using [Big-Endian](https://en.wikipedia.org/wiki/Endianness#Big-endian)
>    * Be careful when using `WordToMachine()` and `ShortToMachine()` for proper transition
> 2. How program is put in address space
>    * Nachos described this in `bin/noff.h`
>      * `Struct segment` represents a segmet of a program
>      * `Struct noffHeader` define the *code section*, *initialized data section* and *uninitialized data section*
> 3. Related source code
>    * `code/machine/machine.h`
>    * `code/machine/machine.cc`
>    * `code/machine/translate.h`
>    * `code/machine/translate.cc`
>    * `code/userprog/addrspace.h`
>    * `code/userprog/addrspace.cc`
>    * `code/userprog/exception.h`
>    * `code/userprog/exception.cc`

## I. TLB exception handling

> Currently, Nachos memory management is using software simulate the TLB mechanism.
> Its working principle, exception handling, replacement algorithm is similar with paging memory management.

### Exercise 1: Trace code

> Read the `code/userprog/progtest.cc`, understand the procedure of Nachos executing user program and memory menagement related point
>
> Read the following code, understand current Nachos TLB technique and Address Binding (地址轉換) mechanism
>
> * `code/machine/machine.h`
> * `code/machine/machine.cc`
> * `code/userprog/exception.h`
> * `code/userprog/exception.cc`

### Exercise 2: TLB MISS exception handling

> Modify the `ExceptionHandler()` in `code/userprog/exception.cc`. Makes Nachos able to handle TLB exception
>
> (When TLB exception, Nachos will throw PageFaultException. Reference `code/machine/machine.cc`)

### Exercise 3: Replacement algorithm

> Implement at least two replacement algorithm, compare the replacement times between two algorithm.

## II. Paging Memory Management

> In the current Nachos, The member variable `AddrSpace* space` used in `Class Thread` use `TranslationEntry* pageTable` to manage memory.
> When application program is starting up it will initialize it; When context switch, it will also do reserve and resume.
> (Such that `Class Machine::TranslationEntry* pageTable` will always pointing at the current running Thread's page table)

### Exercise 4: Global data structure for memory management

> Impelement a global data structure for memory allocation and recycle, and record the current memory usage status
>
> e.g. Free Linked List (空閒鏈表)
>
> [![Linked List](https://www.cs.nuim.ie/~dkelly/CS240-05/Day%208%20Slides_files/image010.gif)](http://www.cs.nuim.ie/~dkelly/CS240-05/Day%208%20Slides.htm)
>
> e.g. Bitmap(位圖)
>
> [![Bitmap](https://www.cs.nuim.ie/~dkelly/CS240-05/Day%208%20Slides_files/image009.gif)](http://www.cs.nuim.ie/~dkelly/CS240-05/Day%208%20Slides.htm)

### Exercise 5: Support multi-threads

> In the current Nachos, only single Thread can exist in memory. We need to break this restriction

### Exercise 6: Missing page interrupt handling

> Based on TLB mechanism exception handling and the replacement algorithm. Implement missing page interrupt handler and page replacement algorithm.
>
> (The TLB exception mechanism is loading the page in memory from memory to TLB. Thus, missing page handler is to load new page from disk to memory)

## III. Lazy-loading (i.e. Demand Paging)

### Exercise 7: Loading page on demand

> Nachos allocate memory must be completed once the user program is loaded into memory. Thus the user program size is strictly restrict to be lower than 4KB.
> Implement a lazy-loading algorithm that load the page from disk to memory if and only if the missing page exception occur.

## Challenges

### Challenge 1

> Add `SUSPENDED` state for Thread. And implement the switching between `SUSPENDED`, `READY` and `BLOCKED`

### Challenge 2

> The defect of Hierarchical Paging (multi-level page table) is the page table size is *in direct ratio to* virtual address space.
> To reduce the consumption on physical page table. Implement inverted page table on Nachos.

## Resources

### Example

* [**Baidu文庫 - nachos Lab4實習報告**](https://wenku.baidu.com/view/be56dfe2541810a6f524ccbff121dd36a32dc430.html)
* [Baidu文庫 - 北大操作系統高級課程-陳向群作業-虛擬內存管理實習報告](https://wenku.baidu.com/view/420a3f6b04a1b0717ed5dd79.html)
* [Sina博客 - Nachos3.4 Lab3 虛擬內存管理 實習報告 TLB異常處理](http://blog.sina.com.cn/s/blog_4ae8f77f01018n63.html)
* [Sina博客 - Nachos3.4 Lab3 虛擬內存管理 實習報告 分頁式內存管理](http://blog.sina.com.cn/s/blog_4ae8f77f01018n6r.html)
