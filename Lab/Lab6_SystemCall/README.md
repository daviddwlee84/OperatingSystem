# Lab 6: System Call

> In this Lab I'll use `./build_modified_nachos.sh` to build the docker container.
> And I'll add user program in `code/test/` and use `code/userprog/nachos` to test.
>
> As usual, use `-d S` to show the system call debug message. And `-x executable` to run user program. 

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

I've left the API when doing Lab 4 in `code/userprog/exception.cc`.

```cpp
void
ExceptionHandler(ExceptionType which)
{
    ...

    // System Call
    // The system call codes (SC_[TYPE]) is defined in userprog/syscall.h
    // The system call stubs is defined in test/start.s
    int type = machine->ReadRegister(2); // r2: the standard C calling convention on the MIPS

    if (which == SyscallException) {
        if (type == SC_Halt) {
            DEBUG('a', "Shutdown, initiated by user program.\n");
            PrintTLBStatus(); // TLB debug usage
            interrupt->Halt();
        } else if (type == SC_Exit || type == SC_Exec || type == SC_Join) {
            // Address Space Control (Process Management) System Calls
            AddressSpaceControlHandler(type);
        } else if (type == SC_Create || type == SC_Open || type == SC_Write || type == SC_Read || type == SC_Close) {
            // File System System Calls
            FileSystemHandler(type);
        } else if (type == SC_Fork || type == SC_Yield) {
            // User-level Threads System Calls
            UserLevelThreadsHandler(type);
        }

        // Increment the Program Counter before returning.
        IncrementPCRegs();
        return;
    }
}
```

For this part, I'll put the handler in `FileSystemHandler`.

And because we need to advance the program counter after handling the system call. I've made this function and called after handling any syscall.

```cpp
//----------------------------------------------------------------------
// IncrementPCRegs
// 	Because when Nachos cause the exception. The PC won't increment
//  (i.e. PC+4) in Machine::OneInstruction in machine/mipssim.cc.
//  Thus, when invoking a system call, we need to advance the program
//  counter. Or it will cause the infinity loop.
//----------------------------------------------------------------------

void IncrementPCRegs(void) {
    // Debug usage
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));

    // Advance program counter
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
}
```

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

In `code/userprog/syscall.h`, the definition is as following

```cpp
/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */
 
/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;

/* when an address space starts up, it has two open files, representing 
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput	0  
#define ConsoleOutput	1 
```

And the operations

```cpp
/* Create a Nachos file, with "name" */
void Create(char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file.
 */
OpenFileId Open(char *name);

/* Write "size" bytes from "buffer" to the open file. */
void Write(char *buffer, int size, OpenFileId id);

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int Read(char *buffer, int size, OpenFileId id);

/* Close the file, we're done reading and writing to it. */
void Close(OpenFileId id);
```

#### Create and Open

Because these two functions needs to get the file name by reading memory.

So I've made this read file name helper function.

```cpp
// Helper function to get file name using ReadMem for Create and Open syscall
char* getFileNameFromAddress(int address) {
    int position = 0;
    int data;
    char name[FileNameMaxLength + 1];
    do {
        // each time read one byte
        bool success = machine->ReadMem(address + position, 1, &data);
        ASSERT_MSG(success, "Fail to read memory in Create syscall");
        name[position++] = (char)data;

        ASSERT_MSG(position <= FileNameMaxLength, "Filename length too long")
    } while(data != '\0');
    name[position] = '\0';
    return name;
}
```

And here is the `Create` syscall (type `SC_Create`)

```cpp
int address = machine->ReadRegister(4); // memory starting position
DEBUG('S', COLORED(GREEN, "Received Create syscall (r4 = %d): "), address);
char* name = getFileNameFromAddress(address);

bool success = fileSystem->Create(name, 0); // initial file length set 0

DEBUG('S', success ? COLORED(GREEN, "File \"%s\" created.\n") : COLORED(FAIL, "File \"%s\" fail to create.\n"), name);
```

Here is the `Open` syscall (type `SC_Close`)

