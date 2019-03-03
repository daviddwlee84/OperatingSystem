/* coff2noff.c 
 *
 * This program reads in a COFF format file, and outputs a NOFF format file.
 * The NOFF format is essentially just a simpler version of the COFF file,
 * recording where each segment is in the NOFF file, and where it is to
 * go in the virtual address space.
 * 
 * Assumes coff file is linked with either
 *	gld with -N -Ttext 0 
 * 	ld with  -N -T 0
 * to make sure the object file has no shared text.
 *
 * Also assumes that the COFF file has at most 3 segments:
 *	.text	-- read-only executable instructions 
 *	.data	-- initialized data
 *	.bss/.sbss -- uninitialized data (should be zero'd on program startup)
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#define MAIN
#include "copyright.h" 
#undef MAIN

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>

#include "coff.h"
#include "noff.h"

/* Routines for converting words and short words to and from the
 * simulated machine's format of little endian.  These end up
 * being NOPs when the host machine is little endian.
 */

unsigned int
WordToHost(unsigned int word) {
#ifdef HOST_IS_BIG_ENDIAN
	 register unsigned long result;
	 result = (word >> 24) & 0x000000ff;
	 result |= (word >> 8) & 0x0000ff00;
	 result |= (word << 8) & 0x00ff0000;
	 result |= (word << 24) & 0xff000000;
	 return result;
#else 
	 return word;
#endif /* HOST_IS_BIG_ENDIAN */
}

unsigned short
ShortToHost(unsigned short shortword) {
#if HOST_IS_BIG_ENDIAN
	 register unsigned short result;
	 result = (shortword << 8) & 0xff00;
	 result |= (shortword >> 8) & 0x00ff;
	 return result;
#else 
	 return shortword;
#endif /* HOST_IS_BIG_ENDIAN */
}

#define ReadStruct(f,s) 	Read(f,(char *)&s,sizeof(s))

extern char *malloc();
char *noffFileName = NULL;

/* read and check for error */
void Read(int fd, char *buf, int nBytes)
{
    if (read(fd, buf, nBytes) != nBytes) {
        fprintf(stderr, "File is too short\n");
	unlink(noffFileName);
	exit(1);
    }
}

/* write and check for error */
void Write(int fd, char *buf, int nBytes)
{
    if (write(fd, buf, nBytes) != nBytes) {
	fprintf(stderr, "Unable to write file\n");
	unlink(noffFileName);
	exit(1);
    }
}

main (int argc, char **argv)
{
    int fdIn, fdOut, numsections, i, inNoffFile;
    struct filehdr fileh;
    struct aouthdr systemh;
    struct scnhdr *sections;
    char *buffer;
    NoffHeader noffH;

    if (argc < 2) {
	fprintf(stderr, "Usage: %s <coffFileName> <noffFileName>\n", argv[0]);
	exit(1);
    }
    
/* open the COFF file (input) */
    fdIn = open(argv[1], O_RDONLY, 0);
    if (fdIn == -1) {
	perror(argv[1]);
	exit(1);
    }

/* open the NOFF file (output) */
    fdOut = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC , 0666);
    if (fdIn == -1) {
	perror(argv[2]);
	exit(1);
    }
    noffFileName = argv[2];
    
/* Read in the file header and check the magic number. */
    ReadStruct(fdIn,fileh);
    fileh.f_magic = ShortToHost(fileh.f_magic);
    fileh.f_nscns = ShortToHost(fileh.f_nscns); 
    if (fileh.f_magic != MIPSELMAGIC) {
	fprintf(stderr, "File is not a MIPSEL COFF file\n");
        unlink(noffFileName);
	exit(1);
    }
    
/* Read in the system header and check the magic number */
    ReadStruct(fdIn,systemh);
    systemh.magic = ShortToHost(systemh.magic);
    if (systemh.magic != OMAGIC) {
	fprintf(stderr, "File is not a OMAGIC file\n");
        unlink(noffFileName);
	exit(1);
    }
    
