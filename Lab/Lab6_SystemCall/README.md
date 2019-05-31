# Lab 6: System Call

## I. Understanding Nachos System Call

In the [MIPS Instruction Reference](http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html), it describe the MIPS system call instruction.

SYSCALL -- *System call*

* Description: Generates a software interrupt.
* Operation: advance_pc (4);
* Syntax: syscall
* Encoding: `0000 00-- ---- ---- ---- ---- --00 1100`

### Exercise 1: Trace code

> Read the system call related code, understand current Nachos system call mechanism
>
> * `code/userprog/syscall.h`
> * `code/userprog/exception.cc`
> * `code/test/start.s`

We've met the "exception handler" in the [virtual memory Lab](../Lab4_VirtualMemory/README.md#Exercise-2:-TLB-MISS-exception-handling).

The system calls are considered as a kind of exception in Nachos.

When `Machine::OneInstruction` (in `code/machine/mipssim.cc`) decodes a `OP_SYSCALL` (define in `code/machine/mipssim.h` as `#define OP_SYSCALL 61`)

```c
void
Machine::OneInstruction(Instruction *instr)
{
    switch (instr->opCode){

        ...

        case OP_SYSCALL:
            RaiseException(SyscallException, 0);
            return;

        ...
    }

    ...
```

It will call `Machine::RaiseException` and then finally into the `ExceptionHandler` that we've seen before.

There is a important thing to mention that when handling the system call, we need to make sure we've advanced the program counter. This is also mentioned in `Machine::OneInstruction`'s comment.

```c
// On a syscall,
// the OS software must increment the PC so execution begins
// at the instruction immediately after the syscall.
```

> I've also taken some notes [**here**](../../Notes/Nachos/MIPS.md#Exception-Related-Problem)

#### `syscall.h` and `IN_ASM`

There is a macro `IN_ASM`.

In `code/userprog/syscall.h`, there is a condition `#ifndef IN_ASM` followed by the declaration of all the ten syscall supported by Nachos. And the definition of these syscall are in `code/machine/sysdep.cc`. Most of them were been packed in the UNIX standard library.

But in `code/test/start.s`, it will define `IN_ASM` first and then include the `code/userprog/syscall.h`. This time it will define all the system call in assembly way. Including setting the syscall type in the `$2` register.

For example of `Halt()` and also the comments:

```c
/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is 
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */

	.globl Halt
	.ent	Halt
Halt:
	addiu $2,$0,SC_Halt
	syscall
	j	$31
	.end Halt
```

Each of them corresponding to a system call code define in `code/userprog/syscall.h`

```c
/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SC_Halt		0
#define SC_Exit		1
#define SC_Exec		2
#define SC_Join		3
#define SC_Create	4
#define SC_Open		5
#define SC_Read		6
#define SC_Write	7
#define SC_Close	8
#define SC_Fork		9
#define SC_Yield	10
```

That's why in the exception handler, when we want to know which type of the syscall trigger the exception, we will check the r2 register.

Such like this in `code/userprog/exception.cc`:

```c
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    // System Call
    // The system call codes (SC_[TYPE]) is defined in userprog/syscall.h
    // The system call stubs is defined in test/start.s
    int type = machine->ReadRegister(2); // r2: the standard C calling convention on the MIPS

    if (which == SyscallException) {
        switch(type) {
            case: ...

        }
    }
}
```

> Note that I don't use switch case in practice in the `ExceptionHandler()`.

## II. File System Related System Call

### Exercise 2: Implement file system system call

> On the analogy of the Halt implementation, complete the following system calls
>
> * Create
> * Open
> * Close
> * Write
> * Read
>
> There are basic description of the system calls in `code/userprog/syscall.h`

#### Create

#### Open

#### Read

#### Write

#### Close

### Exercise 3: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 2 and test the correctness.

## III. Executing User Program Related system call

### Exercise 4: Implement user program system call

> Implement the following system calls
>
> * Exec
> * Fork
> * Yield
> * Join
> * Exit
>
> There are basic description of the system calls in `code/userprog/syscall.h`

#### Exit

I've implemented the initial version Exit syscall in [virtual memory Lab](../Lab4_VirtualMemory/README.md#The-Exit-Syscall).
But this time, it requires not only receiving the return value but also the thread control.

Here is the following procedure that a Exit syscall need to do

1. 
2. 
3. 
4. 

#### Exec

#### Join

#### Fork

#### Yield

### Exercise 5: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 4 and test the correctness.

#### Test Exec, Exit, Join

![Nachos Exec/Exit/Join Example](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/img005.gif)

## Resources

CSC546 - Operating Systems

* [Lecture 4](http://condor.depaul.edu/glancast/546class/docs/lec4.html)
  * **Exit**
    * [System Calls Need to Adjust the PC Registers](http://condor.depaul.edu/glancast/546class/docs/adjustRegs.html)
  * Adjust Registers
  * Unix File System
  * Nachos File System
  * fileSystem
  * Nachos Write system call
* [**Lecture 9**](http://condor.depaul.edu/glancast/546class/docs/lec9.html)
  * Join
  * **Thread Class Additions**
  * List Class Additions
  * rcsmerge
  * Join Details
  * Exec Changes
  * **Exit Changes**

[CPS 110 / EE 153 Operating Systems (Duke University)](https://users.cs.duke.edu/~chase/cps110/)

* [**Nachos Exec/Exit/Join Example**](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld005.htm) - [Home - Process, Unix and IPC](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld001.htm)
  * [pdf - The Classical OS Model in Unix](https://users.cs.duke.edu/~chase/cps210-archive/slides/ux6.pdf)

### Nachos Related

#### System Call Entry

* [PTT C_and_CPP - Re: [問題] start.s在Nachos下的作用](https://www.ptt.cc/bbs/C_and_CPP/M.1534860829.A.C66.html)

### Example

* [nachos-Lab6實習報告](https://wenku.baidu.com/view/bd03f40ee97101f69e3143323968011ca300f71e.html)

Others

* [System call definitions](https://users.cs.duke.edu/~carla/syscall_def.html)

Yet Another

* [srijanshetty/nachos-syscalls](https://github.com/srijanshetty/nachos-syscalls)
* [manikantareddyd/NACHOS-syscall-implementation](https://github.com/manikantareddyd/NACHOS-syscall-implementation/)

Just.. a little bit related

* [CSDN - 【nachos】山東大學操作系統課設實驗nachos系統（6）系統調用Exec()和Exit()](https://blog.csdn.net/mottled233/article/details/78633571)
* [OS::NachOS::HW1 - System call: Sleep](http://blog.terrynini.tw/tw/OS-NachOS-HW1/#System-call-Sleep)
