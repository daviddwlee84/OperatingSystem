// utility.h 
//	Miscellaneous useful definitions, including debugging routines.
//
//	The debugging routines allow the user to turn on selected
//	debugging messages, controllable from the command line arguments
//	passed to Nachos (-d).  You are encouraged to add your own
//	debugging flags.  The pre-defined debugging flags are:
//
//	'+' -- turn on all debug messages
//   	't' -- thread system
//   	's' -- semaphores, locks, and conditions
//   	'i' -- interrupt emulation
//   	'm' -- machine emulation (USER_PROGRAM)
//   	'd' -- disk emulation (FILESYS)
//   	'f' -- file system (FILESYS)
//   	'a' -- address spaces (USER_PROGRAM)
//   	'n' -- network emulation (NETWORK)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef UTILITY_H
#define UTILITY_H

#include "copyright.h"

// Miscellaneous useful routines

#include "bool.h"
					 	// Boolean values.  
						// This is the same definition 
						// as in the g++ library.

#define min(a,b)  (((a) < (b)) ? (a) : (b))
#define max(a,b)  (((a) > (b)) ? (a) : (b))

// Divide and either round up or down 
#define divRoundDown(n,s)  ((n) / (s))
#define divRoundUp(n,s)    (((n) / (s)) + ((((n) % (s)) > 0) ? 1 : 0))

// This declares the type "VoidFunctionPtr" to be a "pointer to a
// function taking an integer argument and returning nothing".  With
// such a function pointer (say it is "func"), we can call it like this:
//
//	(*func) (17);
//
// This is used by Thread::Fork and for interrupt handlers, as well
// as a couple of other places.

typedef void (*VoidFunctionPtr)(int arg); 
typedef void (*VoidNoArgFunctionPtr)(); 


// Include interface that isolates us from the host machine system library.
// Requires definition of bool, and VoidFunctionPtr
#include "sysdep.h"				

// Interface to debugging routines.

extern void DebugInit(char* flags);	// enable printing debug messages

extern bool DebugIsEnabled(char flag); 	// Is this debug flag enabled?

extern void DEBUG (char flag, char* format, ...);  	// Print debug message 
							// if flag is enabled

//----------------------------------------------------------------------
// ASSERT
//      If condition is false,  print a message and dump core.
//	Useful for documenting assumptions in the code.
//
//	NOTE: needs to be a #define, to be able to print the location 
//	where the error occurred.
//----------------------------------------------------------------------
#define ASSERT(condition)                                                          \
    if (!(condition))                                                              \
    {                                                                              \
        fprintf(stderr, COLORED(FAIL, "Assertion failed: line %d, file \"%s\"\n"), \
                __LINE__, __FILE__);                                               \
        fflush(stderr);                                                            \
        Abort();                                                                   \
    }

//----------------------------------------------------------------------
// ASSERT_MSG
//      If condition is false,  print a message and dump core.
//	Useful for documenting assumptions in the code.
//----------------------------------------------------------------------
#define ASSERT_MSG(condition, message)                                                               \
    if (!(condition))                                                                                \
    {                                                                                                \
        fprintf(stderr, COLORED(FAIL, "Assertion failed: due to \"%s\", at line %d, file \"%s\"\n"), \
                message, __LINE__, __FILE__);                                                        \
        fflush(stderr);                                                                              \
        Abort();                                                                                     \
    }

// My customized ANSI colour codes
// Foreground color
#define HEADER "\033[95m"
#define OKBLUE "\033[94m"
#define OKGREEN "\033[92m"
#define WARNING "\033[93m"
#define FAIL "\033[91m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
// Background color
#define HIGHLIGHT "\033[103m"
// Typographical Emphasis
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
// All in one
#define IMPORTANT "\033[1;90;103m"
// Other
#define INVERSE "\033[7m"
#define ENDC "\033[0m"

// Color the string using ANSI escape code.
#define COLORED(color, string) \
    color string ENDC

#endif // UTILITY_H
