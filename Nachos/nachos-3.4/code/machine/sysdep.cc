// sysdep.cc
//	Implementation of system-dependent interface.  Nachos uses the 
//	routines defined here, rather than directly calling the UNIX library,
//	to simplify porting between versions of UNIX, and even to
//	other systems, such as MSDOS.
//
//	On UNIX, almost all of these routines are simple wrappers
//	for the underlying UNIX system calls.
//
//	NOTE: all of these routines refer to operations on the underlying
//	host machine (e.g., the DECstation, SPARC, etc.), supporting the 
//	Nachos simulation code.  Nachos implements similar operations,
//	(such as opening a file), but those are implemented in terms
//	of hardware devices, which are simulated by calls to the underlying
//	routines in the host workstation OS.
//
//	This file includes lots of calls to C routines.  C++ requires
//	us to wrap all C definitions with a "extern "C" block".
// 	This prevents the internal forms of the names from being
// 	changed by the C++ compiler.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

extern "C" {
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/un.h>
#include <sys/mman.h>
#ifdef HOST_i386
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#endif
#ifdef HOST_SPARC
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#endif


// UNIX routines called by procedures in this file 

#ifdef HOST_SNAKE
// int creat(char *name, unsigned short mode);
// int open(const char *name, int flags, ...);
#else
  //int creat(const char *name, unsigned short mode);
  //int open(const char *name, int flags, ...);
// void signal(int sig, VoidFunctionPtr func); -- this may work now!
#ifdef HOST_i386
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
             struct timeval *timeout);
#else
#ifdef HOST_SPARC
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
             struct timeval *timeout);
#else
int select(int numBits, void *readFds, void *writeFds, void *exceptFds, 
	struct timeval *timeout);
#endif
#endif
#endif

  //int unlink(char *name);
  //int read(int filedes, char *buf, int numBytes);
  //int write(int filedes, char *buf, int numBytes);
  //int lseek(int filedes, int offset, int whence);
  //int tell(int filedes);
  //int close(int filedes);
  //int unlink(char *name);

// definition varies slightly from platform to platform, so don't 
// define unless gcc complains
// extern int recvfrom(int s, void *buf, int len, int flags, void *from, int *fromlen);
// extern int sendto(int s, void *msg, int len, int flags, void *to, int tolen);


void srand(unsigned seed);
int rand(void);
unsigned sleep(unsigned);
void abort();
void exit(int);
  //int mprotect(char *addr, int len, int prot);

  //int socket(int, int, int);
  //int bind (int, const void*, int);
  //int recvfrom (int, void*, int, int, void*, int *);
  //int sendto (int, const void*, int, int, void*, int);
}

#include "interrupt.h"
#include "system.h"

//----------------------------------------------------------------------
// PollFile
// 	Check open file or open socket to see if there are any 
//	characters that can be read immediately.  If so, read them
//	in, and return TRUE.
//
//	In the network case, if there are no threads for us to run,
//	and no characters to be read,
//	we need to give the other side a chance to get our host's CPU
//	(otherwise, we'll go really slowly, since UNIX time-slices
//	infrequently, and this would be like busy-waiting).  So we
//	delay for a short fixed time, before allowing ourselves to be
//	re-scheduled (sort of like a Yield, but cast in terms of UNIX).
//
//	"fd" -- the file descriptor of the file to be polled
//----------------------------------------------------------------------

bool
PollFile(int fd)
{
    int rfd = (1 << fd), wfd = 0, xfd = 0, retVal;
    struct timeval pollTime;

// decide how long to wait if there are no characters on the file
    pollTime.tv_sec = 0;
    if (interrupt->getStatus() == IdleMode)
        pollTime.tv_usec = 20000;              	// delay to let other nachos run
    else
        pollTime.tv_usec = 0;                 	// no delay

// poll file or socket
#if (defined(HOST_i386) || defined(HOST_SPARC)) 
    retVal = select(32, (fd_set*)&rfd, (fd_set*)&wfd, (fd_set*)&xfd, &pollTime);
#else
    retVal = select(32, &rfd, &wfd, &xfd, &pollTime);
#endif

    ASSERT((retVal == 0) || (retVal == 1));
    if (retVal == 0)
	return FALSE;                 		// no char waiting to be read
    return TRUE;
}

//----------------------------------------------------------------------
// OpenForWrite
// 	Open a file for writing.  Create it if it doesn't exist; truncate it 
//	if it does already exist.  Return the file descriptor.
//
//	"name" -- file name
//----------------------------------------------------------------------

