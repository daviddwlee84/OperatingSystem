This README file describes the Nachos release.  Comments, questions,
and bug reports are always welcome, and can be directed to 
nachos@cs.berkeley.edu (for now, an alias to just me, Tom Anderson), or 
to the alt.os.nachos newsgroup.

Nachos is instructional software for teaching undergraduate, and potentially
graduate, level operating systems courses.  The Nachos distribution
comes with: 

   an overview paper
   simple baseline code for a working operating system
   a simulator for a generic personal computer/workstation
   sample assignments
   a C++ primer (Nachos is written in an easy-to-learn subset of C++, 
     and the primer helps teach C programmers our subset)

The assignments illustrate and explore all areas of modern operating
systems, including threads and concurrency, multiprogramming, 
system calls, virtual memory, software-loaded TLB's, file systems, 
network protocols, remote procedure call, and distributed systems.

The most up to date version of nachos is linked to the file called, 
nachos.tar.Z.  On Jan. 20, 1993, this version was nachos-3.1.tar.Z,
but it will be periodically updated as bugs are fixed and features added.
REMEMBER TO TURN BINARY MODE ON WHEN RETRIEVING .Z FILES.

To get started, you should:
  1. use ftp to fetch the nachos.tar.Z file (turning on binary mode first)
  2. uncompress nachos.tar.Z
  3. tar -xf nachos.tar
  4. lpr nachos.ps  -- print out the paper describing nachos
  5. lpr doc/*.ps  -- print out the sample assignments
  6. cd code; make print -- print out the nachos source code
  7. cd code/c++example; lpr *.ps *.h *.cc -- print out C++ primer
  8. if you have no DEC MIPS workstations available at your site, you 
      will need to install the gcc cross-compiler on your system.  See
      the instructions at the end of this file.
  9. edit code/Makefile.dep to specify host machine type 
     if cross-compiling (cf. step 8) you also need to:
        edit code/test/Makefile and code/bin/Makefile
  10. cd code; make all -- compile nachos source code

Version 3 has been used for a semester at Berkeley by over a 
hundred students, so most of the bugs are out of the system.
However, there are likely to be some remaining problems; if you
find these, please send e-mail to nachos@cs.berkeley.edu (particularly
if you have a fix :-).

At present, Nachos runs on several platforms, including: 
  DEC MIPS, running Ultrix
  SUN SPARCstations (only tested on SunOS, not Solaris, though)
  HP PA-RISC, running HP-UX
  386 boxes, running 386BSD UNIX or FreeBSD

Notably we do not currently support:
  PC Windows 
  Macintosh
  non-SPARC SUN workstations
However, PC and Macintosh support is under development.  The main change
that you need to make to support another platform is an implementation
of the low-level machine-dependent context switch code, in threads/switch.s.
Several example architectures are now supported in switch.s.

The basic Nachos system was written for a MIPS workstation.  It has
been ported to other platforms, but as of now, there are a few gotchas.
The Nachos kernel and machine simulator run directly on the host machine,
but user-level programs running on top of Nachos are simulated instruction-by-
instruction.  The simulator assumes MIPS object code, in little endian format.
It would take much more work to complete the port and change the CPU 
simulator to simulate other instruction sets (although this is under
investigation).  Keeping the MIPS CPU causes a few problems:

1) You have to generate user-level code for the simulated machine.
If you have a heterogeneous environment with some MIPS and non-MIPS 
workstations, this isn't so hard -- students only need to compile a few 
small user programs.  But if you only have non-MIPS machines, you need to 
get gcc to cross-compile to the DEC MIPS.  Gcc only recently has been fixed 
to support this, and the instructions for how to do this are listed below.
If you are unable to get the cross-compiler to work, do not despair.
The distribution comes with a few simple user programs (pre-compiled to
MIPS object code) that students can use to test out Nachos kernel services.

2) The Nachos kernel runs runs native mode while the user programs
runs on the simulated CPU.  This is a little weird on the non-MIPS workstations
because the user programs are using little endian (typically) and the kernel is
using big endian.  Some information (such as the argv[] array) that
is passed between the kernel and the user though user memory must be
byte swapped.  (Unfortunately, this isn't as easy to fix as simply
cross-compiling to the SGI MIPS, which is big endian; in a few places,
the simulation assumes little endian format.  We're working on fixing this.) 

The end of this file contains the procedure for constructing a cross-compiler
to the MIPS, using the gcc toolkit.

Version 3.1 Release Notes
--------------------------

Primarily, fixed up problems with cross-compiled environment.


Version 3 Release Notes
-----------------------

Note that version 2 is still available, in 
  ftp.cs.berkeley.edu:ucb/nachos/version2


There are several major changes for version 3, relative to earlier versions:

1. Extensive comments.  All procedures and data structures now have
  commented explanations.  Hopefully, this will help make it easier for
  students (and professors) to read and understand the baseline system.
  In writing the comments, I realized that we continue to lack an "overall" 
  roadmap to the system; Nachos deals with conceptually hard issues
  in a bunch of places in the code, and I think students would find a roadmap
  helpful.  I am plotting how to do this; for now, my apologies for 
  anything that seems unduly complex and opaque.  Any suggestions
  for places that need better explanations are welcome.

2. Modifications to improve portability, along with ports to several more
  platforms.  All machine dependencies are now isolated into only a few 
  locations (primarily, switch.h/switch.s, and sysdep.h/sysdep.cc), making
  it much easier to port Nachos to new platforms.  This is evidenced by
  the fact that the HP PA-RISC and 386UNIX ports were each completed in a
  few days worth of concentrated effort.  There is now a common code base,
  so the separate code base for SPARCs in Version 2 is no longer needed.

3. The directory structure has been simplified and made more generic.
  Instead of directories named for the assignments that I give, I have
  named them after topic areas: threads, userprog, vm, filesys, and network.
  Each represents a single assignment, but there is a large amount
  of flexibility now in choosing the order to cover these topics.
  Here is the dependency graph:

  threads -> userprog -> vm
	  -> filesys
	  -> network

  In other words, all other assignments rely on you covering threads first, 
  but the next assignment after that could be either multiprogramming,
  the file system, or network support.  The only other constraint is that
  the virtual memory stuff relies on the user programming assignment
  being completed [NOTE however that we provide no code for the virtual
  memory assignment, so it could be easily folded into the userprog 
  assignment.]

  Also, the userprog and vm assignments rely on there being a file system
  to fetch executables and to serve as backing store for virtual memory pages.
  A "stub" version of the file system is provided to allow these assignments
  to be done first; the stub version is not needed if file systems
  are covered before user programming and virtual memory.

4. More extensive options with respect to the sample assignments.
  I have now three semesters of experience in teaching with Nachos.
  My assignments have varied slightly from semester to semester, and
  I have now compiled all of these versions into the sample assignments
  [with comments as to which portions I assigned in any given semester].
  The expectation is that you will subset the portion that you find most
  interesting; if you have suggestions for what I might include in the
  sample assignments, I would be happy to hear them.  Hopefully, from 
  this point on, any changes to the sample assignments will only be to add 
  further options.

  Over the long term, it seems to me we will each need to vary the
  assignments, to prevent widespread sharing of solution sets.

5. Support for a software-loaded Translation Lookaside Buffer.  This
  can be disabled (turning the machine simulation back to using simple
  linear page tables) for those who want to avoid the added complexity,
  but it is a feature of many modern architectures, and I think it
  is a good illustration of caching issues.  This is the only substantive
  change for this version.

  One advantage is that it allows a *lot* more flexibility in the 
  VM assignment -- for instance, a student could build a flat one-level 
  page table, segmentation plus paging, an inverted page table, etc.  
  This is all without modifying the hardware emulation.  Also, this could 
  also lead to issues such as shared memory segments between address spaces,
  which couldn't be supported in the current model.

  One consequence is that there are now a new object code format for
  Nachos user programs.  The standard UNIX format, COFF, is way too 
  complicated.  I have a simplified format, NOFF (Nachos Object Format),
  which simply identifies the code, data, and bss segments.  By default, 
  these segments are concatenated together (as in earlier versions of
  Nachos), beginning at location 0, but with the software loaded TLB,
  you have the flexibility to do something smarter.

  The converter from COFF to NOFF has been ported to run on all of the
  supported machines.


Future plans:

1. Known bugs 
    a. Nachos has a memory leak that causes it to increase its 
       virtual memory size over time, even if Nachos is not doing anything. 

2. Planned ports (other suggestions welcome):
    a. M/S Windows (somewhere between Jan and June 94)
    b. Macintosh (ditto)
    c. DEC Alpha (as soon as it gets a reliable g++)

3. Nachos user's guide and roadmap (not under development yet, so 
     definite target date.  Maybe end of summer 94)
  This would come in two parts -- first, a student guide that
  would walk students through the baseline code, explaining how
  the system works, and also to explain a bit of the underlying machine 
  emulation.  At Berkeley, we devote about an hour per week in section 
  to going through the code, but it would be helpful (particularly for 
  those schools without discussion sections) to have this written down.
  I've found in all three semesters I've taught the course that students really 
  do end up repeating many of the same questions.

  The second part would be an instructor's guide -- how do you
  get Nachos up and running on various systems, how the internals
  of the machine emulation work, how much time each of the assignments
  takes, etc.  

  The result would replace the existing sample assignments with 
  something more helpful. 
  (The downside is that some parts of Nachos build on other parts,
  so I have to be clear about these dependencies.)

4. New development -- this is in semi-priority order.

a. Modify the network simulation to be performance accurate, by
   using Chandy-Misra conservative simulation techniques to
   keep the clocks on each simulated Nachos machine in sync.  
   I have a prototype implementation of this, so this isn't all that 
   difficult.  I'll make sure to leave an option to disable this, to go back
   to the way the simulation works now, for backward compatibility.

b. Modify the file system to do write ahead logging for reliability.
   I talk about transactions in my class, and having example code
   would be really useful, at least for me.  Again, I have a prototype 
   implementation of this, and I'll make sure that it can be disabled.

c. Write an RPC stub generator (actually, simplify the one used in Mach,
   and convert it to generate Nachos network messages).  I think
   the students would get a lot out of seeing a working RPC system,
   and I think I can do this in a way that would be simple enough
   for most students to easily understand.  As it stands, I have
   the feeling most of my students don't understand the mechanics of
   setting up an RPC connection, which at present, I can only describe
   verbally.

   At first, I'm likely to do only a C-to-C stub generator, rather
   than a C++ stub generator.  Although the latter would obviously fit 
   into Nachos better, it's also harder!

d. Modify Nachos to insert interrupts at arbitrary points in the code.
   Currently, interrupts (such as timer expiring) only occur when 
   Nachos is executing user-level code, or when the Nachos kernel calls
   the enable interrupt routine.  A different (better?) approach would
   be to check for interrupts on every procedure entry within the Nachos
   kernel; we could do this by modifying the compiler-inserted "mcount" 
   routine for performance profiling.

Again, comments on how to improve Nachos are always welcome.

Tom Anderson
tea@cs.berkeley.edu
nachos@cs.berkeley.edu



Building a gcc cross-compiler
-----------------------------
The gcc distribution has fairly good documentation on how to do this,
but since I walked through it, I figured I would just give you a recipe.
The following works from the SPARC to the DEC MIPS; if you want a 
cross-compiler to a different platform (eg, the HP Snakes), you'll need
to just alter this procedure slightly.

NOTE: we don't need the full cross-compiled environment.  In particular,
Nachos user programs include none of the standard UNIX library or system
call stubs, and it assumes its own crt.s (assembly language assist for
starting a program running).  This makes this significantly simpler,
and it vastly reduces the size of (and overall simplifies) the resulting 
object code.

# To build a cross-compiler using the gnu tools:

# grab the tools via anonymous ftp
% ftp prep.ai.mit.edu

 ftp> cd /pub/gnu
# REMEMBER TO SET BINARY MODE
 ftp> binary
# get gcc, binutils, gas -- these or later versions
 ftp> get gcc-2.4.5.tar.gz	
 ftp> get binutils-2.2.1.tar.gz
 ftp> get gas-2.1.1.tar.gz
 ftp> quit

# gunzip decompresses the tar files
# you can also get gunzip from prep.ai.mit.edu
% gunzip *

# where the executables are to go, usually /usr/local
% setenv gccLocal /usr/local 	

# de-tar the files
% tar -xf gas-2.1.1.tar
% tar -xf binutils-2.2.1.tar
% tar -xf gcc-2.4.5.tar
% mkdir tar
% mv *.tar tar

# build gas and binutils first
% cd gas* 

% ./configure --host=sparc-sun-sunos4.1.3 --target=decstation-ultrix 
	--prefix $gccLocal

% make

% make install

% cd ../bin* 

% ./configure --host=sparc-sun-sunos4.1.3 --target=decstation-ultrix 
	--prefix $gccLocal

% make

% make install

% cd ../gcc*

% ./configure --host=sparc-sun-sunos4.1.3 --target=decstation-ultrix
	--with-gnu-as --with-gnu-ld --prefix $gccLocal
   	--local-prefix $gccLocal

# Afer building the cross-compiler, the Makefile is going to try to
# use it to build a set of libraries, and a couple test cases.
# Unfortunately, the libraries depend on UNIX headers (such as stdio.h).
#
# Since Nachos user programs don't need these headers (they'd be
# wrong anyway, since Nachos doesn't support the standard UNIX syscall
# interface), we need to fake out the Makefile.

# create a dummy lib files, to keep make happy 
% ar r libgcc.a /dev/null
% ar r libgcc2.a /dev/null

# Delete the following lines from the Makefile
#     ENQUIRE = enquire
#     CROSS_TEST = cross-test
% vi Makefile

% make LANGUAGES=c

# at this point you may get an error building libgcc2; ignore it and proceed.

% make install LANGUAGES=c

# at this point, the cross-compiler and subsidiary tools
# are now installed, in $gccLocal/decstation-ultrix/bin

# one last thing --
#   you need to edit nachos/code/test/Makefile to use the
#   gcc cross-compilation tools you have just built, instead of using 
#   normal gcc.
