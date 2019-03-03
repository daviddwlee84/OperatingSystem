// elevator.cc
//	Routines to emulate a bank of elevators.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "elevator.h"
#include "system.h"

static void ElevatorDone(int arg) { ((ElevatorBank*)arg)->HandleInterrupt(); }

// information about an elevator event that affects riders or
// controllers.  Class is private to this module
class PendingElevatorEvent {
  public:
    PendingElevatorEvent(ElevatorEvent ev, int fl, int el) {
    	event = ev; floor = fl; elevator = el;
    }
    ElevatorEvent event;	// the pending event
    int floor;			// which floor it referenced
    int elevator;		// which elevator it referenced
};

// data structure to represent the state of the physical elevator
// class is private to this module
class ElevatorInfo {
  public:
    ElevatorInfo() { 		// initialize elevator state
    	display = Neither; doorsOpen = inMotion = FALSE;
	lastFloor = goingTo = 0; willArrive = 0;
	riders = new List();
    }
    ~ElevatorInfo() { delete riders; }
    
    int OpenDoors() {  // open the elevator doors, & return where we are
    	ASSERT(!doorsOpen && !inMotion);
   	doorsOpen = TRUE;
	return lastFloor;
    }
    void CloseDoors() { ASSERT(doorsOpen); doorsOpen = FALSE; }
    
    // set the elevator in motion, and return false if it was already moving
    // make sure that the doors are closed and that we're not moving
    // to the current floor!
    bool MoveTo(int goingToFloor) {
    	ASSERT(!doorsOpen && (inMotion || lastFloor != goingToFloor));
    	goingTo = goingToFloor;
   	if (!inMotion) {	
	    willArrive = stats->totalTicks + DelayPerFloor;
	    inMotion = TRUE;
	    return TRUE;
    	}
	return FALSE;
    }
    
    bool Enter(int onFloor) { // rider enters the elevator
	if (!doorsOpen || lastFloor != onFloor) {
	    return FALSE;
        } else {
            ASSERT(!inMotion);
    	    ASSERT(riders->NumInList() < MaxRiders);
    	    riders->Append(currentThread);
	    return TRUE;
        }
    }
    bool Exit(int onFloor) {	// rider leaves the elevator
	if (!doorsOpen || lastFloor != onFloor) {
	    return FALSE;
        } else {
            ASSERT(!inMotion);
    	    riders->Remove(currentThread);
	    return TRUE;
        }
    }

    bool ReachedNextFloor() {	// has the elevator reached the next floor?
        if (inMotion && (stats->totalTicks >= willArrive)) {
	   lastFloor += ((goingTo > lastFloor) ? 1 : -1);
	   return TRUE;
        }
        return FALSE;
    }
    bool CheckArrived() {	// has it reached the destination?
	if (inMotion && lastFloor == goingTo) {
	    inMotion = FALSE;
	    return TRUE;
   	}
   	return FALSE;
    }
    
    Direction display;		// rider visible display (up or down)
    bool doorsOpen;		// are the doors open?
    bool inMotion;		// is the elevator moving?
    int lastFloor;		// last floor the elevator was on
    int willArrive;		// when will it arrive at the next floor?
    int goingTo;		// where is the elevator going (if anywhere)
    List *riders;	// who is on board?
};

//----------------------------------------------------------------------
// ElevatorBank::ElevatorBank
// 	Initialize the elevator hardware, calling "riders" and
//	"controllers" to notify the rider/controller threads
// 	that an event has occurred that they should be interested 
//	in (such as, elevator arrives, doors open, etc)
//----------------------------------------------------------------------

ElevatorBank::ElevatorBank(int numElvtr, int numFlr, 
    			VoidFunctionPtr riders, int ridersArg, VoidFunctionPtr controllers, int controllersArg)
{
    numElevators = numElvtr;
    numFloors = numFlr;
    handlerRiders = riders;
    argRiders = ridersArg;
    handlerControllers = controllers;
    argControllers = controllersArg;
    riderEvents = new ListOfEvents;
    controllerEvents = new ListOfEvents;
    elevators = new ElevatorInfo *[numElevators];
    for (int i = 0; i < numElevators; i++) {
        elevators[i] = new ElevatorInfo();
    }
}

