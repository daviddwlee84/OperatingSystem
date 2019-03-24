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
#include "synch.h" // Lab3 for Lock

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
    printf("*** current thread (uid=%d, tid=%d, pri=%d, name=%s) => ", currentThread->getUserId(), currentThread->getThreadId(), currentThread->getPriority(), currentThread->getName());
    IntStatus oldLevel; // for case 1 sleep (avoid cross initialization problem of switch case)
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
            // line 246, file "../threads/thread.cc"
            // Assert interrupt is off
            oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
            currentThread->Sleep();
            (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
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
    t2->Fork(CustomThreadFunc, (void*)1);
    t3->Fork(CustomThreadFunc, (void*)2);

    Thread *t4 = new Thread("fork 4");

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
// ThreadWithTicks
//  Re-enable the interrupt to invoke OnTick() make system time moving forward
//----------------------------------------------------------------------

void
ThreadWithTicks(int runningTime)
{
    int num;
    
    for (num = 0; num < runningTime * SystemTick; num++) {
        printf("*** thread with running time %d looped %d times (stats->totalTicks: %d)\n", runningTime, num+1, stats->totalTicks);
        interrupt->OneTick(); // make system time moving forward (advance simulated time)
        // Switch interrupt on and off to invoke OneTick() (not necessary...)
        // interrupt->SetLevel(IntOn);
        // interrupt->SetLevel(IntOff);
    }
    currentThread->Finish();
}

//----------------------------------------------------------------------
// Lab2 Challenge Round Robin
// 	Fork some Thread with different priority
//  and observe if the lower one will take over the CPU
//----------------------------------------------------------------------

void
Lab2ChallengeRR()
{
    DEBUG('t', "Entering Lab2ChallengeRR");

    printf("\nSystem initial ticks:\tsystem=%d, user=%d, total=%d\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    Thread *t1 = new Thread("7");
    Thread *t2 = new Thread("2");
    Thread *t3 = new Thread("5");

    printf("\nAfter new Thread ticks:\tsystem=%d, user=%d, total=%d\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    t1->Fork(ThreadWithTicks, (void*)7);
    t2->Fork(ThreadWithTicks, (void*)2);
    t3->Fork(ThreadWithTicks, (void*)5);

    printf("\nAfter 3 fork() ticks:\tsystem=%d, user=%d, total=%d\n\n", stats->systemTicks, stats->userTicks, stats->totalTicks);

    // update the lastSwitchTick
    // (according to previous test, it will start from 50)
    scheduler->lastSwitchTick = stats->totalTicks;
    currentThread->Yield(); // Yield the main thread
}

//----------------------------------------------------------------------
// Product
//  Product with value
//----------------------------------------------------------------------

typedef struct PRODUCT {
    int value;
} product;

//----------------------------------------------------------------------
// Bounded buffer
//  Condumer must wait if the buffer is empty,
//  and the producer must wait if the buffer is full
//  (no malloc in Nachos?! so use define)
//----------------------------------------------------------------------

#define BUFFER_SIZE 10

class buffer {
    public:
        buffer() {
            fillCount = new Semaphore("Fill Count", 0);
            emptyCount = new Semaphore("Empty Count", BUFFER_SIZE);
            buffer_mutex = new Lock("Buffer mutex");
            count = 0;
        };
        ~buffer() {
            delete list;
        }
        void putItemIntoBuffer(product* item) {
            emptyCount->P(); // down
            buffer_mutex->Acquire();
            
            /* Critical Section */
            list[count++] = *item;
            /********************/

            buffer_mutex->Release();
            fillCount->V(); // up
        };
        product* removeItemFromBuffer() {
            fillCount->P(); // down
            buffer_mutex->Acquire();

            /* Critical Section */
            product* item = &list[count-- -1];
            /********************/

            buffer_mutex->Release();
            emptyCount->V(); // up

            return item;
        };
        void printBuffer() {
            printf("Buffer: [", BUFFER_SIZE, count);
            int i;
            for (i = 0; i < count; i++) {
                printf("%d, ", list[i].value);
            }
            for (; i < BUFFER_SIZE; i++) {
                printf("__, ");
            }
            printf("]\n");
        }
    private:
        int count;
        Lock* buffer_mutex;
        Semaphore* fillCount;
        Semaphore* emptyCount;
        product list[BUFFER_SIZE];
} *shared_buffer;

//----------------------------------------------------------------------
// Produce Item
//  Generate prodoct with value
//----------------------------------------------------------------------

product*
produceItem(int value)
{
    printf("Producing item with value %d!!\n", value);
    product item;
    item.value = value;
    return &item;
}

//----------------------------------------------------------------------
// Consume Item
//  Delete product
//----------------------------------------------------------------------

void
consumeItem(product* item)
{
    printf("Consuming item with value %d!!\n", item->value);
    // delete item; // Use it when using linked list, or it will get error
}

//----------------------------------------------------------------------
// Producer
//  generate data, put it into the buffer, and start again. 
//----------------------------------------------------------------------

void
ProducerThread(int iterNum)
{
    for (int i = 0; i < iterNum; i++) {
        printf("## %s ##: ", currentThread->getName());
        product* item = produceItem(i);
        shared_buffer->putItemIntoBuffer(item);
        // shared_buffer->printBuffer();

        interrupt->OneTick();
    }
}

//----------------------------------------------------------------------
// Consumer
//  consuming the data, one piece at a time.
//----------------------------------------------------------------------

void
ConsumerThread(int iterNum)
{
    for (int i = 0; i < iterNum; i++) {
        printf("$$ %s $$: ", currentThread->getName());
        product* item = shared_buffer->removeItemFromBuffer();
        // shared_buffer->printBuffer();
        consumeItem(item);

        interrupt->OneTick();
    }
}

//----------------------------------------------------------------------
// Lab3 Exercise4 Producer-consumer problem (Bounded-buffer problem)
//  The problem describes two processes, the producer and the consumer,
//  who share a common, fixed-size buffer used as a queue.
//  The producer's job is to generate data, put it into the buffer,
//  and start again. 
//  At the same time, the consumer is consuming the data
//  (i.e., removing it from the buffer), one piece at a time.
//  The problem is to make sure that the producer won't try to add data
//  into the buffer if it's full and that the consumer won't try to
//  remove data from an empty buffer.
//----------------------------------------------------------------------

void
Lab3ProducerConsumer()
{
    DEBUG('t', "Entering Lab3ProducerConsumer");

    shared_buffer = new buffer();

    Thread *producer1 = new Thread("Producer 1");
    Thread *producer2 = new Thread("Producer 2");
    Thread *consumer1 = new Thread("Consumer 1");
    Thread *consumer2 = new Thread("Consumer 2");

    producer1->Fork(ProducerThread, (void*)8);
    consumer1->Fork(ConsumerThread, (void*)6);
    consumer2->Fork(ConsumerThread, (void*)9);
    producer2->Fork(ProducerThread, (void*)7);

    currentThread->Yield(); // Yield the main thread
}

//----------------------------------------------------------------------
// Shared Data for Barrier
//----------------------------------------------------------------------

#define ROWS 24
#define COLS 10
#define N_THREAD 4 // Threads
#define N_ROWS (ROWS/N_THREAD) // Threads calculate rows
double matrix[ROWS][COLS];

Barrier* barrier;

//----------------------------------------------------------------------
// Calculation Thread
//  Do the calculation for each Thread
//----------------------------------------------------------------------

void
CalcThread(int startRow)
{
    int x, y;

    // Determine calculate region
    int endRow = startRow + N_ROWS;

    // First calculation
    for (x = startRow; x < endRow; x++) for (y = 0; y < COLS; y++) {
        matrix[x][y] += (x+1)*(y+1);
    }

    printf("Thread \"%s\" finish First Calculation\n", currentThread->getName());

    /********** Barrier **********/
    barrier->ArrivedAndWait();
    /*****************************/

    // Second calculation
    for (x = startRow; x < endRow; x++) for (y = 0; y < COLS; y++) {
        matrix[x][y] /= startRow+1;
    }

    printf("Thread \"%s\" finish Second Calculation\n", currentThread->getName());

    /********** Barrier **********/
    barrier->ArrivedAndWait();
    /*****************************/

    // Third calculation
    for (x = startRow; x < endRow; x++) for (y = 0; y < COLS; y++) {
        matrix[x][y] -= startRow/N_ROWS;
    }

    printf("Thread \"%s\" finish Third Calculation\n", currentThread->getName());
}

//----------------------------------------------------------------------
// Lab3 Challenge1 Barrier
//  1. Threads do first calculation (use and change values in data)
//  2. Barrier! Wait for all threads to finish first calculation before continuing
//  3. Threads do second calculation (use and change values in data)
//  4. Barrier! Wait for all threads to finish second calculation before continuing
//  5. Threads do third calculation (use and change values in data)
//
//  Shared memory: matrix[24][10]; 4 Threads. Each thread manipulate 6 rows
//----------------------------------------------------------------------

void
Lab3Barrier()
{
    Thread* calcThreads[N_THREAD];
    barrier = new Barrier("Matrix Calc", N_THREAD+1); // +1 for main thread

    // Creating threads
    for (int i = 0; i < N_THREAD; i++) {
        char ThreadName[10];
        sprintf(ThreadName, "Calc %d", i+1); // Used to transfer int -> string
        // Create threads with priority grater than main
        Thread *calc = new Thread(strdup(ThreadName)); // Duplicate string or they shared pointer
        calcThreads[i] = calc;
    }

    // Fork threads
    int startRow = 0;
    for (int i = 0; i < N_THREAD; i++) {
        calcThreads[i]->Fork(CalcThread, (void*)startRow);
        startRow += N_ROWS;
    }

    // Initialization
    int row, col;
    for (row = 0; row < ROWS; row++ ) {
        for (col = 0; col < COLS; col++) {
            matrix[row][col] = 0;
        }
    }

    printf("main() is ready.\n");

    currentThread->Yield(); // Yield the main thread
    // Everybody doing First calculation

    barrier->ArrivedAndWait();
    // main will wake everybody up when everybody is reaching the barrier

    printf("main() is going!\n");

    currentThread->Yield(); // Yield the main thread
    // Everybody doing Second calculation

    barrier->ArrivedAndWait();

    printf("main() is going again!\n");

    currentThread->Yield(); // Yield the main thread
    // Everybody doing Third calculation 

    // Back to main and print the result
    printf("Result of data:\n");
    for (row = 0; row < ROWS; row++ ) {
        for (col = 0; col < COLS; col++) {
            printf("%5.2lf ", matrix[row][col]);
        }
        printf("\n");
    }
}

//----------------------------------------------------------------------
// Shared Data for Reader-Writer
//  A quote of Albert Einstein
//----------------------------------------------------------------------

#define SENTENCE_LENGTH 13
#define MAX_CHAR 10
const char AlbertEinstein[SENTENCE_LENGTH][MAX_CHAR] = {
    "Insanity:", "doing", "the", "same", "thing", "over", "and", "over", "again",
    "and", "expecting", "different", "results."
};
int words; // How many words does writer writes.
char quote[SENTENCE_LENGTH][MAX_CHAR];

ReaderWriterLock* RWLock;

//----------------------------------------------------------------------
// Writer Thread
//  Trying hard to write the quote...
//----------------------------------------------------------------------

void
WriterThread(int dummy)
{
    while (words < SENTENCE_LENGTH) {
        RWLock->WriterAcquire();

        strcpy(quote[words], AlbertEinstein[words]); // composing...
        words++;

        printf("Writer is writting: ");
        for (int i = 0; i < words; i++) {
            printf("%s ", quote[i]);
        }
        printf("\n");

        RWLock->WriterRelease();
    }
}

//----------------------------------------------------------------------
// Reader Thread
//  Keeping trying to read the quote. Until writer has finished the
//  quote. (i.e. the words is match the SENTENCE_LENGTH)
//----------------------------------------------------------------------

void
ReaderThread(int reader_id)
{
    do {
        RWLock->ReaderAcquire();

        printf("Reader %d is reading: ", reader_id);
        for (int i = 0; i < words; i++) {
            printf("%s ", quote[i]);
        }
        printf("\n");

        RWLock->ReaderRelease();
    } while (words < SENTENCE_LENGTH);
}

//----------------------------------------------------------------------
// Lab3 Challenge2 Reader-writer
//  Create multiple reader and a writer.
//
//  Shared data: quote[][]
//----------------------------------------------------------------------

void
Lab3ReaderWriter()
{
    // Change the reader amount to whatever you like
    // but not too many if you use random context switch
    // and you/(the writer) are/(is) not lucky...
    const int total_reader = 2;

    // Creating writer
    Thread* writer = new Thread("Writer");
    writer->Fork(WriterThread, (void*)0);

    // Creating readers
    for (int i = 0; i < total_reader; i++) {
        char readerName[10];
        sprintf(readerName, "Reader %d", i+1); // Used to transfer int -> string
        // Create threads with priority grater than main
        Thread* reader = new Thread(strdup(readerName)); // Duplicate string or they shared pointer
        reader->Fork(ReaderThread, (void*)i+1);
    }

    RWLock = new ReaderWriterLock("RWLock");

    currentThread->Yield(); // Yield the main thread
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    // Lab3 Data structure test
    // buffer* test_buffer = new buffer();

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
    case 7:
        printf("Lab2 Challenge RR:\n");
        printf("(don't forget to add `-rr` argument to activate timer)\n");
        Lab2ChallengeRR();
        break;
    case 8:
        printf("Lab3 Exercise4: Producer-consumer problem (Bounded-buffer problem)\n");
        printf("(add `-d c -rs` argument to show \"Context Switch\" and activate random timer)\n");
        Lab3ProducerConsumer();

        /* Lab3 Data Structure Test

        // Test PRODUCT struct
        product item;
        item.value = 87;
        struct PRODUCT *new_item;
        new_item = &item;
        printf("%d\n\n", new_item->value);

        // Test buffer class
        item.value = 78;
        // buffer* test_buffer = new buffer();
        test_buffer->putItemIntoBuffer(&item);
        test_buffer->printBuffer();
        new_item = test_buffer->removeItemFromBuffer();
        printf("%d\n\n", new_item->value);

        */
        break;
    case 9:
        printf("Lab3 Challenge1: Barrier\n");
        printf("(add `-d b` to show Barrier related debugging messabe)\n");
        Lab3Barrier();
        break;
    case 10:
        printf("Lab3 Challenge2: Reader-Writer\n");
        printf("(add `-d wc -rs` argument to show \"Context Switch\", RWLock message and activate random timer)\n");
        Lab3ReaderWriter();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