int
OpenForWrite(char *name)
{
    int fd = open(name, O_RDWR|O_CREAT|O_TRUNC, 0666);

    ASSERT(fd >= 0); 
    return fd;
}

//----------------------------------------------------------------------
// OpenForReadWrite
// 	Open a file for reading or writing.
//	Return the file descriptor, or error if it doesn't exist.
//
//	"name" -- file name
//----------------------------------------------------------------------

int
OpenForReadWrite(char *name, bool crashOnError)
{
    int fd = open(name, O_RDWR, 0);

    ASSERT(!crashOnError || fd >= 0);
    return fd;
}

//----------------------------------------------------------------------
// Read
// 	Read characters from an open file.  Abort if read fails.
//----------------------------------------------------------------------

void
Read(int fd, char *buffer, int nBytes)
{
    int retVal = read(fd, buffer, nBytes);
    ASSERT(retVal == nBytes);
}

//----------------------------------------------------------------------
// ReadPartial
// 	Read characters from an open file, returning as many as are
//	available.
//----------------------------------------------------------------------

int
ReadPartial(int fd, char *buffer, int nBytes)
{
    return read(fd, buffer, nBytes);
}


//----------------------------------------------------------------------
// WriteFile
// 	Write characters to an open file.  Abort if write fails.
//----------------------------------------------------------------------

void
WriteFile(int fd, char *buffer, int nBytes)
{
    int retVal = write(fd, buffer, nBytes);
    ASSERT(retVal == nBytes);
}

//----------------------------------------------------------------------
// Lseek
// 	Change the location within an open file.  Abort on error.
//----------------------------------------------------------------------

void 
Lseek(int fd, int offset, int whence)
{
    int retVal = lseek(fd, offset, whence);
    ASSERT(retVal >= 0);
}

//----------------------------------------------------------------------
// Tell
// 	Report the current location within an open file.
//----------------------------------------------------------------------

int 
Tell(int fd)
{
#ifdef HOST_i386
    return lseek(fd,0,SEEK_CUR); // 386BSD doesn't have the tell() system call
#else
    return tell(fd);
#endif
}


//----------------------------------------------------------------------
// Close
// 	Close a file.  Abort on error.
//----------------------------------------------------------------------

void 
Close(int fd)
{
    int retVal = close(fd);
    ASSERT(retVal >= 0); 
}

//----------------------------------------------------------------------
// Unlink
// 	Delete a file.
//----------------------------------------------------------------------

bool 
Unlink(char *name)
{
    return unlink(name);
}

//----------------------------------------------------------------------
// OpenSocket
// 	Open an interprocess communication (IPC) connection.  For now, 
//	just open a datagram port where other Nachos (simulating 
//	workstations on a network) can send messages to this Nachos.
//----------------------------------------------------------------------

int
OpenSocket()
{
    int sockID;
    
    sockID = socket(AF_UNIX, SOCK_DGRAM, 0);
    ASSERT(sockID >= 0);

    return sockID;
}

//----------------------------------------------------------------------
// CloseSocket
// 	Close the IPC connection. 
//----------------------------------------------------------------------

void
CloseSocket(int sockID)
{
    (void) close(sockID);
}

//----------------------------------------------------------------------
// InitSocketName
// 	Initialize a UNIX socket address -- magical!
//----------------------------------------------------------------------

static void 
InitSocketName(struct sockaddr_un *uname, char *name)
{
    uname->sun_family = AF_UNIX;
    strcpy(uname->sun_path, name);
}

//----------------------------------------------------------------------
// AssignNameToSocket
// 	Give a UNIX file name to the IPC port, so other instances of Nachos
//	can locate the port. 
//----------------------------------------------------------------------

void
AssignNameToSocket(char *socketName, int sockID)
{
    struct sockaddr_un uName;
    int retVal;

    (void) unlink(socketName);    // in case it's still around from last time

    InitSocketName(&uName, socketName);
    retVal = bind(sockID, (struct sockaddr *) &uName, sizeof(uName));
    ASSERT(retVal >= 0);
    DEBUG('n', "Created socket %s\n", socketName);
}

//----------------------------------------------------------------------
// DeAssignNameToSocket
// 	Delete the UNIX file name we assigned to our IPC port, on cleanup.
//----------------------------------------------------------------------
void
DeAssignNameToSocket(char *socketName)
{
    (void) unlink(socketName);
}

