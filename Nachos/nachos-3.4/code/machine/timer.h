// timer.h 
//	Data structures to emulate a hardware timer.
//
//	A hardware timer generates a CPU interrupt every X milliseconds.
//	This means it can be used for implementing time-slicing, or for
//	having a thread go to sleep for a specific period of time. 
//
//	We emulate a hardware timer by scheduling an interrupt to occur
//	every time stats->totalTicks has increased by TimerTicks.
//
//	In order to introduce some randomness into time-slicing, if "doRandom"
//	is set, then the interrupt comes after a random number of ticks.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef TIMER_H
#define TIMER_H

#include "copyright.h"
#include "utility.h"

// The following class defines a hardware timer. 
class Timer {
  public:
    Timer(VoidFunctionPtr timerHandler, int callArg, bool doRandom);
				// Initialize the timer, to call the interrupt
				// handler "timerHandler" every time slice.
    ~Timer() {}

// Internal routines to the timer emulation -- DO NOT call these

    void TimerExpired();	// called internally when the hardware
				// timer generates an interrupt

    int TimeOfNextInterrupt();  // figure out when the timer will generate
				// its next interrupt 

  private:
    bool randomize;		// set if we need to use a random timeout delay
    VoidFunctionPtr handler;	// timer interrupt handler 
    int arg;			// argument to pass to interrupt handler

};

#endif // TIMER_H
