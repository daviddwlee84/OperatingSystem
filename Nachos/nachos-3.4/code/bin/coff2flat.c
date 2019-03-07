/*
 Copyright (c) 1992 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

/* This program reads in a COFF format file, and outputs a flat file --
 * the flat file can then be copied directly to virtual memory and executed.
 * In other words, the various pieces of the object code are loaded at 
 * the appropriate offset in the flat file.
 *
 * Assumes coff file compiled with -N -T 0 to make sure it's not shared text.
 */

#define MAIN
#include "copyright.h"
#undef MAIN

#include <filehdr.h>
#include <aouthdr.h>
#include <scnhdr.h>
#include <reloc.h>
#include <syms.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>

/* NOTE -- once you have implemented large files, it's ok to make this bigger! */
#define StackSize      		1024      /* in bytes */
#define ReadStruct(f,s) 	Read(f,(char *)&s,sizeof(s))

extern char *malloc();

/* read and check for error */
void Read(int fd, char *buf, int nBytes)
{
    if (read(fd, buf, nBytes) != nBytes) {
	fprintf(stderr, "File is too short\n");
	exit(1);
    }
}

/* write and check for error */
void Write(int fd, char *buf, int nBytes)
{
    if (write(fd, buf, nBytes) != nBytes) {
	fprintf(stderr, "Unable to write file\n");
	exit(1);
    }
}

/* do the real work */
main (int argc, char **argv)
{
    int fdIn, fdOut, numsections, i, top, tmp;
    struct filehdr fileh;
    struct aouthdr systemh;
    struct scnhdr *sections;
    char *buffer;

    if (argc < 2) {
	fprintf(stderr, "Usage: %s <coffFileName> <flatFileName>\n", argv[0]);
	exit(1);
    }
    
/* open the object file (input) */
    fdIn = open(argv[1], O_RDONLY, 0);
    if (fdIn == -1) {
	perror(argv[1]);
	exit(1);
    }

/* open the flat file (output) */
    fdOut = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0666);
    if (fdIn == -1) {
	perror(argv[2]);
	exit(1);
    }
    
/* Read in the file header and check the magic number. */
    ReadStruct(fdIn,fileh);
    if (fileh.f_magic != MIPSELMAGIC) {
	fprintf(stderr, "File is not a MIPSEL COFF file\n");
	exit(1);
    }
    
/* Read in the system header and check the magic number */
    ReadStruct(fdIn,systemh);
    if (systemh.magic != OMAGIC) {
	fprintf(stderr, "File is not a OMAGIC file\n");
	exit(1);
    }
    
/* Read in the section headers. */
    numsections = fileh.f_nscns;
    sections = (struct scnhdr *)malloc(fileh.f_nscns * sizeof(struct scnhdr));
    Read(fdIn, (char *) sections, fileh.f_nscns * sizeof(struct scnhdr));

 /* Copy the segments in */
    printf("Loading %d sections:\n", fileh.f_nscns);
    for (top = 0, i = 0; i < fileh.f_nscns; i++) {
	printf("\t\"%s\", filepos 0x%x, mempos 0x%x, size 0x%x\n",
	      sections[i].s_name, sections[i].s_scnptr,
	      sections[i].s_paddr, sections[i].s_size);
	if ((sections[i].s_paddr + sections[i].s_size) > top)
	    top = sections[i].s_paddr + sections[i].s_size;
	if (strcmp(sections[i].s_name, ".bss") && /* no need to copy if .bss */
	    	strcmp(sections[i].s_name, ".sbss")) {
	    lseek(fdIn, sections[i].s_scnptr, 0);
	    buffer = malloc(sections[i].s_size);
	    Read(fdIn, buffer, sections[i].s_size);
	    Write(fdOut, buffer, sections[i].s_size);
	    free(buffer);
	}
    }
/* put a blank word at the end, so we know where the end is! */
    printf("Adding stack of size: %d\n", StackSize);
    lseek(fdOut, top + StackSize - 4, 0);
    tmp = 0;
    Write(fdOut, (char *)&tmp, 4);

    close(fdIn);
    close(fdOut);
}
