// templatestack.cc
//	Routines to implement a LIFO stack of arbitrary things.
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
#include "templatestack.h"

//----------------------------------------------------------------------
// Stack<T>::Stack
// 	The constructor for the Stack class.  Note that it doesn't have a
// 	return type.
//
// 	"sz" -- maximum number of elements on the Stack at any time
//----------------------------------------------------------------------

template <class T>
Stack<T>::Stack(int sz) {

    ASSERT(sz >= 1);

    // Initialize the data members of the stack object.
    size = sz;
    top = 0;
    stack = new T[size];   // allocate an array of integers.
}

//----------------------------------------------------------------------
// Stack<T>::~Stack
// 	The destructor for the Stack class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

template <class T>
Stack<T>::~Stack() {

    delete [] stack;
}

//----------------------------------------------------------------------
// Stack<T>::Push
// 	Put a T on the top of the stack; error on overflow.
//
//	"value" -- the value to put on the stack
//----------------------------------------------------------------------


template <class T>
void
Stack<T>::Push(T value) {
    ASSERT(!Full());
    
    stack[top++] = value;
}

//----------------------------------------------------------------------
// Stack<T>::Pop
// 	Remove a T from the top of the stack, returning its value.
//	Error if the stack is empty.
//----------------------------------------------------------------------

template <class T>
T
Stack<T>::Pop() {

    ASSERT(!Empty());
    
    return (stack[--top]);
}

//----------------------------------------------------------------------
// Stack<T>::Full
// 	Return TRUE if the stack has no more room.
//----------------------------------------------------------------------

template <class T>
bool
Stack<T>::Full() {
    return (top == size);
}

//----------------------------------------------------------------------
// Stack<T>::Empty
// 	Return TRUE if the stack has nothing on it.
//----------------------------------------------------------------------

template <class T>
bool
Stack<T>::Empty() {
    return (top == 0);
}

//----------------------------------------------------------------------
// Stack<T>::SelfTest
// 	Test our stack implementation by pushing 10 T's onto the 
//	stack, and then print them as it pops them off.
//----------------------------------------------------------------------

template <class T>
void
Stack<T>::SelfTest(T start) {
    T count = start;
    
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
    Stack<int> *s1 = new Stack<int>(10);   
    Stack<char> *s2 = new Stack<char>(10);   

    cout << "Testing Stack<int>\n";
    s1->SelfTest(17);

    cout << "Testing Stack<char>\n";
    s2->SelfTest('a');

    delete s1;		   // always delete what you allocate
    delete s2;		   // always delete what you allocate
    return 0;
}