//----------------------------------------------------------------------
// ElevatorBank::~ElevatorBank
// 	Deallocate the elevator hardware.
//----------------------------------------------------------------------

ElevatorBank::~ElevatorBank() {
    delete riderEvents;
    delete controllerEvents;
    for (int i = 0; i < numElevators; i++) {
        delete elevators[i];
    }
    delete elevators;
}
        
//----------------------------------------------------------------------
// ElevatorBank::OpenDoors
// 	Open an elevator's doors; the elevator must not be
//	in motion!
//----------------------------------------------------------------------

void
ElevatorBank::OpenDoors(int elevator)
{
    ASSERT(elevator >= 0 && elevator < numElevators);
    PostEvent(riderEvents, DoorsOpened, 
    		elevators[elevator]->OpenDoors(), elevator, FALSE);
}
    
//----------------------------------------------------------------------
// ElevatorBank::CloseDoors
// 	Close an elevator's doors.
//----------------------------------------------------------------------

void 
ElevatorBank::CloseDoors(int elevator) {
    ASSERT(elevator >= 0 && elevator < numElevators);
    elevators[elevator]->CloseDoors();
}

//----------------------------------------------------------------------
// ElevatorBank::MoveTo
// 	Set the elevator into motion to "goingToFloor". 
//	Elevator doors must be closed. If elevator is already 
//	in motion, then if the new destination is ahead, keep going
//	but stop at the new spot.  If the new destination is behind us,
//	wait until we get to the next even floor boundary, then change
//	direction.
//----------------------------------------------------------------------

void 
ElevatorBank::MoveTo(int goingToFloor, int elevator)
{
    ASSERT(elevator >= 0 && elevator < numElevators);
    ASSERT(goingToFloor >= 0 && goingToFloor < numFloors);
    if (elevators[elevator]->MoveTo(goingToFloor)) {
    	interrupt->Schedule(ElevatorDone, (int)this, DelayPerFloor, ElevatorInt);
    }
}

//----------------------------------------------------------------------
// ElevatorBank::WhereIsElevator
//	Return the current position of the elevator (which floor its on).
//	If elevator is in motion, return the last floor we've gone by.
//----------------------------------------------------------------------

int 
ElevatorBank::WhereIsElevator(int elevator) 
{
    ASSERT(elevator >= 0 && elevator < numElevators);
    return elevators[elevator]->lastFloor;
}

//----------------------------------------------------------------------
// ElevatorBank::MarkDirection
//	Change the display showing where this elevator is headed.
//----------------------------------------------------------------------

void 
ElevatorBank::MarkDirection(int elevator, Direction dir)
{
     ASSERT(elevator >= 0 && elevator < numElevators);
     elevators[elevator]->display = dir;
}


//----------------------------------------------------------------------
// ElevatorBank::PressButton
//	Rider has pressed button to indicate where he/she wants to go
//----------------------------------------------------------------------

void 
ElevatorBank::PressButton(int onFloor, Direction goingTo)
{
    ASSERT(0 <= onFloor && onFloor < numFloors && goingTo != Neither);
    PostEvent(controllerEvents, 
    	goingTo == Up ? UpButtonPressed : DownButtonPressed,
    	onFloor, 0 /* elevator # ignored for this type of event */, FALSE);
}

//----------------------------------------------------------------------
// ElevatorBank::getDirection
//	Return direction set by MarkDirection
//----------------------------------------------------------------------

Direction 
ElevatorBank::getDirection(int elevator)
{
     ASSERT(elevator >= 0 && elevator < numElevators);
     return elevators[elevator]->display;
}

