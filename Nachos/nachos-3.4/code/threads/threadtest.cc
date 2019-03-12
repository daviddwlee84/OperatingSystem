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
// TS command (Lab 1) (extend in Lab 2 for priority)
// 	Showing current threads' status (like ps in Linux)
//----------------------------------------------------------------------

void
TS()
{
    DEBUG('t', "Entering TS");

    const char* TStoString[] = {"JUST_CREATED", "RUNNING", "READY", "BLOCKED"};

    printf("UID\tTID\tNAME\tPRI\tSTATUS\n");
    for (int i = 0; i < MAX_THREAD_NUM; i++) { // check pid flag
        if (tid_flag[i]) {
            printf("%d\t%d\t%s\t%d\t%s\n", tid_pointer[i]->getUserId(), tid_pointer[i]->getThreadId(), tid_pointer[i]->getName(), tid_pointer[i]->getPriority(), TStoString[tid_pointer[i]->getThreadStatus()]);
        }
    }
}

//----------------------------------------------------------------------
// CustomThreadFunc
//
//	"which" is simply a number identifying the operation to do on current thread
//----------------------------------------------------------------------

void
CustomThreadFunc(int which)
{
    printf("*** current thread (uid=%d, tid=%d, pri=%d name=%s) => ", currentThread->getUserId(), currentThread->getThreadId(), currentThread->getPriority(), currentThread->getName());
    switch (which)
    {
        case 0:
            printf("Yield\n");
            scheduler->Print();
            printf("\n\n");
            currentThread->Yield();
            break;
        case 1:
            printf("Sleep\n");
            currentThread->Sleep();
            break;
        case 2:
            printf("Finish\n");
            currentThread->Finish();
            break;
        default:
            printf("Yield (default)\n");
            currentThread->Yield();
            break;
    }
}

//----------------------------------------------------------------------
// Lab1 Exercise4-2
// 	Create some threads and use TS to show the status
//----------------------------------------------------------------------

void
Lab1Exercise4_2()
{
    DEBUG('t', "Entering Lab1Exercise4_2");

    Thread *t1 = new Thread("fork 1");
    Thread *t2 = new Thread("fork 2");
    Thread *t3 = new Thread("fork 3");

    t1->Fork(CustomThreadFunc, (void*)0);
    // t2->Fork(CustomThreadFunc, (void*)1);
    t3->Fork(CustomThreadFunc, (void*)2);

    Thread *t4 = new Thread("fork 4");
    t4->Fork(CustomThreadFunc, (void*)0);

    CustomThreadFunc(0); // Yield the current thread (i.e. main which is defined in system.cc)

    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n");
}

//----------------------------------------------------------------------
// Lab2 Exercise3-1
// 	Fork some Thread with different ways to initial the priority
//----------------------------------------------------------------------

void
Lab2Exercise3_1()
{
    DEBUG('t', "Entering Lab2Exercise3_1");

    Thread *t1 = new Thread("with p", 87);

    Thread *t2 = new Thread("set p");
    t2->setPriority(100);

    Thread *t3 = new Thread("no p");

    t1->Fork(CustomThreadFunc, (void*)0);
    t2->Fork(CustomThreadFunc, (void*)0);
    t3->Fork(CustomThreadFunc, (void*)0);

    CustomThreadFunc(0); // Yield the current thread

    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n");
}

//----------------------------------------------------------------------
// Lab2 Exercise3-2
// 	Fork some Thread with different priority
//  and observe if the lower one will take over the CPU
//----------------------------------------------------------------------

void
Lab2Exercise3_2()
{
    DEBUG('t', "Entering Lab2Exercise3_2");

    Thread *t1 = new Thread("lower", 78);
    Thread *t2 = new Thread("highest", 87);
    // The lowest one will be put in front of the list
    // due to SortedInsert() in list.cc
    Thread *t3 = new Thread("lowest", 38);

    t1->Fork(CustomThreadFunc, (void*)0);
    t2->Fork(CustomThreadFunc, (void*)0);
    t3->Fork(CustomThreadFunc, (void*)0);

    CustomThreadFunc(0); // Yield the current thread

    // Because the main() Thread has priority 0
    // Then any process yield will make main keep running
    // Since 0 is the lowest number and it will be in front of the readyList
    // So the TS command will be called right after the first Yield()
    printf("--- Calling TS command ---\n");
    TS();
    printf("--- End of TS command ---\n\n");
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
    case 4:
        printf("Lab1 Exercise4-2:\n");
        Lab1Exercise4_2();
        break;
    case 5:
        printf("Lab2 Exercise3-1:\n");
        Lab2Exercise3_1();
        break;
    case 6:
        printf("Lab2 Exercise3-2:\n");
        Lab2Exercise3_2();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

