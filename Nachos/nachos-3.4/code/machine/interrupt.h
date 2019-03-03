// interrupt.h 
//	Data structures to emulate low-level interrupt hardware.
//
//	The hardware provides a routine (SetLevel) to enable or disable
//	interrupts.
//
//	In order to emulate the hardware, we need to keep track of all
//	interrupts the hardware devices would cause, and when they
//	are supposed to occur.  
//
//	This module also keeps track of simulated time.  Time advances
//	only when the following occur: 
//		interrupts are re-enabled
//		a user instruction is executed
//		there is nothing in the ready queue
//
//	As a result, unlike real hardware, interrupts (and thus time-slice 
//	context switches) cannot occur anywhere in the code where interrupts
//	are enabled, but rather only at those places in the code where 
//	simulated time advances (so that it becomes time to invoke an
//	interrupt in the hardware simulation).
//
//	NOTE: this means that incorrectly synchronized code may work
//	fine on this hardware simulation (even with randomized time slices),
//	but it wouldn't work on real hardware.  (Just because we can't
//	always detect when your program would fail in real life, does not 
//	mean it's ok to write incorrectly synchronized code!)
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "copyright.h"
#include "list.h"

// Interrupts can be disabled (IntOff) or enabled (IntOn)
enum IntStatus { IntOff, IntOn };

// Nachos can be running kernel code (SystemMode), user code (UserMode),
// or there can be no runnable thread, because the ready list 
// is empty (IdleMode).
enum MachineStatus {IdleMode, SystemMode, UserMode};

// IntType records which hardware device generated an interrupt.
// In Nachos, we support a hardware timer device, a disk, a console
// display and keyboard, and a network.
enum IntType { TimerInt, DiskInt, ConsoleWriteInt, ConsoleReadInt, 
				ElevatorInt, NetworkSendInt, NetworkRecvInt};

// The following class defines an interrupt that is scheduled
// to occur in the future.  The internal data structures are
// left public to make it simpler to manipulate.

class PendingInterrupt {
  public:
    PendingInterrupt(VoidFunctionPtr func, int param, int time, IntType kind);
				// initialize an interrupt that will
				// occur in the future

    VoidFunctionPtr handler;    // The function (in the hardware device
				// emulator) to call when the interrupt occurs
    int arg;                    // The argument to the function.
    int when;			// When the interrupt is supposed to fire
    IntType type;		// for debugging
};

// The following class defines the data structures for the simulation
// of hardware interrupts.  We record whether interrupts are enabled
// or disabled, and any hardware interrupts that are scheduled to occur
// in the future.

class Interrupt {
  public:
    Interrupt();			// initialize the interrupt simulation
    ~Interrupt();			// de-allocate data structures
    
    IntStatus SetLevel(IntStatus level);// Disable or enable interrupts 
					// and return previous setting.

    void Enable();			// Enable interrupts.
    IntStatus getLevel() {return level;}// Return whether interrupts
					// are enabled or disabled
    
    void Idle(); 			// The ready queue is empty, roll 
					// simulated time forward until the 
					// next interrupt

    void Halt(); 			// quit and print out stats
    
    void YieldOnReturn();		// cause a context switch on return 
					// from an interrupt handler

    MachineStatus getStatus() { return status; } // idle, kernel, user
    void setStatus(MachineStatus st) { status = st; }

    void DumpState();			// Print interrupt state
    

    // NOTE: the following are internal to the hardware simulation code.
    // DO NOT call these directly.  I should make them "private",
    // but they need to be public since they are called by the
    // hardware device simulators.

    void Schedule(VoidFunctionPtr handler,// Schedule an interrupt to occur
	int arg, int when, IntType type);// at time ``when''.  This is called
    					// by the hardware device simulators.
    
    void OneTick();       		// Advance simulated time

  private:
    IntStatus level;		// are interrupts enabled or disabled?
    List *pending;		// the list of interrupts scheduled
				// to occur in the future
    bool inHandler;		// TRUE if we are running an interrupt handler
    bool yieldOnReturn; 	// TRUE if we are to context switch
				// on return from the interrupt handler
    MachineStatus status;	// idle, kernel mode, user mode

    // these functions are internal to the interrupt simulation code

    bool CheckIfDue(bool advanceClock); // Check if an interrupt is supposed
					// to occur now

    void ChangeLevel(IntStatus old, 	// SetLevel, without advancing the
	IntStatus now);  		// simulated time
};

#endif // INTERRRUPT_H
