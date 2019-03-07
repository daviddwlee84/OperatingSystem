// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, (void*)1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// Lab1Exercise3Thread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
Lab1Exercise3Thread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("*** thread %d (uid=%d, tid=%d) looped %d times\n", which, currentThread->getUserId(), currentThread->getThreadId(), num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// Lab1 Exercise3
// 	Create multi-threads and show its uid and tid
//----------------------------------------------------------------------

void
Lab1Exercise3()
{
    DEBUG('t', "Entering Lab1Exercise3");

    const int max_threads = 5;
    const int test_uid = 87;

    for (int i = 0; i < max_threads; i++) {
        // Generate a Thread object
        Thread *t = new Thread("forked thread");
        t->setUserId(test_uid); // set uid
        // Define a new thread's function and its parameter
        t->Fork(Lab1Exercise3Thread, (void*)t->getThreadId());
    }

    Lab1Exercise3Thread(0);
}

//----------------------------------------------------------------------
// Lab1 Exercise4-1
// 	Create many threads until reach the maximum MAX_THREAD_NUM
//----------------------------------------------------------------------

void
Lab1Exercise4_1()
{
    DEBUG('t', "Entering Lab1Exercise4_1");

    const int max_threads = 130;

    for (int i = 0; i < max_threads; i++) {
        // Generate a Thread object
        Thread *t = new Thread("forked thread");
        printf("*** thread name %s (tid=%d)\n", t->getName(), t->getThreadId());
    }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
        printf("Lab1 Exercise3:\n");
        Lab1Exercise3();
        break;
    case 3:
        printf("Lab1 Exercise4-1:\n");
        Lab1Exercise4_1();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