```cpp
int address = machine->ReadRegister(4); // memory starting position
DEBUG('S', COLORED(GREEN, "Received Open syscall (r4 = %d): "), address);
char* name = getFileNameFromAddress(address);

OpenFile *openFile = fileSystem->Open(name);

DEBUG('S', COLORED(GREEN, "File \"%s\" opened.\n"), name);
machine->WriteRegister(2, (OpenFileId)openFile); // return result
```

#### Close

Closing the file is simply de-allocate the open file pointer.

Here is the `Close` syscall (type `SC_Close`)

```cpp
OpenFileId id = machine->ReadRegister(4); // OpenFile object id
DEBUG('S', COLORED(GREEN, "Received Close syscall (r4 = %d): "), id);

OpenFile* openFile = (OpenFile*)id; // transfer id back to OpenFile
delete openFile; // release the file

DEBUG('S', COLORED(GREEN, "File has closed.\n"));
```

#### Read and Write

This part is more complicated, because we need to read and write the buffer one by one byte. And handle the "data type transform" problem

Here is the `Read` syscall (type `SC_Read`)

```cpp
int address = machine->ReadRegister(4); // memory starting position
int size = machine->ReadRegister(5); // read "size" bytes
OpenFileId id = machine->ReadRegister(6); // OpenFile object id
DEBUG('S', COLORED(GREEN, "Received Read syscall (r4 = %d, r5 = %d, r6 = %d): "), address, size, id);

OpenFile* openFile = (OpenFile*)id; // transfer id back to OpenFile
char* buffer = new char[size];
int numBytes = openFile->Read(buffer, size);
for (int i = 0; i < numBytes; i++) { // each time write one byte
    bool success = machine->WriteMem(address + i, 1, (int)buffer[i]);
    if (!success) {
        i--;
    }
}
DEBUG('S', COLORED(GREEN, "Read %d bytes into buffer.\n"), numBytes);
machine->WriteRegister(2, numBytes); // Return the number of bytes actually read
```

Finally, here is the `Write` syscall (type `SC_Write`)

```cpp
int address = machine->ReadRegister(4); // memory starting position
int size = machine->ReadRegister(5); // read "size" bytes
OpenFileId id = machine->ReadRegister(6); // OpenFile object id
DEBUG('S', COLORED(GREEN, "Received Write syscall (r4 = %d, r5 = %d, r6 = %d): "), address, size, id);

char* buffer = new char[size];
for (int i = 0; i < size; i++) { // each time write one byte
    bool success = machine->ReadMem(address + i, 1, (int*)&buffer[i]);
    if (!success) {
        i--;
    }
}
OpenFile* openFile = (OpenFile*)id; // transfer id back to OpenFile
int numBytes = openFile->Write(buffer, size);

DEBUG('S', COLORED(GREEN, "Write %d bytes into file.\n"), numBytes);
machine->WriteRegister(2, numBytes); // Return the number of bytes actually write
```

### Exercise 3: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 2 and test the correctness.

I've made the user program `code/test/filesyscall.c` which will create a file call "test.txt" then write string and then read from it. Finally use the exit state to show how many bytes it has read.

```c
/* filesyscall.c
 *	Simple program to test file system syscall (Lab 6)
 */

#include "syscall.h"

#define BUFFER_SIZE 11

int main() {
    char data[9]; // as file name and content
    char buffer[9];
    OpenFileId fid_write;
    OpenFileId fid_read;
    int numBytes;

    data[0] = 't';
    data[1] = 'e';
    data[2] = 's';
    data[3] = 't';
    data[4] = '.';
    data[5] = 't';
    data[6] = 'x';
    data[7] = 't';
    data[8] = '\0';

    Create(data);

    fid_write = Open(data);
    fid_read = Open(data);

    Write(data, 8, fid_write);

    numBytes = Read(buffer, 8, fid_read);

    Close(fid_write);
    Close(fid_read);

    Exit(numBytes);
}
```

