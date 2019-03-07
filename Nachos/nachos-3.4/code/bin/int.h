
/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#include "copyright.h"


#define MEMSIZE (1<<24)
#define memoffset 0x10000000

/* centralized memory-access primitives */
#define amark(x)	x
#define imark(x)	x

#define ifetch(addr)	(*(int *)(int) (&(mem-memoffset)[imark(addr)]))
#define fetch(addr)	(*(int *)(int) (&(mem-memoffset)[amark(addr)]))
#define sfetch(addr)	(*(short *)(int) (&(mem-memoffset)[amark(addr)]))
#define usfetch(addr)	(*(unsigned short *)(int)(&(mem-memoffset)[amark(addr)]))
#define cfetch(addr)	(*(char *)(int) (&(mem-memoffset)[amark(addr)]))
#define ucfetch(addr)	(*(unsigned char *)(int)(&(mem-memoffset)[amark(addr)]))

#define store(addr, i)	\
	((*(int *)(int) (&(mem-memoffset)[amark(addr)]) = (i)))
#define sstore(addr, i)	\
	((*(short *)(int) (&(mem-memoffset)[amark(addr)]) = (i)))
#define cstore(addr, i)	\
	(((mem-memoffset)[amark(addr)] = (i)))

