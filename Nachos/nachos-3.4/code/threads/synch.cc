// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::~Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

/**********************************************************************/
/************** Lab3: Mutex Lock and Condition Variable ***************/
/**********************************************************************/

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!

//----------------------------------------------------------------------
// Lock::Lock
// 	Initialize a lock, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------

Lock::Lock(char* debugName)
{
    name = debugName;
    semaphore = new Semaphore("Lock", 1);
}

//----------------------------------------------------------------------
// Lock::~Lock
// 	De-allocate lock, when no longer needed.  Assume no one
//	is still waiting on the lock!
//----------------------------------------------------------------------

Lock::~Lock()
{
    delete semaphore;
}

//----------------------------------------------------------------------
// Lock::Acquire
// 	Acquire Mutex Lock.
//	* When lock is BUSY, enter SLEEP state. (if loop and wait => spinlock)
//	* When lock is FREE, current Thread get lock and keep running.
//----------------------------------------------------------------------

void
Lock::Acquire()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    DEBUG('s', "Lock \"%s\" Acquired by Thread \"%s\"\n", name, currentThread->getName());
    semaphore->P(); // value 1 => 0
    holderThread = currentThread;

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Lock::Release
// 	Release Mutex Lock.
//  (Note: Only the Thread which own this lock can release lock)
//	Set lock status to FREE. If any other Thread is waiting this lock,
//  wake one of them up, enter READY state.
//----------------------------------------------------------------------

void
Lock::Release()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    DEBUG('s', "Lock \"%s\" Released by Thread \"%s\"\n", name, currentThread->getName());
    // make sure the owner of this lock is currentThread
    ASSERT(this->isHeldByCurrentThread());
    holderThread = NULL;
    semaphore->V(); // value 0 => 1

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Lock::isHeldByCurrentThread
//  true if the current thread holds this lock.
//----------------------------------------------------------------------

bool Lock::isHeldByCurrentThread()
{
    return currentThread == holderThread;
}

//----------------------------------------------------------------------
// Condition::Condition
// 	Initialize a Condition Variable, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------

Condition::Condition(char* debugName)
{
    name = debugName;
    waitQueue = new List();
}

//----------------------------------------------------------------------
// Condition::~Condition
// 	De-allocate condition variable, when no longer needed.  Assume no one
//	is still waiting on the condition variable!
//----------------------------------------------------------------------

Condition::~Condition()
{
    delete waitQueue;
}

//----------------------------------------------------------------------
// Condition::Wait
//  Wait blocks the calling thread until the specified condition is signalled.
//  This routine should be called while mutex is locked, and it will
//  automatically release the mutex while it waits.
//  After signal is received and thread is awakened,
//  mutex will be automatically locked for use by the thread.
//  The programmer is then responsible for unlocking mutex when the thread
//  is finished with it.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Wait(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the currentThread
    ASSERT(conditionLock->isHeldByCurrentThread())
    // conditionLock must be locked
    ASSERT(conditionLock->isLocked());

    waitQueue->Append(currentThread);

    // Release the lock while it waits
    conditionLock->Release();
    currentThread->Sleep();

    // Awake by Signal...

    conditionLock->Acquire();

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Condition::Signal
//  Signal is used to signal (or wake up) another thread which is waiting
//  on the condition variable. It should be called after mutex is locked,
//  and must unlock mutex in order for Condition::Wait() routine to complete.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Signal(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the current Thread
    ASSERT(conditionLock->isHeldByCurrentThread())

    if (!waitQueue->IsEmpty()) {
        // Putting thread from the front of waitQueue onto ready list
        Thread* thread = (Thread*) waitQueue->Remove();
        scheduler->ReadyToRun(thread);
    }

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Condition::Broadcast
//  Wakeup all the threads waiting on this condition.
//  Brodcast should be used instead of Condition::Signal() if more than
//  one thread is in a blocking wait state.
//
//  "conditionLock" is the lock protecting the use of this condition
//----------------------------------------------------------------------

void
Condition::Broadcast(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    // conditionLock must be held by the current Thread
    ASSERT(conditionLock->isHeldByCurrentThread())

    while (!waitQueue->IsEmpty()) {
        // Putting all the threads on ready list
        Thread* thread = (Thread*) waitQueue->Remove();
        scheduler->ReadyToRun(thread);
    }

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
