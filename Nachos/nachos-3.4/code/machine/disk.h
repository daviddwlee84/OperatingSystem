// disk.h 
//	Data structures to emulate a physical disk.  A physical disk
//	can accept (one at a time) requests to read/write a disk sector;
//	when the request is satisfied, the CPU gets an interrupt, and 
//	the next request can be sent to the disk.
//
//	Disk contents are preserved across machine crashes, but if
//	a file system operation (eg, create a file) is in progress when the 
//	system shuts down, the file system may be corrupted.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef DISK_H
#define DISK_H

#include "copyright.h"
#include "utility.h"

// The following class defines a physical disk I/O device.  The disk
// has a single surface, split up into "tracks", and each track split
// up into "sectors" (the same number of sectors on each track, and each
// sector has the same number of bytes of storage).  
//
// Addressing is by sector number -- each sector on the disk is given
// a unique number: track * SectorsPerTrack + offset within a track.
//
// As with other I/O devices, the raw physical disk is an asynchronous device --
// requests to read or write portions of the disk return immediately,
// and an interrupt is invoked later to signal that the operation completed.
//
// The physical disk is in fact simulated via operations on a UNIX file.
//
// To make life a little more realistic, the simulated time for
// each operation reflects a "track buffer" -- RAM to store the contents
// of the current track as the disk head passes by.  The idea is that the
// disk always transfers to the track buffer, in case that data is requested
// later on.  This has the benefit of eliminating the need for 
// "skip-sector" scheduling -- a read request which comes in shortly after 
// the head has passed the beginning of the sector can be satisfied more 
// quickly, because its contents are in the track buffer.  Most 
// disks these days now come with a track buffer.
//
// The track buffer simulation can be disabled by compiling with -DNOTRACKBUF

#define SectorSize 		128	// number of bytes per disk sector
#define SectorsPerTrack 	32	// number of sectors per disk track 
#define NumTracks 		32	// number of tracks per disk
#define NumSectors 		(SectorsPerTrack * NumTracks)
					// total # of sectors per disk

class Disk {
  public:
    Disk(char* name, VoidFunctionPtr callWhenDone, int callArg);
    					// Create a simulated disk.  
					// Invoke (*callWhenDone)(callArg) 
					// every time a request completes.
    ~Disk();				// Deallocate the disk.
    
    void ReadRequest(int sectorNumber, char* data);
    					// Read/write an single disk sector.
					// These routines send a request to 
    					// the disk and return immediately.
    					// Only one request allowed at a time!
    void WriteRequest(int sectorNumber, char* data);

    void HandleInterrupt();		// Interrupt handler, invoked when
					// disk request finishes.

    int ComputeLatency(int newSector, bool writing);	
    					// Return how long a request to 
					// newSector will take: 
					// (seek + rotational delay + transfer)

  private:
    int fileno;				// UNIX file number for simulated disk 
    VoidFunctionPtr handler;		// Interrupt handler, to be invoked 
					// when any disk request finishes
    int handlerArg;			// Argument to interrupt handler 
    bool active;     			// Is a disk operation in progress?
    int lastSector;			// The previous disk request 
    int bufferInit;			// When the track buffer started 
					// being loaded

    int TimeToSeek(int newSector, int *rotate); // time to get to the new track
    int ModuloDiff(int to, int from);        // # sectors between to and from
    void UpdateLast(int newSector);
};

#endif // DISK_H