/* Read in the section headers. */
    numsections = fileh.f_nscns;
    printf("numsections %d \n",numsections);
    sections = (struct scnhdr *)malloc(numsections * sizeof(struct scnhdr));
    Read(fdIn, (char *) sections, numsections * sizeof(struct scnhdr));

   for (i = 0; i < numsections; i++) {
     sections[i].s_paddr =  WordToHost(sections[i].s_paddr);
     sections[i].s_size = WordToHost(sections[i].s_size);
     sections[i].s_scnptr = WordToHost(sections[i].s_scnptr);
   }

 /* initialize the NOFF header, in case not all the segments are defined
  * in the COFF file
  */
    noffH.noffMagic = NOFFMAGIC;
    noffH.code.size = 0;
    noffH.initData.size = 0;
    noffH.uninitData.size = 0;

 /* Copy the segments in */
    inNoffFile = sizeof(NoffHeader);
    lseek(fdOut, inNoffFile, 0);
    printf("Loading %d sections:\n", numsections);
    for (i = 0; i < numsections; i++) {
	printf("\t\"%s\", filepos 0x%x, mempos 0x%x, size 0x%x\n",
	      sections[i].s_name, sections[i].s_scnptr,
	      sections[i].s_paddr, sections[i].s_size);
	if (sections[i].s_size == 0) {
		/* do nothing! */	
	} else if (!strcmp(sections[i].s_name, ".text")) {
	    noffH.code.virtualAddr = sections[i].s_paddr;
	    noffH.code.inFileAddr = inNoffFile;
	    noffH.code.size = sections[i].s_size;
    	    lseek(fdIn, sections[i].s_scnptr, 0);
    	    buffer = malloc(sections[i].s_size);
    	    Read(fdIn, buffer, sections[i].s_size);
    	    Write(fdOut, buffer, sections[i].s_size);
    	    free(buffer);
	    inNoffFile += sections[i].s_size;
 	} else if (!strcmp(sections[i].s_name, ".data")
	  		|| !strcmp(sections[i].s_name, ".rdata")) {
  	    /* need to check if we have both .data and .rdata 
	     *  -- make sure one or the other is empty! */ 
	    if (noffH.initData.size != 0) {
	        fprintf(stderr, "Can't handle both data and rdata\n");
	        unlink(noffFileName);
	        exit(1);
	    }
	    noffH.initData.virtualAddr = sections[i].s_paddr;
	    noffH.initData.inFileAddr = inNoffFile;
	    noffH.initData.size = sections[i].s_size;
    	    lseek(fdIn, sections[i].s_scnptr, 0);
    	    buffer = malloc(sections[i].s_size);
    	    Read(fdIn, buffer, sections[i].s_size);
    	    Write(fdOut, buffer, sections[i].s_size);
    	    free(buffer);
	    inNoffFile += sections[i].s_size;
	} else if (!strcmp(sections[i].s_name, ".bss") ||
			!strcmp(sections[i].s_name, ".sbss")) {
  	    /* need to check if we have both .bss and .sbss -- make sure they 
	     * are contiguous
	     */
	    if (noffH.uninitData.size != 0) {
	        if (sections[i].s_paddr == (noffH.uninitData.virtualAddr +
	        				noffH.uninitData.size)) {
		    fprintf(stderr, "Can't handle both bss and sbss\n");
		    unlink(noffFileName);
		    exit(1);
		}
	        noffH.uninitData.size += sections[i].s_size;
	    } else {
	        noffH.uninitData.virtualAddr = sections[i].s_paddr;
	        noffH.uninitData.size = sections[i].s_size;
	    }
	    /* we don't need to copy the uninitialized data! */
	} else {
	    fprintf(stderr, "Unknown segment type: %s\n", sections[i].s_name);
            unlink(noffFileName);
	    exit(1);
	}
    }
    lseek(fdOut, 0, 0);
    Write(fdOut, (char *)&noffH, sizeof(NoffHeader));
    close(fdIn);
    close(fdOut);
    exit(0);
}
