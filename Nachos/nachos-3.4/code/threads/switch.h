/* switch.h
 *	Definitions needed for implementing context switching.
 *
 *	Context switching is inherently machine dependent, since
 *	the registers to be saved, how to set up an initial
 *	call frame, etc, are all specific to a processor architecture.
 *
 * 	This file currently supports the DEC MIPS and SUN SPARC architectures.
 */

/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#ifndef SWITCH_H
#define SWITCH_H

#include "copyright.h"

#ifdef HOST_MIPS

/* Registers that must be saved during a context switch. 
 * These are the offsets from the beginning of the Thread object, 
 * in bytes, used in switch.s
 */
#define SP 0
#define S0 4
#define S1 8
#define S2 12
#define S3 16
#define S4 20
#define S5 24
#define S6 28
#define S7 32
#define FP 36
#define PC 40

/* To fork a thread, we set up its saved register state, so that
 * when we switch to the thread, it will start running in ThreadRoot.
 *
 * The following are the initial registers we need to set up to
 * pass values into ThreadRoot (for instance, containing the procedure
 * for the thread to run).  The first set is the registers as used
 * by ThreadRoot; the second set is the locations for these initial
 * values in the Thread object -- used in Thread::AllocateStack().
 */

#define InitialPC	s0
#define InitialArg	s1
#define WhenDonePC	s2
#define StartupPC	s3

#define PCState		(PC/4-1)
#define FPState		(FP/4-1)
#define InitialPCState	(S0/4-1)
#define InitialArgState	(S1/4-1)
#define WhenDonePCState	(S2/4-1)
#define StartupPCState	(S3/4-1)

#endif 	// HOST_MIPS

#ifdef HOST_SPARC

/* Registers that must be saved during a context switch.  See comment above. */ 
#define I0 4
#define I1 8
#define I2 12
#define I3 16
#define I4 20
#define I5 24
#define I6 28
#define I7 32

/* Aliases used for clearing code.  */
#define FP I6
#define PC I7

/* Registers for ThreadRoot.  See comment above. */
#define InitialPC       %o0
#define InitialArg      %o1
#define WhenDonePC      %o2
#define StartupPC       %o3

#define PCState         (PC/4-1)
#define InitialPCState  (I0/4-1)
#define InitialArgState (I1/4-1)
#define WhenDonePCState (I2/4-1)
#define StartupPCState  (I3/4-1)
#endif 	// HOST_SPARC

#ifdef HOST_SNAKE

/* Registers that must be saved during a context switch.  See comment above. */ 
#define   SP   0
#define   S0   4
#define   S1   8
#define   S2   12
#define   S3   16
#define   S4   20
#define   S5   24
#define   S6   28
#define   S7   32
#define   S8   36
#define   S9   40
#define   S10  44
#define   S11  48
#define   S12  52
#define   S13  56
#define   S14  60
#define   S15  64
#define   PC   68

/* Registers for ThreadRoot.  See comment above. */
#define InitialPC       %r3		/* S0 */
#define InitialArg      %r4
#define WhenDonePC      %r5
#define StartupPC       %r6

#define PCState         (PC/4-1)
#define InitialPCState  (S0/4-1)
#define InitialArgState (S1/4-1)
#define WhenDonePCState (S2/4-1)
#define StartupPCState  (S3/4-1)
#endif 	// HOST_SNAKE

#ifdef HOST_i386

/* the offsets of the registers from the beginning of the thread object */
#define _ESP     0
#define _EAX     4
#define _EBX     8
#define _ECX     12
#define _EDX     16
#define _EBP     20
#define _ESI     24
#define _EDI     28
#define _PC      32

/* These definitions are used in Thread::AllocateStack(). */
#define PCState         (_PC/4-1)
#define FPState         (_EBP/4-1)
#define InitialPCState  (_ESI/4-1)
#define InitialArgState (_EDX/4-1)
#define WhenDonePCState (_EDI/4-1)
#define StartupPCState  (_ECX/4-1)

#define InitialPC       %esi
#define InitialArg      %edx
#define WhenDonePC      %edi
#define StartupPC       %ecx
#endif

#endif // SWITCH_H
