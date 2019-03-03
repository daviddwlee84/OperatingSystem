// post.h 
//	Data structures for providing the abstraction of unreliable,
//	ordered, fixed-size message delivery to mailboxes on other 
//	(directly connected) machines.  Messages can be dropped by
//	the network, but they are never corrupted.
//
// 	The US Post Office delivers mail to the addressed mailbox. 
// 	By analogy, our post office delivers packets to a specific buffer 
// 	(MailBox), based on the mailbox number stored in the packet header.
// 	Mail waits in the box until a thread asks for it; if the mailbox
//      is empty, threads can wait for mail to arrive in it. 
//
// 	Thus, the service our post office provides is to de-multiplex 
// 	incoming packets, delivering them to the appropriate thread.
//
//      With each message, you get a return address, which consists of a "from
// 	address", which is the id of the machine that sent the message, and
// 	a "from box", which is the number of a mailbox on the sending machine 
//	to which you can send an acknowledgement, if your protocol requires 
//	this.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#ifndef POST_H
#define POST_H

#include "network.h"
#include "synchlist.h"

// Mailbox address -- uniquely identifies a mailbox on a given machine.
// A mailbox is just a place for temporary storage for messages.
typedef int MailBoxAddress;

// The following class defines part of the message header.  
// This is prepended to the message by the PostOffice, before the message 
// is sent to the Network.

class MailHeader {
  public:
    MailBoxAddress to;		// Destination mail box
    MailBoxAddress from;	// Mail box to reply to
    unsigned length;		// Bytes of message data (excluding the 
				// mail header)
};

// Maximum "payload" -- real data -- that can included in a single message
// Excluding the MailHeader and the PacketHeader

#define MaxMailSize 	(MaxPacketSize - sizeof(MailHeader))


// The following class defines the format of an incoming/outgoing 
// "Mail" message.  The message format is layered: 
//	network header (PacketHeader) 
//	post office header (MailHeader) 
//	data

class Mail {
  public:
     Mail(PacketHeader pktH, MailHeader mailH, char *msgData);
				// Initialize a mail message by
				// concatenating the headers to the data

     PacketHeader pktHdr;	// Header appended by Network
     MailHeader mailHdr;	// Header appended by PostOffice
     char data[MaxMailSize];	// Payload -- message data
};

// The following class defines a single mailbox, or temporary storage
// for messages.   Incoming messages are put by the PostOffice into the 
// appropriate mailbox, and these messages can then be retrieved by
// threads on this machine.

class MailBox {
  public: 
    MailBox();			// Allocate and initialize mail box
    ~MailBox();			// De-allocate mail box

    void Put(PacketHeader pktHdr, MailHeader mailHdr, char *data);
   				// Atomically put a message into the mailbox
    void Get(PacketHeader *pktHdr, MailHeader *mailHdr, char *data); 
   				// Atomically get a message out of the 
				// mailbox (and wait if there is no message 
				// to get!)
  private:
    SynchList *messages;	// A mailbox is just a list of arrived messages
};

// The following class defines a "Post Office", or a collection of 
// mailboxes.  The Post Office is a synchronization object that provides
// two main operations: Send -- send a message to a mailbox on a remote 
// machine, and Receive -- wait until a message is in the mailbox, 
// then remove and return it.
//
// Incoming messages are put by the PostOffice into the 
// appropriate mailbox, waking up any threads waiting on Receive.

class PostOffice {
  public:
    PostOffice(NetworkAddress addr, double reliability, int nBoxes);
				// Allocate and initialize Post Office
				//   "reliability" is how many packets
				//   get dropped by the underlying network
    ~PostOffice();		// De-allocate Post Office data
    
    void Send(PacketHeader pktHdr, MailHeader mailHdr, char *data);
    				// Send a message to a mailbox on a remote 
				// machine.  The fromBox in the MailHeader is 
				// the return box for ack's.
    
    void Receive(int box, PacketHeader *pktHdr, 
		MailHeader *mailHdr, char *data);
    				// Retrieve a message from "box".  Wait if
				// there is no message in the box.

    void PostalDelivery();	// Wait for incoming messages, 
				// and then put them in the correct mailbox

    void PacketSent();		// Interrupt handler, called when outgoing 
				// packet has been put on network; next 
				// packet can now be sent
    void IncomingPacket();	// Interrupt handler, called when incoming
   				// packet has arrived and can be pulled
				// off of network (i.e., time to call 
				// PostalDelivery)

  private:
    Network *network;		// Physical network connection
    NetworkAddress netAddr;	// Network address of this machine
    MailBox *boxes;		// Table of mail boxes to hold incoming mail
    int numBoxes;		// Number of mail boxes
    Semaphore *messageAvailable;// V'ed when message has arrived from network
    Semaphore *messageSent;	// V'ed when next message can be sent to network
    Lock *sendLock;		// Only one outgoing message at a time
};

#endif
