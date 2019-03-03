// elevatortest.cc 
//	Driver code to test the elevator device implementation.
//	We create a single rider thread and a single elevator thread;
//	in practice there will be multiple riders and multiple elevators,
//	each with its own thread.
//
// Copyright (c) 1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "elevatortest.h"
#include "elevator.h"
#include "synch.h"
#include "thread.h"

// Data structures to control elevator device self test.
// Definition private to this module. 
class ElevatorInspector {
  public:
    ElevatorInspector();	// allocate self test data structures
    ~ElevatorInspector();	// deallocate self test data
    
    void RiderTest();		// test out the elevator simulation,
  				// by acting like a rider
    void ControllerTest();	// test out elevator simulation, by
    				// acting like an elevator controller
    //void CallBack();		// get notification of event
    
  private:
    ElevatorBank *elevators;
    Semaphore *riderWakeup;	// to synchronize rider with callbacks
    Semaphore *controllerWakeup;// to synch controller with callbacks
    
    // The next two functions wait for the rider/controller
    // to be signalled.  In a real implementation there would be
    // multiple riders and multiple elevators, so some way of getting
    // each event to the right set of threads is needed.
    ElevatorEvent WaitForNextRiderEvent(int *floor, int *elevator) {
        ElevatorEvent event;
	
        while ((event = elevators->getNextRiderEvent(
        				floor, elevator)) == NoEvent) {
            riderWakeup->P();
        }
        return event;
    }
    ElevatorEvent WaitForNextControllerEvent(int *floor, int *elevator) {
        ElevatorEvent event;
	
        while ((event = elevators->getNextControllerEvent(
        				floor, elevator)) == NoEvent) {
            controllerWakeup->P();
        }
        return event;
    }

  public:
    Semaphore *getriderWakeup() {return riderWakeup;};
    Semaphore *getcontrollerWakeup() {return controllerWakeup;};

};

static void
ControllerTest(ElevatorInspector *inspector) {
    inspector->ControllerTest();
}

static void
riders (int arg) {
    ElevatorInspector *inspector = (ElevatorInspector *)arg;
    inspector->getriderWakeup()->V();
}

static void
controllers (int arg) {
    ElevatorInspector *inspector = (ElevatorInspector *)arg;
    inspector->getcontrollerWakeup()->V();
}

//----------------------------------------------------------------------
// ElevatorSelfTest
//	Run self test on elevator device.  Create one thread to
//	to act as the rider, and another to act as the controller.
//----------------------------------------------------------------------

void
ElevatorSelfTest () {
    ElevatorInspector *inspector = new ElevatorInspector();
    Thread *controller = new Thread("controller");
    
    controller->Fork(ControllerTest, inspector);
    inspector->RiderTest();
    delete inspector;
}

//----------------------------------------------------------------------
// ElevatorInspector::ElevatorInspector
//	Allocate and initialize data structures for testing the
//	elevator simulation.
//----------------------------------------------------------------------

ElevatorInspector::ElevatorInspector()
{ 
    elevators = new ElevatorBank(1 /*numLifts*/, 2 /*numFlrs*/, riders, (int)this, controllers, (int)this);
    riderWakeup = new Semaphore("rider", 0);
    controllerWakeup = new Semaphore("controller", 0);
}

//----------------------------------------------------------------------
// ElevatorInspector::~ElevatorInspector
//	Deallocate data structures for testing the elevator simulation.
//----------------------------------------------------------------------

ElevatorInspector::~ElevatorInspector()
{ 
    delete elevators;
    delete riderWakeup;
    delete controllerWakeup;
}
        
//----------------------------------------------------------------------
// ElevatorInspector::RiderTest
//	Behave like a rider, going from floor 0 to floor 1.
//----------------------------------------------------------------------

void
ElevatorInspector::RiderTest () {
    int floor, elevator;
    ElevatorEvent event;
    
    elevators->PressButton(0, Up);
    event = WaitForNextRiderEvent(&floor, &elevator);
    ASSERT(event == DoorsOpened && floor == 0);
    ASSERT(elevators->getDirection(elevator) == Up);
    // should make sure that there aren't too many on the elevator
    // already
    ASSERT(elevators->EnterElevator(floor, elevator));
    elevators->PressFloor(1, elevator);
    event = WaitForNextRiderEvent(&floor, &elevator);
    ASSERT(event == DoorsOpened && floor == 1);
    ASSERT(elevators->ExitElevator(floor, elevator));
}

//----------------------------------------------------------------------
// ElevatorInspector::ControllerTest
//	Behave like an elevator controller, for testing the
//	elevator simulation.
//----------------------------------------------------------------------

void
ElevatorInspector::ControllerTest () {
    int floor, elevator;
    ElevatorEvent event;

    // assume elevator starts on ground floor    
    ASSERT(elevators->WhereIsElevator(0) == 0);	
    event = WaitForNextControllerEvent(&floor, &elevator);
    ASSERT(event == UpButtonPressed && floor == 0);
    
    // indicate to rider that elevator is going up
    elevators->MarkDirection(0, Up);	
    elevators->OpenDoors(0);
    
    // should wait a fixed amount of time for the rider to get
    // on, in case they took the stairs
    event = WaitForNextControllerEvent(&floor, &elevator);
    ASSERT(event == FloorButtonPressed && floor == 1);
    elevators->CloseDoors(0);
    elevators->MoveTo(floor, 0);
    event = WaitForNextControllerEvent(&floor, &elevator);
    ASSERT(event == ElevatorArrived && floor == 1 && elevator == 0);
    elevators->OpenDoors(elevator);
}

//----------------------------------------------------------------------
// ElevatorInspector::CallBack
//	Handle an event; since we don't know whether it is a
//	rider event or a controller event, just wake them both up.
//	If there's no event pending, they'll just go right back
//	to sleep.
//----------------------------------------------------------------------

/*
void
ElevatorInspector::CallBack() {
    controllerWakeup->V();
    riderWakeup->V();
}*/

