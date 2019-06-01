// main.cc 
//	Bootstrap code to initialize the operating system kernel.
//
//	Allows direct calls into internal operating system functions,
//	to simplify debugging and testing.  In practice, the
//	bootstrap code would just initialize data structures,
//	and start a user program to print the login prompt.
//
// 	Most of this file is not needed until later assignments.
//
// Usage: nachos -d <debugflags> -rs <random seed #>
//		-s -x <nachos file> -c <consoleIn> <consoleOut>
//		-f -cp <unix file> <nachos file>
//		-p <nachos file> -r <nachos file> -l -D -t
//              -n <network reliability> -m <machine id>
//              -o <other machine id>
//              -z
//
//    -d causes certain debugging messages to be printed (cf. utility.h)
//    -rs causes Yield to occur at random (but repeatable) spots
//    -z prints the copyright message
//
//  USER_PROGRAM
//    -s causes user programs to be executed in single-step mode
//    -x runs a user program
//    -c tests the console
//
//  FILESYS
//    -f causes the physical disk to be formatted
//    -cp copies a file from UNIX to Nachos
//    -p prints a Nachos file to stdout
//    -r removes a Nachos file from the file system
//    -l lists the contents of the Nachos directory
//    -D prints the contents of the entire file system 
//    -t tests the performance of the Nachos file system
//
//  NETWORK
//    -n sets the network reliability
//    -m sets this machine's host id (needed for the network)
//    -o runs a simple test of the Nachos network software
//
//  NOTE -- flags are ignored until the relevant assignment.
//  Some of the flags are interpreted here; some in system.cc.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#define MAIN
#include "copyright.h"
#undef MAIN

#include "utility.h"
#include "system.h"

#ifdef THREADS
extern int testnum;
#endif

// External functions used by this file

extern void ThreadTest(void), Copy(char *unixFile, char *nachosFile);
extern void Print(char *file), PerformanceTest(void);
extern void StartProcess(char *file), ConsoleTest(char *in, char *out);
extern void SynchConsoleTest(char *in, char *out); // Lab5: Synchronous console
extern void StartTwoThread(char *filename); // Lab4: Multi-thread test
extern void MailTest(int networkID);
#ifdef MULTI_LEVEL_DIR
extern void MakeDir(char *dirname); // Lab5: Multi-level directory
#endif

// External variable

bool VERBOSE = TRUE; // print message when machine Halt in machine/interrupt.cc

//----------------------------------------------------------------------
// main
// 	Bootstrap the operating system kernel.  
//	
//	Check command line arguments
//	Initialize data structures
//	(optionally) Call test procedure
//
//	"argc" is the number of command line arguments (including the name
//		of the command) -- ex: "nachos -d +" -> argc = 3 
//	"argv" is an array of strings, one for each command line argument
//		ex: "nachos -d +" -> argv = {"nachos", "-d", "+"}
//----------------------------------------------------------------------

int
main(int argc, char **argv)
{
    int argCount;			// the number of arguments 
					// for a particular command

    DEBUG('t', "Entering main");
    (void) Initialize(argc, argv);

// If define THREADS it will consume all the input when we want to test the file system...
#if THREADS && !TEST_FILESYS
    for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount) {
        argCount = 1;
        switch (argv[0][1]) {
        case 'q':
            testnum = atoi(argv[1]);
            argCount++;
            break;
        default:
            testnum = 1;
            break;
        }
    }

    ThreadTest();
#endif

    for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount) {
	    argCount = 1;
        if (!strcmp(*argv, "-z"))               // print copyright
            printf (copyright);
        if (!strcmp(*argv, "-Q")) // be quiet (disable dummy machine message)
            VERBOSE = FALSE;
#ifdef USER_PROGRAM
        if (!strcmp(*argv, "-x")) {        	// run a user program
	    ASSERT(argc > 1);
            StartProcess(*(argv + 1));
            argCount = 2;
        } else if (!strcmp(*argv, "-X")) {      // Lab4: run two user program (multi-thread)
	    ASSERT(argc > 1);
            StartTwoThread(*(argv + 1));
            argCount = 2;
        } else if (!strcmp(*argv, "-c")) { // test the console
            if (argc == 1) {
                ConsoleTest(NULL, NULL);
            } else {
                ASSERT(argc > 2);
                ConsoleTest(*(argv + 1), *(argv + 2));
                argCount = 3;
            }
            interrupt->Halt(); // once we start the console, then
                               // Nachos will loop forever waiting
                               // for console input
        } else if (!strcmp(*argv, "-sc")) { // test the synchronous console
            if (argc == 1) {
                SynchConsoleTest(NULL, NULL);
            } else {
                ASSERT(argc > 2);
                SynchConsoleTest(*(argv + 1), *(argv + 2));
                argCount = 3;
            }
            interrupt->Halt(); // once we start the console, then
                               // Nachos will loop forever waiting
                               // for console input
        }
#endif // USER_PROGRAM
#ifdef FILESYS
        if (!strcmp(*argv, "-cp")) { // copy from UNIX to Nachos
            ASSERT(argc > 2);
            Copy(*(argv + 1), *(argv + 2));
            argCount = 3;
        } else if (!strcmp(*argv, "-p")) { // print a Nachos file
            ASSERT(argc > 1);
            Print(*(argv + 1));
            argCount = 2;
        } else if (!strcmp(*argv, "-r")) { // remove Nachos file (i.e. rm in UNIX)
            ASSERT(argc > 1);
            bool success = fileSystem->Remove(*(argv + 1));
            ASSERT_MSG(success, "Remove file fail!");
            argCount = 2;
        } else if (!strcmp(*argv, "-l")) { // list Nachos root directory
            fileSystem->List();
        } else if (!strcmp(*argv, "-D")) { // print entire filesystem
            fileSystem->Print();
        } else if (!strcmp(*argv, "-t")) { // performance test
            PerformanceTest();
        }
#ifdef MULTI_LEVEL_DIR
        // Lab5: Directory Operations
        else if (!strcmp(*argv, "-mkdir")) { // make directory
            ASSERT(argc > 1);
            MakeDir(*(argv + 1));
            argCount = 2;
        } else if (!strcmp(*argv, "-rd")) { // remove Nachos file or directory recursively (i.e. rm -r in UNIX)
            ASSERT(argc > 1);
            bool success = fileSystem->RemoveDir(*(argv + 1));
            ASSERT_MSG(success, "Remove directory fail!");
            argCount = 2;
        } else if (!strcmp(*argv, "-ld")) { // list Nachos directory
            ASSERT(argc > 1);
            fileSystem->ListDir(*(argv + 1));
            argCount = 2;
        }
#endif // MULTI_LEVEL_DIR
#endif // FILESYS
#ifdef NETWORK
        if (!strcmp(*argv, "-o")) {
	    ASSERT(argc > 1);
            Delay(2); 				// delay for 2 seconds
						// to give the user time to 
						// start up another nachos
            MailTest(atoi(*(argv + 1)));
            argCount = 2;
        }
#endif // NETWORK
    }

    currentThread->Finish();	// NOTE: if the procedure "main" 
				// returns, then the program "nachos"
				// will exit (as any other normal program
				// would).  But there may be other
				// threads on the ready list.  We switch
				// to those threads by saying that the
				// "main" thread is finished, preventing
				// it from returning.
    return(0);			// Not reached...
}
