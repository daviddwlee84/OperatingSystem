// list.cc 
//     	Routines to manage a singly-linked list of integers. 
//
// 	A "ListElement" is allocated for each item to be put on the
//	list; it is de-allocated when the item is removed. This means
//      we don't need to keep a "next" pointer in every object we
//      want to put on a list.
// 
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


extern "C" {
#include <assert.h>

#define ASSERT(expression)  assert(expression)
}

#include "copyright.h"
#include "list.h"

const int NULL = 0;


// The following class defines a "list element" -- which is
// used to keep track of one item on a list.  It is equivalent to a
// LISP cell, with a "car" ("next") pointing to the next element on the list,
// and a "cdr" ("item") containing the item on the list.
//
// Class defined in list.cc, because only the List class can be allocating
// and accessing ListElements.

class ListElement {
   public:
     ListElement(int value) { item = value; next = NULL;};
     					// constructor for list element

     ListElement *next;		// next element on list, 
				// NULL if this is the last
     int item; 	    	        // value of this element
};



//----------------------------------------------------------------------
// List::List
//	Initialize a list, empty to start with.
//	Elements can now be added to the list.
//----------------------------------------------------------------------

List::List() { 

    first = last = NULL; 
}


//----------------------------------------------------------------------
// List::~List
//	Prepare a list for deallocation.  If the list still contains any 
//	ListElements, de-allocate them.
//----------------------------------------------------------------------

List::~List() { 
   
    while (!Empty())
	(void) Remove();	 // delete all the list elements
}


//----------------------------------------------------------------------
// List::Prepend
//      Put an integer on the front of the list.
//      
//	Allocate a ListElement to keep track of the integer.
//      If the list is empty, then this will be the only element.
//	Otherwise, put it at the beginning.
//
//	"value" is the integer to be put on the list.
//----------------------------------------------------------------------

void
List::Prepend(int value) {
    ListElement *element = new ListElement(value);

    if (Empty()) {		// list is empty
	first = element;
	last = element;
    } else {			// else put it before first
	element->next = first;
	first = element;
    }
}

//----------------------------------------------------------------------
// List::Remove
//      Remove the first integer from the front of the list.
//	Error if nothing on the list.
// 
// Returns:
//	The removed integer.
//----------------------------------------------------------------------

int
List::Remove() {
    ListElement *element = first;
    int value;

    ASSERT(!Empty());

    element = first;
    value = first->item;

    if (first == last) {	// list had one item, now has none 
        first = NULL;
	last = NULL;
    } else {
        first = element->next;
    }

    delete element;		// deallocate list element -- no longer needed
    return value;
}

//----------------------------------------------------------------------
// List::Empty
//      Returns TRUE if the list is empty (has no items).
//----------------------------------------------------------------------

bool
List::Empty() { 
    return (first == NULL);
}
