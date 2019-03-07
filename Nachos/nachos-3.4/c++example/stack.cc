// stack.cc
//	Routines to implement a LIFO stack of integers.
//	
//	The stack is represented as an array; we return an error
//	if the caller tries to push more things onto the stack than we have
//	room for.
//
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


extern "C" {
#include <assert.h>

#define ASSERT(expression)  assert(expression)
}

#include <iostream.h>
#include "copyright.h"
#include "stack.h"

//----------------------------------------------------------------------
// Stack::Stack
// 	The constructor for the Stack class.  Note that it doesn't have a
// 	return type.
//
// 	"sz" -- maximum number of elements on the Stack at any time
//----------------------------------------------------------------------

Stack::Stack(int sz) {

    ASSERT(sz >= 1);

    // Initialize the data members of the stack object.
    size = sz;
    top = 0;
    stack = new int[size];   // allocate an array of integers.
}

//----------------------------------------------------------------------
// Stack::~Stack
// 	The destructor for the Stack class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

Stack::~Stack() {

    delete [] stack;
}

//----------------------------------------------------------------------
// Stack::Push
// 	Put an integer on the top of the stack; error on overflow.
//
//	"value" -- the value to put on the stack
//----------------------------------------------------------------------

void
Stack::Push(int value) {
    ASSERT(!Full());
    
    stack[top++] = value;
}

//----------------------------------------------------------------------
// Stack::Pop
// 	Remove an integer from the top of the stack, returning its value.
//	Error if the stack is empty.
//----------------------------------------------------------------------

int
Stack::Pop() {

    ASSERT(!Empty());
    
    return (stack[--top]);
}

//----------------------------------------------------------------------
// Stack::Full
// 	Return TRUE if the stack has no more room.
//----------------------------------------------------------------------

bool
Stack::Full() {
    return (top == size);
}

//----------------------------------------------------------------------
// Stack::Empty
// 	Return TRUE if the stack has nothing on it.
//----------------------------------------------------------------------

bool
Stack::Empty() {
    return (top == 0);
}

//----------------------------------------------------------------------
// Stack::SelfTest
// 	Test our stack implementation by pushing 10 numbers onto the 
//	stack, and then print them as it pops them off.
//----------------------------------------------------------------------

void
Stack::SelfTest() {
    int count = 17;
    
    // Put a bunch of stuff in the stack...
    while (!Full()) {
	cout << "pushing " << count << "\n";
	Push(count++);
    }
    
    // ... and take it out again.
    while (!Empty()) {
	cout << "popping " << Pop() << "\n";
    }
}

//----------------------------------------------------------------------
// main
// 	Run the test code for the stack implementation.
//----------------------------------------------------------------------

int
main() {
    Stack *stack = new Stack(10);   // Constructor with an argument.

    stack->SelfTest();

    delete stack;		   // always delete what you allocate
    return 0;
}
