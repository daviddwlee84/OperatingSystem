// stack.h
//	Data structures for a "stack" -- a Last-In-First-Out list of integers.
//
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef STACK_H		// to prevent recursive includes
#define STACK_H

#include "copyright.h"

// The following defines the Stack class.  The functions are
// implemented in the file stack.cc.
//
// The constructor (initializer) for the Stack is passed the number
// of elements (integers) in the stack.

class Stack {
  public:
    Stack(int sz);    // Constructor:  initialize variables, allocate space.
    ~Stack();         // Destructor:   deallocate space allocated above.
    
    void Push(int value); // Push an integer on the stack, checking for overflow
    int Pop();        // Pop an integer off the stack, checking for underflow.
    
    bool Full();       // Returns TRUE if the stack is full, FALSE otherwise.
    bool Empty();      // Returns TRUE if the stack is empty, FALSE otherwise.

    void SelfTest();	// Test whether the implementation works.
    
  private:
    int size;         // The maximum capacity of the stack.
    int top;          // Index of the next position to be used.
    int *stack;       // A pointer to an array that holds the contents.
};

#endif // STACK_H
