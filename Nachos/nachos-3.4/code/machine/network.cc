// network.cc 
//	Routines to simulate a network interface, using UNIX sockets
//	to deliver packets between multiple invocations of nachos.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

// Dummy functions because C++ can't call member functions indirectly 
static void NetworkReadPoll(int arg)
{ Network *net = (Network *)arg; net->CheckPktAvail(); }
static void NetworkSendDone(int arg)
{ Network *net = (Network *)arg; net->SendDone(); }

// Initialize the network emulation
//   addr is used to generate the socket name
//   reliability says whether we drop packets to emulate unreliable links
//   readAvail, writeDone, callArg -- analogous to console
Network::Network(NetworkAddress addr, double reliability,
	VoidFunctionPtr readAvail, VoidFunctionPtr writeDone, int callArg)
{
    ident = addr;
    if (reliability < 0) chanceToWork = 0;
    else if (reliability > 1) chanceToWork = 1;
    else chanceToWork = reliability;

    // set up the stuff to emulate asynchronous interrupts
    writeHandler = writeDone;
    readHandler = readAvail;
    handlerArg = callArg;
    sendBusy = FALSE;
    inHdr.length = 0;
    
    sock = OpenSocket();
    sprintf(sockName, "SOCKET_%d", (int)addr);
    AssignNameToSocket(sockName, sock);		 // Bind socket to a filename 
						 // in the current directory.

    // start polling for incoming packets
    interrupt->Schedule(NetworkReadPoll, (int)this, NetworkTime, NetworkRecvInt);
}

Network::~Network()
{
    CloseSocket(sock);
    DeAssignNameToSocket(sockName);
}

// if a packet is already buffered, we simply delay reading 
// the incoming packet.  In real life, the incoming 
// packet might be dropped if we can't read it in time.
void
Network::CheckPktAvail()
{
    // schedule the next time to poll for a packet
    interrupt->Schedule(NetworkReadPoll, (int)this, NetworkTime, NetworkRecvInt);

    if (inHdr.length != 0) 	// do nothing if packet is already buffered
	return;		
    if (!PollSocket(sock)) 	// do nothing if no packet to be read
	return;

    // otherwise, read packet in
    char *buffer = new char[MaxWireSize];
    ReadFromSocket(sock, buffer, MaxWireSize);

    // divide packet into header and data
    inHdr = *(PacketHeader *)buffer;
    ASSERT((inHdr.to == ident) && (inHdr.length <= MaxPacketSize));
    bcopy(buffer + sizeof(PacketHeader), inbox, inHdr.length);
    delete []buffer ;

    DEBUG('n', "Network received packet from %d, length %d...\n",
	  				(int) inHdr.from, inHdr.length);
    stats->numPacketsRecvd++;

    // tell post office that the packet has arrived
    (*readHandler)(handlerArg);	
}

// notify user that another packet can be sent
void
Network::SendDone()
{
    sendBusy = FALSE;
    stats->numPacketsSent++;
    (*writeHandler)(handlerArg);
}

// send a packet by concatenating hdr and data, and schedule
// an interrupt to tell the user when the next packet can be sent 
//
// Note we always pad out a packet to MaxWireSize before putting it into
// the socket, because it's simpler at the receive end.
void
Network::Send(PacketHeader hdr, char* data)
{
    char toName[32];

    sprintf(toName, "SOCKET_%d", (int)hdr.to);
    
    ASSERT((sendBusy == FALSE) && (hdr.length > 0) 
		&& (hdr.length <= MaxPacketSize) && (hdr.from == ident));
    DEBUG('n', "Sending to addr %d, %d bytes... ", hdr.to, hdr.length);

    interrupt->Schedule(NetworkSendDone, (int)this, NetworkTime, NetworkSendInt);

    if (Random() % 100 >= chanceToWork * 100) { // emulate a lost packet
	DEBUG('n', "oops, lost it!\n");
	return;
    }

    // concatenate hdr and data into a single buffer, and send it out
    char *buffer = new char[MaxWireSize];
    *(PacketHeader *)buffer = hdr;
    bcopy(data, buffer + sizeof(PacketHeader), hdr.length);
    SendToSocket(sock, buffer, MaxWireSize, toName);
    delete []buffer;
}

// read a packet, if one is buffered
PacketHeader
Network::Receive(char* data)
{
    PacketHeader hdr = inHdr;

    inHdr.length = 0;
    if (hdr.length != 0)
    	bcopy(inbox, data, hdr.length);
    return hdr;
}
