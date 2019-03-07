// network.h 
//	Data structures to emulate a physical network connection.
//	The network provides the abstraction of ordered, unreliable,
//	fixed-size packet delivery to other machines on the network.
//
//	You may note that the interface to the network is similar to 
//	the console device -- both are full duplex channels.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef NETWORK_H
#define NETWORK_H

#include "copyright.h"
#include "utility.h"

// Network address -- uniquely identifies a machine.  This machine's ID 
//  is given on the command line.
typedef int NetworkAddress;	 

// The following class defines the network packet header.
// The packet header is prepended to the data payload by the Network driver, 
// before the packet is sent over the wire.  The format on the wire is:  
//	packet header (PacketHeader)
//	data (containing MailHeader from the PostOffice!)

class PacketHeader {
  public:
    NetworkAddress to;		// Destination machine ID
    NetworkAddress from;	// source machine ID
    unsigned length;	 	// bytes of packet data, excluding the 
				// packet header (but including the 
				// MailHeader prepended by the post office)
};

#define MaxWireSize 	64	// largest packet that can go out on the wire
#define MaxPacketSize 	(MaxWireSize - sizeof(struct PacketHeader))	
				// data "payload" of the largest packet


// The following class defines a physical network device.  The network
// is capable of delivering fixed sized packets, in order but unreliably, 
// to other machines connected to the network.
//
// The "reliability" of the network can be specified to the constructor.
// This number, between 0 and 1, is the chance that the network will lose 
// a packet.  Note that you can change the seed for the random number 
// generator, by changing the arguments to RandomInit() in Initialize().
// The random number generator is used to choose which packets to drop.

class Network {
  public:
    Network(NetworkAddress addr, double reliability,
  	  VoidFunctionPtr readAvail, VoidFunctionPtr writeDone, int callArg);
				// Allocate and initialize network driver
    ~Network();			// De-allocate the network driver data
    
    void Send(PacketHeader hdr, char* data);
    				// Send the packet data to a remote machine,
				// specified by "hdr".  Returns immediately.
    				// "writeHandler" is invoked once the next 
				// packet can be sent.  Note that writeHandler 
				// is called whether or not the packet is 
				// dropped, and note that the "from" field of 
				// the PacketHeader is filled in automatically 
				// by Send().

    PacketHeader Receive(char* data);
    				// Poll the network for incoming messages.  
				// If there is a packet waiting, copy the 
				// packet into "data" and return the header.
				// If no packet is waiting, return a header 
				// with length 0.

    void SendDone();		// Interrupt handler, called when message is 
				// sent
    void CheckPktAvail();	// Check if there is an incoming packet

  private:
    NetworkAddress ident;	// This machine's network address
    double chanceToWork;	// Likelihood packet will be dropped
    int sock;			// UNIX socket number for incoming packets
    char sockName[32];		// File name corresponding to UNIX socket
    VoidFunctionPtr writeHandler; // Interrupt handler, signalling next packet 
				//      can be sent.  
    VoidFunctionPtr readHandler;  // Interrupt handler, signalling packet has 
				// 	arrived.
    int handlerArg;		// Argument to be passed to interrupt handler
				//   (pointer to post office)
    bool sendBusy;		// Packet is being sent.
    bool packetAvail;		// Packet has arrived, can be pulled off of
				//   network
    PacketHeader inHdr;		// Information about arrived packet
    char inbox[MaxPacketSize];  // Data for arrived packet
};

#endif // NETWORK_H
