// inheritstack.h
//	Data structures for a "stack" -- a Last-In-First-Out list of integers.
//
//	We define two separate implementations of stacks, to
//	illustrate C++ inheritance.
//
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef INHERITSTACK_H		// to prevent recursive includes
#define INHERITSTACK_H

#include "copyright.h"
#include "list.h"


// The following defines an "abstract" stack of integers.
// This class is abstract because no one is allowed to create
// instances of it; instead, you make instances of the derived
// classes that inherit from it.

class Stack {
  public:
    virtual ~Stack();   // Destructor
    
    virtual void Push(int value) = 0; // Push an integer on the stack
    virtual int Pop() = 0;      // Pop an integer off the stack
    
    virtual bool Full() = 0;    // Returns TRUE if the stack is full
    virtual bool Empty() = 0;   // Returns TRUE if the stack is empty

    void SelfTest(int numToPush); // Test whether the implementation works.
                        // Note that the test routine is shared among
			// all derived classes because it shouldn't 
			// matter to the test code which version we're using!

  protected:
    Stack();            // Constructor is protected to prevent anyone but 
			// derived classes from calling constructor.
};


// The following defines an implementation of Stack using arrays.
// This is the same as the original implementation in stack.h,
// except we don't need a SelfTest() because that's defined above by Stack!

class ArrayStack : public Stack {
  public:
    ArrayStack(int sz);    // Constructor:  initialize variables, allocate space.
    ~ArrayStack();         // Destructor:   deallocate space allocated above.
    
    void Push(int value); // Push an integer on the stack
    int Pop();        // Pop an integer off the stack
    
    bool Full();       // Returns TRUE if the stack is full
    bool Empty();      // Returns TRUE if the stack is empty

  private:
    int size;         // The maximum capacity of the stack.
    int top;          // Index of the next position to be used.
    int *stack;       // A pointer to an array that holds the contents.
};


// The following defines an implementation of Stack using lists.
//
// Note that a list implementation can't overflow, so we don't
// need to pass a maximum size into the constructor.

class ListStack : public Stack {
  public:
    ListStack();    // Constructor:  initialize variables, allocate space.
    ~ListStack();   // Destructor:   deallocate space allocated above.
    
    void Push(int value); // Push an integer on the stack
    int Pop();     // Pop an integer off the stack
    
    bool Full();   // Always return FALSE, this implementation never overflows
    bool Empty();  // Returns TRUE if the stack is empty

  private:
    List *stack;
};

#endif INHERITSTACK_H
