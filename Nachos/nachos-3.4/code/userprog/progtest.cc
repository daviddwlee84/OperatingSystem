// progtest.cc 
//	Test routines for demonstrating that Nachos can load
//	a user program and execute it.  
//
//	Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

//----------------------------------------------------------------------
// StartProcess
// 	Run a user program.  Open the executable, load it into
//	memory, and jump to it.
//  (ps. use -x filename, detail information is in thread/main.c)
//----------------------------------------------------------------------

void
StartProcess(char *filename)
{
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
	printf("Unable to open file %s\n", filename);
	return;
    }
    space = new AddrSpace(executable);    
    currentThread->space = space;

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
// 	Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

//----------------------------------------------------------------------
// ConsoleTest
// 	Test the console by echoing characters typed at the input onto
//	the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void 
ConsoleTest (char *in, char *out)
{
    char ch;

    console = new Console(in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    
    for (;;) {
	readAvail->P();		// wait for character to arrive
	ch = console->GetChar();
	console->PutChar(ch);	// echo it!
	writeDone->P() ;        // wait for write to finish
	if (ch == 'q') return;  // if q, quit
    }
}

/**********************************************************************/
/********************** Lab5: Synchronous Console *********************/
/**********************************************************************/

static SynchConsole *synchConsole;

//----------------------------------------------------------------------
// SynchConsoleTest
// 	Test the synchronous console by echoing characters typed at the input
//	onto the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void 
SynchConsoleTest (char *in, char *out)
{
    char ch;

    synchConsole = new SynchConsole(in, out);

    for (;;) {
        ch = synchConsole->GetChar();
        synchConsole->PutChar(ch); // echo it!
        if (ch == 'q')
            return; // if q, quit
    }
}

/**********************************************************************/
/************************* Lab4: Multi-thread *************************/
/**********************************************************************/

//----------------------------------------------------------------------
// UserProgThread
// 	A basic user program thread.
//----------------------------------------------------------------------

void
UserProgThread(int number)
{
    printf("Running user program thread %d\n", number);
    currentThread->space->InitRegisters();		// set the initial register values
    currentThread->space->RestoreState();		// load page table register
    currentThread->space->PrintState();         // debug usage
    machine->Run();	// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
                // the address space exits
                // by doing the syscall "exit"
}

//----------------------------------------------------------------------
// CreateSingleThread
// 	Run a user program.  Open the executable, load it into
//	memory, create a copy of it and return the thread.
//----------------------------------------------------------------------

Thread*
CreateSingleThread(OpenFile *executable, int number)
{
    printf("Creating user program thread %d\n", number);

    char ThreadName[20];
    sprintf(ThreadName, "User program %d", number);
    Thread *thread = new Thread(strdup(ThreadName), -1);

    AddrSpace *space;
    space = new AddrSpace(executable);
    thread->space = space;

    return thread;
}

//----------------------------------------------------------------------
// StartTwoThread
// 	Run a user program.  Open the executable, load it into
//	memory, create two copy of the thread and jump to it.
//  (ps. use -X filename, detail information is in thread/main.c)
//----------------------------------------------------------------------

void
StartTwoThread(char *filename)
{
    OpenFile *executable = fileSystem->Open(filename);

    if (executable == NULL) {
	    printf("Unable to open file %s\n", filename);
	    return;
    }

    Thread *thread1 = CreateSingleThread(executable, 1);
    Thread *thread2 = CreateSingleThread(executable, 2);

    // machine->DumpMemory(); // debug: shows current memory state

    delete executable;			// close file

    thread1->Fork(UserProgThread, (void*)1);
    thread2->Fork(UserProgThread, (void*)2);

    currentThread->Yield();
}
