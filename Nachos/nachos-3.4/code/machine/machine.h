// machine.h 
//	Data structures for simulating the execution of user programs
//	running on top of Nachos.
//
//	User programs are loaded into "mainMemory"; to Nachos,
//	this looks just like an array of bytes.  Of course, the Nachos
//	kernel is in memory too -- but as in most machines these days,
//	the kernel is loaded into a separate memory region from user
//	programs, and accesses to kernel memory are not translated or paged.
//
//	In Nachos, user programs are executed one instruction at a time, 
//	by the simulator.  Each memory reference is translated, checked
//	for errors, etc.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef MACHINE_H
#define MACHINE_H

#include "copyright.h"
#include "utility.h"
#include "translate.h"
#include "disk.h"

// Definitions related to the size, and format of user memory

#define PageSize 	SectorSize 	// set the page size equal to
					// the disk sector size, for
					// simplicity

#define NumPhysPages    32
#define MemorySize 	(NumPhysPages * PageSize)
#define TLBSize		4		// if there is a TLB, make it small

enum ExceptionType { NoException,           // Everything ok!
		     SyscallException,      // A program executed a system call.
		     PageFaultException,    // No valid translation found
		     ReadOnlyException,     // Write attempted to page marked 
					    // "read-only"
		     BusErrorException,     // Translation resulted in an 
					    // invalid physical address
		     AddressErrorException, // Unaligned reference or one that
					    // was beyond the end of the
					    // address space
		     OverflowException,     // Integer overflow in add or sub.
		     IllegalInstrException, // Unimplemented or reserved instr.
		     
		     NumExceptionTypes
};

// User program CPU state.  The full set of MIPS registers, plus a few
// more because we need to be able to start/stop a user program between
// any two instructions (thus we need to keep track of things like load
// delay slots, etc.)

#define StackReg	29	// User's stack pointer
#define RetAddrReg	31	// Holds return address for procedure calls
#define NumGPRegs	32	// 32 general purpose registers on MIPS
#define HiReg		32	// Double register to hold multiply result
#define LoReg		33
#define PCReg		34	// Current program counter
#define NextPCReg	35	// Next program counter (for branch delay) 
#define PrevPCReg	36	// Previous program counter (for debugging)
#define LoadReg		37	// The register target of a delayed load.
#define LoadValueReg 	38	// The value to be loaded by a delayed load.
#define BadVAddrReg	39	// The failing virtual address on an exception

#define NumTotalRegs 	40

// The following class defines an instruction, represented in both
// 	undecoded binary form
//      decoded to identify
//	    operation to do
//	    registers to act on
//	    any immediate operand value

class Instruction {
  public:
    void Decode();	// decode the binary representation of the instruction

    unsigned int value; // binary representation of the instruction

    char opCode;     // Type of instruction.  This is NOT the same as the
    		     // opcode field from the instruction: see defs in mips.h
    char rs, rt, rd; // Three registers from instruction.
    int extra;       // Immediate or target or shamt field or offset.
                     // Immediates are sign-extended.
};

// The following class defines the simulated host workstation hardware, as 
// seen by user programs -- the CPU registers, main memory, etc.
// User programs shouldn't be able to tell that they are running on our 
// simulator or on the real hardware, except 
//	we don't support floating point instructions
//	the system call interface to Nachos is not the same as UNIX 
//	  (10 system calls in Nachos vs. 200 in UNIX!)
// If we were to implement more of the UNIX system calls, we ought to be
// able to run Nachos on top of Nachos!
//
// The procedures in this class are defined in machine.cc, mipssim.cc, and
// translate.cc.

class Machine {
  public:
    Machine(bool debug);	// Initialize the simulation of the hardware
				// for running user programs
    ~Machine();			// De-allocate the data structures

// Routines callable by the Nachos kernel
    void Run();	 		// Run a user program

    int ReadRegister(int num);	// read the contents of a CPU register

    void WriteRegister(int num, int value);
				// store a value into a CPU register


// Routines internal to the machine simulation -- DO NOT call these 

    void OneInstruction(Instruction *instr); 	
    				// Run one instruction of a user program.
    void DelayedLoad(int nextReg, int nextVal);  	
				// Do a pending delayed load (modifying a reg)
    
    bool ReadMem(int addr, int size, int* value);
    bool WriteMem(int addr, int size, int value);
    				// Read or write 1, 2, or 4 bytes of virtual 
				// memory (at addr).  Return FALSE if a 
				// correct translation couldn't be found.
    
    ExceptionType Translate(int virtAddr, int* physAddr, int size,bool writing);
    				// Translate an address, and check for 
				// alignment.  Set the use and dirty bits in 
				// the translation entry appropriately,
    				// and return an exception code if the 
				// translation couldn't be completed.

    void RaiseException(ExceptionType which, int badVAddr);
				// Trap to the Nachos kernel, because of a
				// system call or other exception.  

    void Debugger();		// invoke the user program debugger
    void DumpState();		// print the user CPU and memory state 


// Data structures -- all of these are accessible to Nachos kernel code.
// "public" for convenience.
//
// Note that *all* communication between the user program and the kernel 
// are in terms of these data structures.

    char *mainMemory;		// physical memory to store user program,
				// code and data, while executing
    int registers[NumTotalRegs]; // CPU registers, for executing user programs


// NOTE: the hardware translation of virtual addresses in the user program
// to physical addresses (relative to the beginning of "mainMemory")
// can be controlled by one of:
//	a traditional linear page table
//  	a software-loaded translation lookaside buffer (tlb) -- a cache of 
//	  mappings of virtual page #'s to physical page #'s
//
// If "tlb" is NULL, the linear page table is used
// If "tlb" is non-NULL, the Nachos kernel is responsible for managing
//	the contents of the TLB.  But the kernel can use any data structure
//	it wants (eg, segmented paging) for handling TLB cache misses.
// 
// For simplicity, both the page table pointer and the TLB pointer are
// public.  However, while there can be multiple page tables (one per address
// space, stored in memory), there is only one TLB (implemented in hardware).
// Thus the TLB pointer should be considered as *read-only*, although 
// the contents of the TLB are free to be modified by the kernel software.

    TranslationEntry *tlb;		// this pointer should be considered 
					// "read-only" to Nachos kernel code

    TranslationEntry *pageTable;
    unsigned int pageTableSize;

  private:
    bool singleStep;		// drop back into the debugger after each
				// simulated instruction
    int runUntilTime;		// drop back into the debugger when simulated
				// time reaches this value
};

extern void ExceptionHandler(ExceptionType which);
				// Entry point into Nachos for handling
				// user system calls and exceptions
				// Defined in exception.cc


// Routines for converting Words and Short Words to and from the
// simulated machine's format of little endian.  If the host machine
// is little endian (DEC and Intel), these end up being NOPs.
//
// What is stored in each format:
//	host byte ordering:
//	   kernel data structures
//	   user registers
//	simulated machine byte ordering:
//	   contents of main memory

unsigned int WordToHost(unsigned int word);
unsigned short ShortToHost(unsigned short shortword);
unsigned int WordToMachine(unsigned int word);
unsigned short ShortToMachine(unsigned short shortword);

#endif // MACHINE_H
