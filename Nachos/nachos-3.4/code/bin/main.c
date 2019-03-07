/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#include "copyright.h"

/* MIPS instruction interpreter */

#include <stdio.h>
#include <filehdr.h>
#include <scnhdr.h>
#include <syms.h>
#include <ldfcn.h>
#include "int.h"

static FILE *fp;
static LDFILE *ldptr;
static SCNHDR texthead, rdatahead, datahead, sdatahead, sbsshead, bsshead;

static char filename[1000] = "a.out";	/* default a.out file */
static char self[256];			/* name of invoking program */

char mem[MEMSIZE];		/* main memory. use malloc later */
int TRACE, Traptrace, Regtrace;
int NROWS=64, ASSOC=1, LINESIZE=4, RAND=0, LRD=0;

extern char *strcpy();

main(argc, argv)
int argc;
char *argv[];
{
	register char *s;
	char *fakeargv[3];

	strcpy(self, argv[0]);
	while  ( argc > 1  &&  argv[1][0] == '-' )
	{
		--argc; ++argv;
		for  ( s=argv[0]+1; *s != '\0'; ++s )
			switch  ( *s )
			{
				case 't':  TRACE = 1; break;
				case 'T':  Traptrace = 1; break;
				case 'r':  Regtrace = 1; break;
				case 'm':
					NROWS = atoi(*++argv);
					ASSOC = atoi(*++argv);
					LINESIZE = atoi(*++argv);
					RAND = ((*++argv)[0] == 'r');
					LRD = ((*argv)[0] == 'l')
					   && ((*argv)[1] == 'r')
					   && ((*argv)[2] == 'd');
					argc -= 4;
					break;
			}
	}

	if (argc >= 2)
		strcpy(filename, argv[1]);
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "%s: Could not open '%s'\n", self, filename);
		exit(0);
	}
	fclose(fp);
	load_program(filename);
	if  ( argv[1] == NULL )
	{
		fakeargv[1] = "a.out";
		fakeargv[2] = NULL;
		argv = fakeargv;
		++argc;
	}
	runprogram(memoffset, argc-1, argv+1); /* where things normally start */
}

char *string(s)
char *s;
{
	char *p;
	extern char *malloc();

	p = malloc((unsigned) strlen(s)+1);
	strcpy(p, s);
	return p;
}

load_program(filename)
char *filename;
{
	register int pc, i, j, strindex, stl;
	char str[1111];
	int rc1, rc2;

	ldptr = ldopen(filename, NULL);
	if  ( ldptr == NULL )
	{
		fprintf(stderr, "%s: Load read error on %s\n", self, filename);
		exit(0);
	}
	if  ( TYPE(ldptr) != 0x162 )
	{
		fprintf(stderr,
			"big-endian object file (little-endian interp)\n");
		exit(0);
	}

#define LOADSECTION(head)	\
	if  ( head.s_scnptr != 0 ) \
	{ \
		/* printf("loading %s\n", head.s_name); /* */ \
		pc = head.s_vaddr; \
		FSEEK(ldptr, head.s_scnptr, 0); \
		for  ( i=0; i<head.s_size; ++i ) \
			*(char *) ((mem-memoffset)+pc++) = getc(fp); \
		if (pc-memoffset >= MEMSIZE) \
			{ printf("MEMSIZE too small. Fix and recompile.\n"); \
			exit(1); } \
	}

	if  ( ldnshread(ldptr, ".text", &texthead) != 1 )
		printf("text section header missing\n");
	else
		LOADSECTION(texthead)

	if  ( ldnshread(ldptr, ".rdata", &rdatahead) != 1 )
		printf("rdata section header missing\n");
	else
		LOADSECTION(rdatahead)

	if  ( ldnshread(ldptr, ".data", &datahead) != 1 )
		printf("data section header missing\n");
	else
		LOADSECTION(datahead)

	if  ( ldnshread(ldptr, ".sdata", &sdatahead) != 1 )
		printf("sdata section header missing\n");
	else
		LOADSECTION(sdatahead)

	if  ( ldnshread(ldptr, ".sbss", &sbsshead) != 1 )
		printf("sbss section header missing\n");
	else
		LOADSECTION(sbsshead)

	if  ( ldnshread(ldptr, ".bss", &bsshead) != 1 )
		printf("bss section header missing\n");
	else
		LOADSECTION(bsshead)

	/* BSS is already zeroed (statically-allocated mem) */
	/* this version ignores relocation info */
}


int *m_alloc(n)
int n;
{
	extern char *malloc();

	return (int *) (int) malloc((unsigned) n);
}

