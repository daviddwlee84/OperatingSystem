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

### Program Counter Related Registers

The Mips processor is a "two-stage" pipeline processor; that is, it has 2 program counter registers - PCReg and NextPCReg.

* PCReg: for the next instruction to execute
* NextPCReg: for the instruction after that

While the processor is executing one instruction, it can simultaneously be preparing the next one to execute.

#### Exception Related Problem

How should these registers be adjusted before returning from a system call that simply continues execution of the user thread that made the system call?

Example: page fault exception

the Mips instruction that caused the page fault will need to be executed again

Example: system call

we should not execute the Mips instruction that caused the exception, since it was the syscall instruction, as this would cause an infinite loop of calls

> So the question is whether the PC registers have already been incremented when the syscall is made or not. The answer is that they have **not** been incremented. So before returning from the system call, the PC registers for the calling thread must be incremented properly!

* [System Calls Need to Adjust the PC Registers](http://condor.depaul.edu/glancast/546class/docs/adjustRegs.html)

#### Nachos User-Space Register

There is the definition of user register and related operations in `code/threads/thread.h`.

It says there are two sets of CPU registers, each to record state in different mode.

* user code running state
* kernel code running state

```c
class Thread {

    ...

  private:

    ...

#ifdef USER_PROGRAM
// A thread running a user program actually has *two* sets of CPU registers -- 
// one for its state while executing user code, one for its state 
// while executing kernel code.

    int userRegisters[NumTotalRegs];	// user-level CPU register state

  public:
    void SaveUserState();		// save user-level register state
    void RestoreUserState();		// restore user-level register state

    AddrSpace *space;			// User code this thread is running.
#endif

};
```

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
* Nachos Study Book Ch6.4 MIPS Simulator
  * Ch6.4.1 Components of MIPS Machine
  * Ch6.4.2 Instruction Interpretation
  * Ch6.4.3 MIPS Instruction Set

### Wikipedia

* [MIPS architecture](https://en.wikipedia.org/wiki/MIPS_architecture)
* [Processor register](https://en.wikipedia.org/wiki/Processor_register)
  * [Program status word](https://en.wikipedia.org/wiki/Program_status_word)
    * [Program counter](https://en.wikipedia.org/wiki/Program_counter)
    * [Status register](https://en.wikipedia.org/wiki/Status_register)
* [Floating-point arithmetic](https://en.wikipedia.org/wiki/Floating-point_arithmetic)

### Tutorial

* [Learn MIPS Assembly in Y minutes](https://learnxinyminutes.com/docs/mips/)
