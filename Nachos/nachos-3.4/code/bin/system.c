/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#include "copyright.h"
#include <stdio.h>
#include <syscall.h>
#include "int.h"

extern int Reg[];
extern char mem[];
extern int Traptrace;

char *u_to_int_addr();

/* handle system calls */
system_break()
{
	if  ( Traptrace )
		printf("**breakpoint ");
	system_trap();
}

system_trap()
{
	int o0, o1, o2;		/* user out register values */
	int syscallno;
	extern long lseek();

	if  ( Traptrace )
	{
		printf("**System call %d\n", Reg[2]);
		dump_reg();
	}

/*	if (Reg[1] == 0)
/*	{			/* SYS_indir */
/*		syscallno = Reg[8];	/* out reg 0 */
/*		o0 = Reg[9];
/*		o1 = Reg[10];
/*		o2 = Reg[11];
/*	}
/*	else	/* */
	{
		syscallno = Reg[2];
		o0 = Reg[4];
		o1 = Reg[5];
		o2 = Reg[6];
	}

	switch (syscallno)
	{
		case SYS_exit:	/*1*/
			printstatistics();
			fflush(stdout);
			exit(0);
			break;
		case SYS_read:	/*3*/
			Reg[1] = 
				read(u_to_int_fd(o0), u_to_int_addr(o1), o2);
			break;
		case SYS_write:	/*4*/
			Reg[1] = 
				write(u_to_int_fd(o0), u_to_int_addr(o1), o2);
			break;

		case SYS_open:	/*5*/
			Reg[1] = open(u_to_int_addr(o0), o1, o2); /* */
			break;

		case SYS_close:	/*6*/
			Reg[1] = 0;	/* hack */
			break;

		case 17:	/* 17 */
			/* old sbreak. where did it go? */
			Reg[1] = ((o0 / 8192) + 1) * 8192;
			break;

		case SYS_lseek:	/*19*/
			Reg[1] = (int) lseek(u_to_int_fd(o0), (long) o1, o2);
			break;

		case SYS_ioctl:/* 54 */
			{	/* copied from sas -- I don't understand yet. */
				/* see dave weaver */
#define	IOCPARM_MASK	0x7f	/* parameters must be < 128 bytes */
				int size = (o1 >> 16) & IOCPARM_MASK;
				char ioctl_group = (o1 >> 8) & 0x00ff;
				if ((ioctl_group == 't') && (size == 8))
				{
					size = 6;
					o1 = (o1 & ~((IOCPARM_MASK << 16))) 
					   | (size << 16);
				}
			}
			Reg[1] = ioctl(u_to_int_fd(o0),o1,u_to_int_addr(o2));
			Reg[1] = 0;	/* hack */
			break;

		case SYS_fstat:		/* 62 */
			Reg[1] = fstat(o1, o2);
			break;

		case SYS_getpagesize:	/* 64 */
			Reg[1] = getpagesize();
			break;

		default:
			printf("Unknown System call %d\n", syscallno);
			if  ( ! Traptrace )
				dump_reg();
			exit(2);
			break;
	}
	if  ( Traptrace )
	{
		printf("**Afterwards:\n");
		dump_reg();
	}
}

char *u_to_int_addr(ptr)
int ptr;
{	/* convert a user pointer to the real address   */
	/* used in the interpreter			*/

	 return ((char *) ((int) mem - memoffset  + ptr));
}

u_to_int_fd(fd)
{
	if (fd > 2)
	{
		/*
		printf("No general file descriptors yet\n");
		exit(2);
		*/
	}
	return (fd);		/* assume we can handle it for now */
}
