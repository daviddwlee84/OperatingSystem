// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable)
{
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;	// we need to increase the size
						// to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

#ifndef DEMAND_PAGING
    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
						// virtual memory
#else
    // Create a virtual memory with the size that the executable file need.
    DEBUG('a', "Demand paging: creating virtual memory!\n");
    bool success_create_vm = fileSystem->Create("VirtualMemory", size);
    ASSERT_MSG(success_create_vm, "fail to create virtual memory");
#endif

#ifndef INVERTED_PAGETABLE
    DEBUG('a', "Initializing address space (page table), num pages %d, size %d\n", 
					numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
	pageTable[i].virtualPage = i;	// for now (default), virtual page # = phys page #

#ifndef DEMAND_PAGING // Lab4: Lazy-loading (Demand paging)
#if USE_BITMAP // Lab4: Global data structure for memory management
    pageTable[i].physicalPage = machine->allocateFrame();
    ASSERT_MSG(pageTable[i].physicalPage != -1, "No free page frame."); // Currently, all page need to be loaded into the memory
#elif USE_LINKED_LIST
    // TODO
#else // not using data structure (i.e. Original linear allocation)
	pageTable[i].physicalPage = i;
#endif // Free memory space data structure
	pageTable[i].valid = TRUE;
#else // Demand Paging
    pageTable[i].valid = FALSE;
#endif
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
    }
#if USE_BITMAP && !DEMAND_PAGING
    DEBUG('M', "Bitmap after allocate: %08X\n", machine->bitmap);
#endif // DEMAND_PAGING
#else // Use Inverted Page Table
    for (i = 0; i < numPages; i++) {
        machine->pageTable[i].physicalPage = machine->allocateFrame(); // Currently don't support demand paging
        machine->pageTable[i].valid = TRUE;
        machine->pageTable[i].use = FALSE;
        machine->pageTable[i].dirty = FALSE;
        machine->pageTable[i].readOnly = FALSE;

        machine->pageTable[i].threadId = currentThread->getThreadId(); // The additional part of inverted page table
    }
    DEBUG('M', "Initialized memory for thread \"%s\".\n", currentThread->getName());
#endif // INVERTED_PAGETABLE
    
#ifndef DEMAND_PAGING // If use lazy loading then don't load executable into memory at first.
// zero out the entire (needed) address space (size), to zero the unitialized data segment 
// and the stack segment
    bzero(machine->mainMemory, size);

#if USE_BITMAP
    // TODO: Current memroy load sequentially, but what we need is load the memory into
    //       the page that allocate by bitmap.

    // char *tempMemory;
    // tempMemory = new char[size];
    // for (i = 0; i < size; i++)
    //     tempMemory[i] = 0;

    // if (noffH.code.size > 0) {
    //     DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
	// 		noffH.code.virtualAddr, noffH.code.size);
    //     executable->ReadAt(&(tempMemory[noffH.code.virtualAddr]),
	// 		noffH.code.size, noffH.code.inFileAddr);
    // }
    // if (noffH.initData.size > 0) {
    //     DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
	// 		noffH.initData.virtualAddr, noffH.initData.size);
    //     executable->ReadAt(&(tempMemory[noffH.initData.virtualAddr]),
	// 		noffH.initData.size, noffH.initData.inFileAddr);
    // }

    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }
#else // original version
    // then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }
#endif // USE_BITMAP
#else // Use demand paging
// zero out the entire address space (MemorySize), to zero the unitialized data segment 
// and the stack segment
    bzero(machine->mainMemory, MemorySize);

    DEBUG('a', "Demand paging: copy executable to virtual memory!\n");

    OpenFile *vm = fileSystem->Open("VirtualMemory");

    char *virtualMemory_temp;
    virtualMemory_temp = new char[size];
    for (i = 0; i < size; i++)
        virtualMemory_temp[i] = 0;

    if (noffH.code.size > 0) {
        DEBUG('a', "\tCopying code segment, at 0x%x, size %d\n",
              noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(virtualMemory_temp[noffH.code.virtualAddr]),
                           noffH.code.size, noffH.code.inFileAddr);
        vm->WriteAt(&(virtualMemory_temp[noffH.code.virtualAddr]),
                    noffH.code.size, noffH.code.virtualAddr*PageSize);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "\tCopying data segment, at 0x%x, size %d\n",
              noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(virtualMemory_temp[noffH.initData.virtualAddr]),
                           noffH.initData.size, noffH.initData.inFileAddr);
        vm->WriteAt(&(virtualMemory_temp[noffH.initData.virtualAddr]),
                    noffH.initData.size, noffH.initData.virtualAddr*PageSize);
    }
    delete vm; // Close the file
#endif

}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
   delete pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{
#ifdef USE_TLB // Lab4: Clean up TLB
    DEBUG('T', "Clean up TLB due to Context Switch!\n");
    for (int i = 0; i < TLBSize; i++) {
        machine->tlb[i].valid = FALSE;
    }
#endif
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
// If using inverted page table, because there is only one page table (for a machine)
// So we won't need the address space page table
#ifndef INVERTED_PAGETABLE
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
#endif
}

//----------------------------------------------------------------------
// AddrSpace::PrintState
// 	Print the state of this address space. (debug usage)
//----------------------------------------------------------------------

void
AddrSpace::PrintState() 
{
    printf("=== %s ===\n", COLORED(BLUE, "Address Space Information"));
    printf("numPages = %d\n", numPages);
    printf("VPN\tPPN\tvalid\tRO\tuse\tdirty\n");
    for (int i = 0; i < numPages; i++) {
        printf("%d\t", pageTable[i].virtualPage);
        printf("%d\t", pageTable[i].physicalPage);
        printf("%d\t", pageTable[i].valid);
        printf("%d\t", pageTable[i].use);
        printf("%d\t", pageTable[i].dirty);
        printf("%d\t", pageTable[i].readOnly);
        printf("\n");
    }
#if USE_BITMAP
    DEBUG('M', "Current Bitmap: %08X\n", machine->bitmap);
#endif
    printf("=================================\n");
}
