// elevator.h 
//	Data structures to emulate a bank of elevators.
//
//	There are two interfaces -- one for the threads controlling
//	the elevators, and one for threads representing elevator
//	passengers.  The only communication between these two
//	groups is via the elevator device.  The elevator device
//	interface has both regular calls from the rider/controller
//	threads to the device, as well as "callbacks" (interrupts) 
//	from the device to the riders/controllers when events occur.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "copyright.h"
#include "utility.h"
#include "list.h"

// which direction the elevator or rider is going
enum Direction { Down, Up, Neither};

// elevator events that require signalling rider (on DoorsOpened)
// or controllers (on ButtonPressed or ElevatorArrived)
enum ElevatorEvent { NoEvent, DoorsOpened, UpButtonPressed, 
	DownButtonPressed, FloorButtonPressed, ElevatorArrived};
		
const int MaxRiders = 4;	// carrying capacity of each elevator
const int DelayPerFloor = 100;	// how long does an elevator take
				// to move between floors?

class ElevatorInfo;
class PendingElevatorEvent;
typedef List ListOfEvents;

// The following class defines a bank of elevators
class ElevatorBank {
  public:
    ElevatorBank(int numElvtrs, int numFlrs, 
    			VoidFunctionPtr riders, int ridersArg, VoidFunctionPtr controllers, int controllersArg);
				// Initialize the elevator hardware, 
				// use "riders" and "controllers" to
				// notify the rider/controller threads
				// that an event occurred they should
				// be interested in (such as, elevator
				// arrives, doors open, etc)
    ~ElevatorBank();		// deallocate the elevator hardware
    
 // Controller interface to elevator hardware
 
    void OpenDoors(int elevator);// Open the elevator doors; elevator
    				// must not be in motion!
    
    void CloseDoors(int elevator);// Close the elevator doors.

    void MoveTo(int goingToFloor, int elevator);
        			// Set the elevator into motion.
				// Elevator doors must be closed.
				// If elevator is already in motion,
				// this causes the elevator to stop
				// at a new floor (eg, if a rider
				// presses a new floor while the
				// elevator is moving)
    int WhereIsElevator(int elevator);
    				// return the current position of
				// the elevator (which floor its on)

    void MarkDirection(int elevator, Direction dir);
    				// change the display to show riders
				// where this elevator is headed

    ElevatorEvent getNextControllerEvent(int *floor, int *elevator) {
    	return getNextEvent(controllerEvents, floor, elevator);
    }				// when the elevator device calls back,
				// this will tell you what event(s)
				// (relevant to controller threads)
				// triggered the callback.

// Rider interface to elevator control panel; roughly in
// the order that they are called by a rider.
 
    void PressButton(int onFloor, Direction goingTo);
    				// Press button outside elevator to
				// indicate where rider wants to go
    
    Direction getDirection(int elevator);
    				// return the indicated direction
				// (set by MarkDirection()) of this
				// elevator
				
    bool EnterElevator(int onFloor, int elevator);
    				// Step onto the elevator; elevator must have
				// < MaxRiders people on it currently
				// Returns false if doors have closed (might
				// have happened between when riders got
				// OpenDoors event and when they decided
				// to step onto the elevator).

    void PressFloor(int goingToFloor, int onElevator);
        			// Press button inside elevator to
				// indicate where rider wants to go;
				// thread must be on the elevator
    
    bool ExitElevator(int onFloor, int elevator);
    				// Step off the elevator. Returns false if
				// doors have closed.
    
    ElevatorEvent getNextRiderEvent(int *floor, int *elevator) {
    	return getNextEvent(riderEvents, floor, elevator);
    }				// when the elevator device calls back,
				// this will tell you what event(s)
				// (relevant to rider threads)
				// triggered the callback.

    void HandleInterrupt();

  private:
    int numElevators;		// how many elevators in this bank?
    int numFloors;		// how many floors in this building?
    VoidFunctionPtr handlerRiders, handlerControllers;
    int argRiders, argControllers;
//    CallBackObj *callRiders; 	// call when an event occurs that
    				// riders would be interested in
//    CallBackObj *callControllers; // call when an event occurs that
    				// elevator controller threads
				// would be interested in
    ListOfEvents *riderEvents;	// pending events relevant to riders
    ListOfEvents *controllerEvents;// pending events relevant to controllers
    ElevatorInfo **elevators;	// array of per-elevator state
    
    void PostEvent(ListOfEvents *list, ElevatorEvent event, int floor,
    			int elevator, bool inHandler);	
    				// an event occurred that requires
				// a callback
				
    ElevatorEvent getNextEvent(ListOfEvents *list, int *floor, int *elevator);
				// pull next event off the pending list
				// of events, return NoEvent if list is
				// empty
};

#endif // ELEVATOR_H
