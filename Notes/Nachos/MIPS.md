# MIPS

> MIPS stands for Microprocessor without Interlocked Pipelined Stages

## Introduction

* ISA (Instruction Set Architecture): RISC (Reduced Instruction Set Computer)

## Registers

> Nachos implement all the thirty-two MIPS R2/3000 Registers.
> Additionally, with 8 more special purpose register for better simulation and debug usage.

* thirty-two 32-bit GPRs (General-Purpose Reigsters): store both data and addresses
* thirty-two 32-bit FPRs (Floating-Point Registers): store floating point numbers
* SPRs (Special-Purpose Registers): hold program state
  * program counter (PC), also called the instruction pointer
  * status register: hold truth values often used to determine whether some instruction should or should not be executed

> PC and status register might be combined in program status word (PSW) register. (IBM System/360 architecture)

### Table

| Register Number | Conventional Name | Usage                                                            |
| --------------- | ----------------- | ---------------------------------------------------------------- |
| $0              | $zero             | Hard-wired to 0                                                  |
| $1              | $at               | Reserved for pseudo-instructions                                 |
| 2−3             | v0, v1            | Return values from functions                                     |
| 4−7             | a0−a3             | Arguments to functions - not preserved by subprograms            |
| 8−15            | t0−t7             | Temporary data, not preserved by subprograms                     |
| 16−23           | s0−s7             | Saved registers, preserved by subprograms                        |
| 24−25           | t8−t9             | More temporary registers, not preserved by subprograms           |
| 26−27           | k0−k1             | Reserved for kernel. Do not use.                                 |
| $28             | $gp               | Global Area Pointer (base of global data segment)                |
| $29             | $sp               | Stack Pointer                                                    |
| $30             | $fp               | Frame Pointer                                                    |
| $31             | $ra               | Return Address                                                   |
| f0− f3          | -                 | Floating point return values                                     |
| f4−f10          | -                 | Temporary registers, not preserved by subprograms                |
| f12−f14         | -                 | First two arguments to subprograms, not preserved by subprograms |
| f16−f18         | -                 | More temporary registers, not preserved by subprograms           |
| f20−f31         | -                 | Saved registers, preserved by subprograms                        |

## MAL MIPS Assembly Language

## Simulator

### QtSPIM

* [SPIM: A MIPS32 Simulator](http://spimsimulator.sourceforge.net/)
  * [Download SPIM](https://sourceforge.net/projects/spimsimulator/files/)

## Resources

Computer Science 315 Lecture Notes

* [Chapter 5. The MIPS Architecture](http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05.html)
  * [5.3. The MIPS Register Files](http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05s03.html)
  * [5.4. MAL: MIPS Assembly Language](http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05s04.html)
  * [15.5. MIPS Exception Handling](http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch15s05.html)

[CS 470](http://www.cs.iit.edu/~virgil/cs470/)

* Lab1 Getting Started with SPIM
* Lab2 MIPS: The Virtual Machine
* Lab3 Control Stuctures in MIPS
* Lab4 Register Usage and Procedures
* Lab5 Memory Issues
* Lab6 Arithmetic in MIPS
* [Lab7 Exceptions in MIPS](http://www.cs.iit.edu/~virgil/cs470/Labs/Lab7.pdf)

### Book

* [MIPS Assembly Language Programming using QtSpim](http://www.egr.unlv.edu/~ed/MIPStextSMv11.pdf)

### Wikipedia

* [MIPS architecture](https://en.wikipedia.org/wiki/MIPS_architecture)
* [Processor register](https://en.wikipedia.org/wiki/Processor_register)
  * [Program status word](https://en.wikipedia.org/wiki/Program_status_word)
    * [Program counter](https://en.wikipedia.org/wiki/Program_counter)
    * [Status register](https://en.wikipedia.org/wiki/Status_register)
* [Floating-point arithmetic](https://en.wikipedia.org/wiki/Floating-point_arithmetic)

### Tutorial

* [Learn MIPS Assembly in Y minutes](https://learnxinyminutes.com/docs/mips/)
