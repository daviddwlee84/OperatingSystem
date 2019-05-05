// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

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
    // Lab4: Page Fault Handling
    if (which == PageFaultException) {
        if (machine->tlb == NULL) { // linear page table page fault
	        DEBUG('m', "=> Linear page table page fault.\n");
            // In current Nachos this shouldn't happen
            // because physical page frame == virtual page number
            // (can be found in AddrSpace::AddrSpace in userprog/addrspace.cc)
            // On the other hand, in our Lab we won't use linear page table at all
            ASSERT(FALSE);
        } else { // TLB miss (no TLB entry)
            // Lab4 Exercise 2 and 3 and 7
	        DEBUG('m', "=> TLB miss (no TLB entry)\n");
            int BadVAddr = machine->ReadRegister(BadVAddrReg); // The failing virtual address on an exception
            TLBMissHandler(BadVAddr);
        }
        return;
    }

    // System Call
    // The system call codes (SC_[TYPE]) is defined in userprog/syscall.h
    // The system call stubs is defined in test/start.s
    int type = machine->ReadRegister(2); // r2: the standard C calling convention on the MIPS

    if ((which == SyscallException) && (type == SC_Halt)) {
        DEBUG('a', "Shutdown, initiated by user program.\n");
#ifdef USE_TLB
        // Lab4: Used for calculate TLB Miss rate (debug purpose)
        printf("TLB Miss: %d, TLB Hit: %d, Total Translate: %d, TLB Miss Rate: %.2lf%%\n",
            TLBMissCount, TranslateCount - TLBMissCount, TranslateCount, (double)TLBMissCount/TranslateCount*100);
#endif
        interrupt->Halt();
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}

/**********************************************************************/
/*************************** Lab4: TLB Miss ***************************/
/**********************************************************************/

// Lab4 Exercise3
// #define TLB_FIFO TRUE
#define TLB_CLOCK TRUE
// #define TLB_LRU TRUE

// Lab4 Exercise2
#if !(TLB_FIFO || TLB_LRU)
int TLBreplaceIdx = 0; // When using TLB_CLOCK, this is circular pointer
#endif

//----------------------------------------------------------------------
// TLBMissHandler
// 	1. Calculate vpn (virtual page number)
//  2. See if the page is valid, if not this is a page fault
//  3. Update TLB (algorithms to find a position to replace)
//----------------------------------------------------------------------

void
TLBMissHandler(int virtAddr)
{
    unsigned int vpn;
    vpn = (unsigned) virtAddr / PageSize;

    // Find the Page
    TranslationEntry page = machine->pageTable[vpn];
    if (!page.valid) { // Lab4 Exercise7
        DEBUG('m', "\t=> Page Miss\n");
        page = PageFaultHandler(vpn);
    }

    // Update TLB
#if TLB_FIFO
    TLBAlgoFIFO(page);
#elif TLB_CLOCK
    TLBAlgoClock(page);
#elif TLB_LRU
    TLBAlgoLRU(page);
#else
    // ONLY USE FOR TESTING Lab4 Exercise2
    // i.e. assume TLBSize = 2
    machine->tlb[TLBreplaceIdx] = page;
    TLBreplaceIdx = TLBreplaceIdx ? 0 : 1;
#endif

}

//----------------------------------------------------------------------
// TLBAlgoFIFO
//  The most naive replacement algorithm
//----------------------------------------------------------------------

#ifdef TLB_FIFO
void
TLBAlgoFIFO(TranslationEntry page)
{
    int TLBreplaceIdx = -1;
    // Find the empty entry
    for (int i = 0; i < TLBSize; i++) {
        if (machine->tlb[i].valid == FALSE) {
            TLBreplaceIdx = i;
            break;
        }
    }
    // If full then move everything forward and remove the last one
    if (TLBreplaceIdx == -1) {
        TLBreplaceIdx = TLBSize - 1;
        for (int i = 0; i < TLBSize - 1; i++) {
            machine->tlb[i] = machine->tlb[i+1];
        }
    }
    // Update TLB
    machine->tlb[TLBreplaceIdx] = page;
}
#endif

//----------------------------------------------------------------------
// TLBAlgoClock
//  Basically the second-chance replacement algorithm with circular list/pointer	
//----------------------------------------------------------------------

#ifdef TLB_CLOCK
void
TLBAlgoClock(TranslationEntry page)
{
    // Find the next one
    // if used then clear to 0 and continue find the next one.
    // until find the one that is not used.
    while (1) {
        TLBreplaceIdx %= TLBSize;
        if (machine->tlb[TLBreplaceIdx].valid == FALSE) {
            break;
        } else  {
            if (machine->tlb[TLBreplaceIdx].use) {
                // Found the entry is recently used
                // clear the R bit and find next
                machine->tlb[TLBreplaceIdx].use = FALSE;
                TLBreplaceIdx++;
            } else {
                // Evict the entry
                break;
            }
        }
    }

    // Update TLB
    machine->tlb[TLBreplaceIdx] = page;
    machine->tlb[TLBreplaceIdx].use = TRUE;
}
#endif

//----------------------------------------------------------------------
// TLBAlgoLRU
// 	
//----------------------------------------------------------------------

#ifdef TLB_LRU
void
TLBAlgoLRU(TranslationEntry page)
{

}
#endif

/**********************************************************************/
/************************ Lab4: Demand Paging *************************/
/**********************************************************************/


//----------------------------------------------------------------------
// PageFaultHandler
// 	
//----------------------------------------------------------------------

TranslationEntry
PageFaultHandler(int virtAddr)
{

}
