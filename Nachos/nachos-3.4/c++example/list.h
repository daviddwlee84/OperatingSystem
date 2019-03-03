// list.h 
//	Data structures to manage LISP-like lists.  
//
// Copyright (c) 1992,1993,1995 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef LIST_H
#define LIST_H

#include "copyright.h"

class ListElement;

// The following class defines a "list" -- a singly linked list of
// list elements, each of which contains an integer.

class List {
  public:
    List();			// initialize the list
    ~List();			// de-allocate the list

    void Prepend(int value); 	// Put item at the beginning of the list
    int Remove(); 	 	// Take item off the front of the list

    bool Empty();		// is the list empty? 

    void SelfTest();
    
  private:
    ListElement *first;  	// Head of the list, NULL if list is empty
    ListElement *last;		// Last element of list
};

#endif // LIST_H