> Why I have to define the string in that way: [here](#assertion-failed-when-define-a-string-using-the-normal-c-way-for-user-program)

```txt
$ docker run -it nachos nachos/nachos-3.4/code/userprog/nachos -d S -x nachos/nachos-3.4/code/test/filesyscall
Received Create syscall (r4 = 1464): File "test.txt" created.
Received Open syscall (r4 = 1464): File "test.txt" opened.
Received Open syscall (r4 = 1464): File "test.txt" opened.
Received Write syscall (r4 = 1464, r5 = 8, r6 = 151082128): Write 8 bytes into file.
Received Read syscall (r4 = 1480, r5 = 8, r6 = 151082144): Read 8 bytes into buffer.
Received Close syscall (r4 = 151082128): File has closed.
Received Close syscall (r4 = 151082144): File has closed.
User program exit with status 8
```

And it will generate the "test.txt" file in docker under `code/userprog`.

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

The API of each of them defined as

```cpp
/* Address space control operations: Exit, Exec, and Join */

/* This user program is done (status = 0 means exited normally). */
void Exit(int status);

/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;

/* Run the executable, stored in the Nachos file "name", and return the
 * address space identifier
 */
SpaceId Exec(char *name);

/* Only return once the the user program "id" has finished.  
 * Return the exit status.
 */
int Join(SpaceId id);

/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program.
 */

/* Fork a thread to run a procedure ("func") in the *same* address space
 * as the current thread.
 */
void Fork(void (*func)());

/* Yield the CPU to another runnable thread, whether in this address space
 * or not.
 */
void Yield();
```

#### Exit

I've implemented the initial version Exit syscall in [virtual memory Lab](../Lab4_VirtualMemory/README.md#The-Exit-Syscall).
But this time, it requires not only receiving the return value but also the thread control.

Here is the following procedure that a Exit syscall need to do

1. 
2. 
3. 
4. 

#### Other Address Space Control Operations: Exec, Join

TBD

#### User-level Thread Operations: Fork, Yield

TBD

### Exercise 5: Test with user program

> Write user program and invoke the system calls implemented in the Exercise 4 and test the correctness.

#### Test Address Space Control Operations: Exit, Exec, Join

![Nachos Exec/Exit/Join Example](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/img005.gif)

In this test I'll create a child thread using `Exec` to execute the `code/test/exit` user program that I've made for [Lab4 Exercise5](../Lab4_VirtualMemory/README.md#Test-Exercise-5).

And then, after the child thread finish, it will `Join` back to the parent thread, then `Exit` and return the result state.

```c
/* execjoin.c
 *	Simple program to test the thread syscall (Lab 6) using the
 *  previous made "exit" program.
 *  This is used to test the address space control operations
 *  This will create executable Exec child with it and the parent will Join
 *  the thread and then Exit with the result
 */

#include "syscall.h"

int main() {
    char executable[5];
    int exitCode;
    SpaceId sp;

    executable[0] = 'e';
    executable[1] = 'x';
    executable[2] = 'i';
    executable[3] = 't';
    executable[4] = '\0';

    sp = Exec(executable);

    exitCode = Join(sp);
    Exit(exitCode);
}
```

Result:

TBD

#### Test User-level Thread Operations: Fork, Yield

In this test program, I'll first create a file called "text.txt" and `Fork` two threads.
These two threads will write character 'a' and 'b' in to the file for 20 and 10 times respectively.

```c
/* forkyield.c
 *	Simple program to test the thread syscall (Lab 6)
 *  This is used to test the user-level thread operations
 *  This example will create two thread and each of them
 *  will write something in the file.
 */

#include "syscall.h"

char file[9];

void ThreadA() {
    char ch = 'a';
    int i;
    OpenFileId fd;

    fd = Open(file);
    for(i = 0; i < 20; i ++) {
        Write(&ch, 1, fd);
        Yield();
    }
}

void ThreadB() {
    char ch = 'b';
    int i;
    OpenFileId fd;

    fd = Open(file);
    for(i = 0; i < 10; i ++) {
        Write(&ch, 1, fd);
        Yield();
    }
}

int main() {
    file[0] = 't';
    file[1] = 'e';
    file[2] = 's';
    file[3] = 't';
    file[4] = '.';
    file[5] = 't';
    file[6] = 'x';
    file[7] = 't';
    file[8] = '\0';

    Create(file);

    Fork(ThreadA);
    Fork(ThreadB);
}
```

Result:

TBD (Debug message of `-d S`)

TBD (the "test.txt" content)

## Trouble Shooting

### Assertion failed when define a string using the normal C way for user program

If define a string like this

```c
char data[5] = "test";
```

It will cause `Assertion failed: line 286, file "../machine/mipssim.cc"`

```c
// ReadMem assumes all 4 byte requests are aligned on an even
// word boundary.  Also, the little endian/big endian swap code would
// fail (I think) if the other cases are ever exercised.
ASSERT((tmp & 0x3) == 0);  
```

But if using this way, then it can prevent from it.

```c
char data[5];

data[0] = 't';
data[1] = 'e';
data[2] = 's';
data[3] = 't';
data[4] = '\0';
```

## Reminder

* I'm not sure the filesys syscall test should use `code/filesys/nachos` or `code/userprog/nachos`.
  * Using the first one will cause "Unable to open file ..." error
  * Using the second one, because the `code/userprog/Makefile` use `FILESYS_STUB`, not sure if it is legal, but it will enter the exception handler, so not quite sure.

TODO

* [ ] Maybe print something when `Read` and `Write`
* [ ] Check if the `if (!success) i--;` is necessary in `Read` and `Write`.
* [ ] More detail notes and deeper implementation.
* [ ] Exercise 5 test result when finish Exercise 4

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
* [Lecture 7](http://condor.depaul.edu/glancast/546class/docs/lec7.html)
  * **Exec**
  * StartProcess
  * Thread::Fork
  * AddrSpace
  * AddrSpace::AddrSpace(char * name, OpenFile* executable)
  * AddrSpace::mapPages()
  * AddrSpace::loadpage(int vpn, int ppn)
* [Lecture 8](http://condor.depaul.edu/glancast/546class/docs/lec8.html)
  * **Exec**
  * Thread 'name' Data Member
  * Next Assignment
  * Unix fork and wait
  * Join
* [**Lecture 9**](http://condor.depaul.edu/glancast/546class/docs/lec9.html)
  * Join
  * **Thread Class Additions**
  * List Class Additions
  * rcsmerge
  * Join Details
  * Exec Changes
  * **Exit Changes**
* [Assignment: Implementing Join](http://condor.depaul.edu/glancast/546class/docs/lec10.html)

[CPS 110 / EE 153 Operating Systems (Duke University)](https://users.cs.duke.edu/~chase/cps110/)

* [**Nachos Exec/Exit/Join Example**](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld005.htm) - [Home - Process, Unix and IPC](https://www2.cs.duke.edu/courses/cps110/spring00/slides/proc-ux/sld001.htm)
  * [pdf - The Classical OS Model in Unix](https://users.cs.duke.edu/~chase/cps210-archive/slides/ux6.pdf)

### Nachos Related

#### System Call Entry

* [PTT C_and_CPP - Re: [問題] start.s在Nachos下的作用](https://www.ptt.cc/bbs/C_and_CPP/M.1534860829.A.C66.html)

### Example

* [nachos-Lab6實習報告](https://wenku.baidu.com/view/bd03f40ee97101f69e3143323968011ca300f71e.html) - This guy written wrong things = =
* [SergioShen/Nachos comment: Finish lab6 exercise3: Syscall: Create, Open, Close, Write, Read](https://github.com/SergioShen/Nachos/commit/7343b11284c10b0ed22aca9200d91c1e66e15a36)
* [SergioShen/Nachos comment: Finish lab6](https://github.com/SergioShen/Nachos/commit/3015618577c3b4b4f1b441f9e18b0f42f4f4851c) - Exec, Fork, Yield, Join
* [SergioShen/Nachos exception.cc](https://github.com/SergioShen/Nachos/blob/latest/code/userprog/exception.cc)

Others

* [System call definitions](https://users.cs.duke.edu/~carla/syscall_def.html)

Yet Another

* [srijanshetty/nachos-syscalls](https://github.com/srijanshetty/nachos-syscalls)
* [manikantareddyd/NACHOS-syscall-implementation](https://github.com/manikantareddyd/NACHOS-syscall-implementation/)

Just.. a little bit related

* [CSDN - 【nachos】山東大學操作系統課設實驗nachos系統（6）系統調用Exec()和Exit()](https://blog.csdn.net/mottled233/article/details/78633571)
* [OS::NachOS::HW1 - System call: Sleep](http://blog.terrynini.tw/tw/OS-NachOS-HW1/#System-call-Sleep)
