// inheritstack.cc
//	Routines for two implementions of a LIFO stack of integers, 
//	one as an array, the other as a list.
//	
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


extern "C" {
#include <assert.h>

#define ASSERT(expression)  assert(expression)
}

const bool FALSE = false;
const bool TRUE = true;

#include <iostream.h>
#include "copyright.h"
#include "list.h"
#include "inheritstack.h"

//----------------------------------------------------------------------
// Stack::Stack, Stack::~Stack
// 	constructor and destructor for the Stack class; no data
//	to initialize!
//----------------------------------------------------------------------

Stack::Stack() {}
Stack::~Stack() {}


// IMPLEMENTATION #1: AS AN ARRAY

//----------------------------------------------------------------------
// ArrayStack::ArrayStack
// 	The constructor for the ArrayStack class.
//
// 	"sz" -- maximum number of elements on the ArrayStack at any time
//----------------------------------------------------------------------

ArrayStack::ArrayStack(int sz) : Stack() {

    ASSERT(sz >= 1);

    // Initialize the data members of the stack object.
    size = sz;
    top = 0;
    stack = new int[size];   // allocate an array of integers.
}

//----------------------------------------------------------------------
// ArrayStack::~ArrayStack
// 	The destructor for the ArrayStack class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

ArrayStack::~ArrayStack() {

    delete [] stack;
}

//----------------------------------------------------------------------
// ArrayStack::Push
// 	Put an integer on the top of the stack; error on overflow.
//
//	"value" -- the value to put on the stack
//----------------------------------------------------------------------

void
ArrayStack::Push(int value) {
    ASSERT(!Full());
    
    stack[top++] = value;
}

//----------------------------------------------------------------------
// ArrayStack::Pop
// 	Remove an integer from the top of the stack, returning its value.
//	Error if the stack is empty.
//----------------------------------------------------------------------

int
ArrayStack::Pop() {

    ASSERT(!Empty());
    
    return (stack[--top]);
}

//----------------------------------------------------------------------
// ArrayStack::Full
// 	Return TRUE if the stack has no more room.
//----------------------------------------------------------------------

bool
ArrayStack::Full() {
    return (top == size);
}

//----------------------------------------------------------------------
// ArrayStack::Empty
// 	Return TRUE if the stack has nothing on it.
//----------------------------------------------------------------------

bool
ArrayStack::Empty() {
    return (top == 0);
}


// IMPLEMENTATION #2: AS A LIST

//----------------------------------------------------------------------
// ListStack::ListStack
// 	The constructor for the ListStack class.
//----------------------------------------------------------------------

ListStack::ListStack() : Stack() {

    stack = new List;	 // allocate an empty list of integers.
}

//----------------------------------------------------------------------
// ListStack::~ListStack
// 	The destructor for the ListStack class.  Just get rid of the list we
// 	allocated in the constructor.
//----------------------------------------------------------------------

ListStack::~ListStack() {

    delete stack;
}

//----------------------------------------------------------------------
// ListStack::Push
// 	Put an integer on the top of the stack.
//
//	"value" -- the value to put on the stack
//----------------------------------------------------------------------

void
ListStack::Push(int value) {
    stack->Prepend(value);
}

//----------------------------------------------------------------------
// ListStack::Pop
// 	Remove an integer from the top of the stack, returning its value.
//	Error if the stack is empty.
//----------------------------------------------------------------------

int
ListStack::Pop() {

    ASSERT(!Empty());
    
    return stack->Remove();
}

//----------------------------------------------------------------------
// ListStack::Full
// 	Return FALSE, because a liststack can never overflow
//----------------------------------------------------------------------

bool
ListStack::Full() {
    return FALSE;
}

//----------------------------------------------------------------------
// ListStack::Empty
// 	Return TRUE if the stack has nothing on it.
//----------------------------------------------------------------------

bool
ListStack::Empty() {
    return stack->Empty();
}

//----------------------------------------------------------------------
// Stack::SelfTest
// 	Test our stack implementation by pushing 10 numbers onto the 
//	stack, and then print them as it pops them off.
//
//	Note this code is generic between the two versions --
//	it doesn't matter whether this is an ArrayStack or a ListStack!
//
//	"numToPush" is the number of items to put on the stack in the 
//	selftest.
//----------------------------------------------------------------------

void
Stack::SelfTest(int numToPush) {
    int count = 17;
    
    // Put a bunch of stuff in the stack...
    for (int i = 0; i < numToPush; i++) {
	ASSERT(!Full());
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
    Stack *s1 = new ArrayStack(10);   // Constructor with an argument.
    Stack *s2 = new ListStack(); 

    cout << "Testing ArrayStack\n";
    s1->SelfTest(10);

    cout << "Testing ListStack\n";
    s2->SelfTest(10);

    delete s1;		   // always delete what you allocate
    delete s2;		   // always delete what you allocate
    return 0;
}
