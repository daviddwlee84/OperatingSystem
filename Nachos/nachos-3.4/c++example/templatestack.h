// templatestack.h
//	Data structures for a stack" -- a Last-In-First-Out list --
//	of arbitrary things.
//
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef TEMPLATESTACK_H		// to prevent recursive includes
#define TEMPLATESTACK_H

#include "copyright.h"

// The following defines the Stack class.  The functions are
// implemented in the file templatestack.cc.
//
// T is the type of the thing we want to put on the stack.

template <class T> 
class Stack {
  public:
    Stack(int sz);    // Constructor
    ~Stack();         // Destructor
    
    void Push(T value); // Push a T on the stack
    T Pop();        	// Pop a T off the stack
    
    bool Full();       // Returns TRUE if the stack is full
    bool Empty();      // Returns TRUE if the stack is empty

    void SelfTest(T start); // Test whether the implementation works.
    
  private:
    int size;         // The maximum capacity of the stack.
    int top;          // Index of the next position to be used.
    T *stack;         // A pointer to an array that holds the contents.
};

#endif // TEMPLATESTACK_H
