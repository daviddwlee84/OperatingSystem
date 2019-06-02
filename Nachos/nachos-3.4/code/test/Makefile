# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort fibonacci exit filesyscall execjoin forkyield

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

# For Lab4: Test the TLB Miss Rate
fibonacci.o: fibonacci.c
	$(CC) $(CFLAGS) -c fibonacci.c
fibonacci: fibonacci.o start.o
	$(LD) $(LDFLAGS) start.o fibonacci.o -o fibonacci.coff
	../bin/coff2noff fibonacci.coff fibonacci

# For Lab4: Test the multi-thread user program
exit.o: exit.c
	$(CC) $(CFLAGS) -c exit.c
exit: exit.o start.o
	$(LD) $(LDFLAGS) start.o exit.o -o exit.coff
	../bin/coff2noff exit.coff exit

# For Lab5: Test file system syscall
filesyscall.o: filesyscall.c
	$(CC) $(CFLAGS) -c filesyscall.c
filesyscall: filesyscall.o start.o
	$(LD) $(LDFLAGS) start.o filesyscall.o -o filesyscall.coff
	../bin/coff2noff filesyscall.coff filesyscall

# For Lab5: Test address space control operations syscall
execjoin.o: execjoin.c
	$(CC) $(CFLAGS) -c execjoin.c
execjoin: execjoin.o start.o
	$(LD) $(LDFLAGS) start.o execjoin.o -o execjoin.coff
	../bin/coff2noff execjoin.coff execjoin

# For Lab5: Test user-level thread operations syscall
forkyield.o: forkyield.c
	$(CC) $(CFLAGS) -c forkyield.c
forkyield: forkyield.o start.o
	$(LD) $(LDFLAGS) start.o forkyield.o -o forkyield.coff
	../bin/coff2noff forkyield.coff forkyield
