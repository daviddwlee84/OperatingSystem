# XV6 Source Code Reading -- Process and Thread

## Requirement

### Code to read

* Basic headers
  * [X] `types.h` (p.1)
  * [X] `param.h` (p.1)
  * [X] `memlayout.h` (p.2)
  * [X] `defs.h` (p.2)
  * [X] `x86.h` (p.4)
  * [X] `asm.h` (p.6)
  * [X] `mmu.h` (p.7)
  * [X] `elf.h` (p.9)
* Processes
  * [ ] `vm.c` (p.17)
  * [ ] `proc.h` (p.23)
  * [ ] `proc.c` (p.24)
  * [ ] `swtch.S` (p.30)
  * [ ] `kalloc.c` (p.31)

#### types.h

* some typedef of unsigned variable
  * unsigned int -> uint -> pde_t
  * unsigned short -> ushort
  * unsigned char -> uchar

#### param.h

* some global define

#### memlayout.h - Memory Layout

* seems like memory address things

#### defs.h

* definitions of many file
  * bio.c
  * console.c
  * exec.c
  * file.c
  * fs.c
  * ide.c
  * ioapic.c
  * kalloc.c
  * kbd.c
  * lapic.c
  * log.c
  * mp.c
  * picirq.c
  * pipe.c
  * proc.c
  * swtch.S
  * spinlock.c
  * sleeplock.c
  * string.c
  * syscall.c
  * timer.c
  * trap.c
  * uart.c
  * vm.c
* and some struct definition

#### x86.h

> Routines to let C code use special x86 instructions.

* Layout of the trap frame

#### asm.h

> Assembler macros to create x86 segments

#### mmu.h - Memory Management Unit

> This file contains definitions for the x86 memory management unit

* lot of define of memory address
  * Eflags register (FL)
  * Control Register flags (CR)
  * Various segment selectors (SEG)
* In `__ASSEMBLER__`
  * Segment Descriptor
  * Normal segment (SEG)
  * Application segment type bits (STA)
  * System segment type bits (STS)
  * page directory index (PDX)
  * page table index (PTX)
  * construct virtual address from indexes and offset (PGADDR)
  * Page directory and page table constants
  * Page table/directory entry flags
  * Address in page table or page directory entry
* Task state segment format

#### elf.h

> Format of an ELF executable file