//----------------------------------------------------------------------
// PollSocket
// 	Return TRUE if there are any messages waiting to arrive on the
//	IPC port.
//----------------------------------------------------------------------
bool
PollSocket(int sockID)
{
    return PollFile(sockID);	// on UNIX, socket ID's are just file ID's
}

//----------------------------------------------------------------------
// ReadFromSocket
// 	Read a fixed size packet off the IPC port.  Abort on error.
//----------------------------------------------------------------------
void
ReadFromSocket(int sockID, char *buffer, int packetSize)
{
    int retVal;
    //    extern int errno;	errno sometimes defined as a macro
    struct sockaddr_un uName;
#ifdef HOST_i386
    unsigned int size = sizeof(uName);
#else
    int size = sizeof(uName);
#endif

    retVal = recvfrom(sockID, buffer, packetSize, 0,
				   (struct sockaddr *) &uName, &size);

    if (retVal != packetSize) {
        perror("in recvfrom");
        printf("called: %x, got back %d, %d\n", (unsigned int) buffer, retVal, errno);
    }
    ASSERT(retVal == packetSize);
}

//----------------------------------------------------------------------
// SendToSocket
// 	Transmit a fixed size packet to another Nachos' IPC port.
//	Abort on error.
//----------------------------------------------------------------------
void
SendToSocket(int sockID, char *buffer, int packetSize, char *toName)
{
    struct sockaddr_un uName;
    int retVal;

    InitSocketName(&uName, toName);
    retVal = sendto(sockID, buffer, packetSize, 0,
			   (sockaddr*) &uName, sizeof(uName));
    ASSERT(retVal == packetSize);
}


//----------------------------------------------------------------------
// CallOnUserAbort
// 	Arrange that "func" will be called when the user aborts (e.g., by
//	hitting ctl-C.
//----------------------------------------------------------------------

void 
CallOnUserAbort(VoidNoArgFunctionPtr func)
{
    (void)signal(SIGINT, (VoidFunctionPtr) func);
}

//----------------------------------------------------------------------
// Sleep
// 	Put the UNIX process running Nachos to sleep for x seconds,
//	to give the user time to start up another invocation of Nachos
//	in a different UNIX shell.
//----------------------------------------------------------------------

void 
Delay(int seconds)
{
    (void) sleep((unsigned) seconds);
}

//----------------------------------------------------------------------
// Abort
// 	Quit and drop core.
//----------------------------------------------------------------------

void 
Abort()
{
    abort();
}

//----------------------------------------------------------------------
// Exit
// 	Quit without dropping core.
//----------------------------------------------------------------------

void 
Exit(int exitCode)
{
    exit(exitCode);
}

//----------------------------------------------------------------------
// RandomInit
// 	Initialize the pseudo-random number generator.  We use the
//	now obsolete "srand" and "rand" because they are more portable!
//----------------------------------------------------------------------

void 
RandomInit(unsigned seed)
{
    srand(seed);
}

//----------------------------------------------------------------------
// Random
// 	Return a pseudo-random number.
//----------------------------------------------------------------------

int 
Random()
{
    return rand();
}

//----------------------------------------------------------------------
// AllocBoundedArray
// 	Return an array, with the two pages just before 
//	and after the array unmapped, to catch illegal references off
//	the end of the array.  Particularly useful for catching overflow
//	beyond fixed-size thread execution stacks.
//
//	Note: Just return the useful part!
//
//	"size" -- amount of useful space needed (in bytes)
//----------------------------------------------------------------------

char * 
AllocBoundedArray(int size)
{
    int pgSize = getpagesize();
    char *ptr = new char[pgSize * 2 + size];

    //mprotect(ptr, pgSize, 0);
    //mprotect(ptr + pgSize + size, pgSize, 0);
    return ptr + pgSize;
}

//----------------------------------------------------------------------
// DeallocBoundedArray
// 	Deallocate an array of integers, unprotecting its two boundary pages.
//
//	"ptr" -- the array to be deallocated
//	"size" -- amount of useful space in the array (in bytes)
//----------------------------------------------------------------------

void 
DeallocBoundedArray(char *ptr, int size)
{
    int pgSize = getpagesize();

    mprotect(ptr - pgSize, pgSize, PROT_READ | PROT_WRITE | PROT_EXEC);
    mprotect(ptr + size, pgSize, PROT_READ | PROT_WRITE | PROT_EXEC);
    delete [] (ptr - pgSize);
}