//----------------------------------------------------------------------
// ElevatorBank::EnterElevator
//	Rider steps onto elevator.  Elevator must be at "onFloor", and 
//	there must be space on the elevator.
//
//	Returns FALSE if doors have closed and/or elevator is on a 
//	different floor.
//----------------------------------------------------------------------

bool 
ElevatorBank::EnterElevator(int onFloor, int elevator)
{
    ASSERT(onFloor >= 0 && onFloor < numFloors);
    ASSERT(elevator >= 0 && elevator < numElevators);
    return elevators[elevator]->Enter(onFloor);
}

//----------------------------------------------------------------------
// ElevatorBank::PressFloor
//	Rider presses button inside elevator to say which
//	floor he/she wants to go to.  Rider must be on elevator!
//----------------------------------------------------------------------

void 
ElevatorBank::PressFloor(int goingToFloor, int elevator)
{
    ASSERT(goingToFloor >= 0 && goingToFloor < numFloors);
    ASSERT(elevator >= 0 && elevator < numElevators);
    //ASSERT(elevators[elevator]->riders->IsInList(currentThread));
    PostEvent(controllerEvents, FloorButtonPressed, goingToFloor, 
    					elevator, FALSE);
}
    
//----------------------------------------------------------------------
// ElevatorBank::ExitElevator
//	Rider steps off elevator.  Rider must be on elevator.
//	Return FALSE if doors have closed.
//----------------------------------------------------------------------

bool 
ElevatorBank::ExitElevator(int onFloor, int elevator)
{
    ASSERT(onFloor >= 0 && onFloor < numFloors);
    ASSERT(elevator >= 0 && elevator < numElevators);
    return elevators[elevator]->Exit(onFloor);
}
 
//----------------------------------------------------------------------
// ElevatorBank::CallBack
//	An event has occurred; check if someone needs to wakeup.
//----------------------------------------------------------------------

void 
ElevatorBank::HandleInterrupt() {
    bool intSched = FALSE;
    
    // Check if any of the elevators have reached a floor
    for (int i = 0; i < numElevators; i++) {
        if (elevators[i]->ReachedNextFloor()) {
	    if (elevators[i]->CheckArrived()) {
	    	PostEvent(controllerEvents, ElevatorArrived, 
	    			elevators[i]->goingTo, i, TRUE);
	    } else if (!intSched) {	
	// need to schedule interrupt for when we reach the next floor
	    	intSched = TRUE;
		elevators[i]->willArrive += DelayPerFloor;
	    	interrupt->Schedule(ElevatorDone, (int)this, DelayPerFloor, ElevatorInt);
	    }
	}
	        
    }
     
    // wake up riders and/or controllers if there are events
    // pending for them
    if (!riderEvents->IsEmpty()) {
         //callRiders->CallBack();
         (*handlerRiders)(argRiders);
    }
    if (!controllerEvents->IsEmpty()) {
         //callControllers->CallBack();
         (*handlerControllers)(argControllers);
    }
}

//----------------------------------------------------------------------
// ElevatorBank::getNextEvent
//	Retrieve an event posted by the elevator device.
//----------------------------------------------------------------------

ElevatorEvent 
ElevatorBank::getNextEvent(ListOfEvents *list, int *floor, 
							int *elevator)
{
    PendingElevatorEvent *event;
    ElevatorEvent returnValue;
    
    if (list->IsEmpty()) {
    	return NoEvent;
    }
    event = (PendingElevatorEvent*)(list->Remove());
    returnValue = event->event;
    *floor = event->floor;
    *elevator = event->elevator;
    delete event;
    return returnValue;
}

void
ElevatorBank::PostEvent(ListOfEvents *list, ElevatorEvent ev, 
				int floor, int elevator, bool inHandler) 
{
    PendingElevatorEvent *event = 
    		new PendingElevatorEvent(ev, floor, elevator);
    
    list->Append(event);
    
    // if we're not already in an interrupt handler,
    // cause an interrupt to occur to pick up this event (soon)
    if (!inHandler) { 
        interrupt->Schedule(ElevatorDone, (int)this, 1, ElevatorInt);
    }
}
