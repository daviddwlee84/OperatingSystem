/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#include "copyright.h"
#include "instr.h"
#include "encode.h"

#define NULL	0

int sptr;
int longdis = 1;

extern char *normalops[], *specialops[];


char *regstrings[] =
{
"0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
"r10", "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19",
"r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "gp", "sp",
"r30", "r31"
};

#define R(i)	regstrings[i]


dump_ascii(instruction, pc)
int instruction, pc;
{
	int addr;
	char *s;
	int opcode;

	if  ( longdis )  printf("%08x: %08x  ", pc, instruction);
	printf("\t");
	opcode = (unsigned) instruction >> 26;
	if ( instruction == I_NOP) {
	  printf("nop");
	}
	else if  ( opcode == I_SPECIAL )
	{
		opcode = instruction & 0x3f;
		printf("%s\t", specialops[opcode]);

		switch( opcode )
		{

			/* rd,rt,shamt */
			case I_SLL:
			case I_SRL:
			case I_SRA:
				printf("%s,%s,0x%x", 
					R(rd(instruction)),
					R(rt(instruction)),
					shamt(instruction));
				break;

			/* rd,rt,rs */
			case I_SLLV:
			case I_SRLV:
			case I_SRAV:
				printf("%s,%s,%s", 
					R(rd(instruction)),
					R(rt(instruction)),
					R(rs(instruction)));
				break;

			/* rs */
			case I_JR:
			case I_JALR:
			case I_MFLO:
			case I_MTLO:
				printf("%s", R(rs(instruction)));
				break;

			case I_SYSCALL:
			case I_BREAK:
				break;

			/* rd */
			case I_MFHI:
			case I_MTHI:
				printf("%s", R(rd(instruction)));
				break;

			/* rs,rt */
			case I_MULT:
			case I_MULTU:
			case I_DIV:
			case I_DIVU:
				printf("%s,%s", 
					R(rs(instruction)),
					R(rt(instruction)));
				break;

			/* rd,rs,rt */ 
			case I_ADD:
			case I_ADDU:
			case I_SUB:
			case I_SUBU:
			case I_AND:
			case I_OR:
			case I_XOR:
			case I_NOR:
			case I_SLT:
			case I_SLTU:
				printf("%s,%s,%s", 
					R(rd(instruction)),
					R(rs(instruction)),
					R(rt(instruction)));
				break;

		}
	}
	else if  ( opcode == I_BCOND )
	{
		switch ( rt(instruction) )	/* this field encodes the op */
		{
		    case I_BLTZ:
		            printf("bltz");
			    break;
		    case I_BGEZ:
			    printf("bgez");
			    break;
		    case I_BLTZAL:
			    printf("bltzal");
			    break;
		    case I_BGEZAL:
			    printf("bgezal");
			    break;
		    default :
		            printf("BCOND");
		}
		printf("\t%s,%08x",
		       R(rs(instruction)),
		       off16(instruction)+pc+4);
	}
	else
	{
		printf("%s\t", normalops[opcode]);

		switch ( opcode )
		{
			/* 26-bit_target */
			case I_J:
			case I_JAL:
		                printf("%08x",
				       top4(pc)|off26(instruction));
				break;

			/* rs,rt,16-bit_offset */
			case I_BEQ:
			case I_BNE:
		                printf("%s,%s,%08x",
				       R(rt(instruction)),
				       R(rs(instruction)),
				       off16(instruction)+pc+4);
				break;

			/* rt,rs,immediate */
			case I_ADDI:
			case I_ADDIU:
			case I_SLTI:
			case I_SLTIU:
			case I_ANDI:
			case I_ORI:
			case I_XORI:
				printf("%s,%s,0x%x", 
					R(rt(instruction)),
					R(rs(instruction)),
					immed(instruction));
				break;

			 /* rt, immed */
			case I_LUI:
				printf("%s,0x%x", 
					R(rt(instruction)),
					immed(instruction));
				break;

			/* coprocessor garbage */
			case I_COP0:
			case I_COP1:
			case I_COP2:
			case I_COP3:
				break;

			/* rt,offset(rs) */
			case I_LB:
			case I_LH:
			case I_LWL:
			case I_LW:
			case I_LBU:
			case I_LHU:
			case I_LWR:
			case I_SB:
			case I_SH:
			case I_SWL:
			case I_SW:
			case I_SWR:
			case I_LWC0:
			case I_LWC1:
			case I_LWC2:
			case I_LWC3 :
			case I_SWC0:
			case I_SWC1:
			case I_SWC2:
			case I_SWC3:
				printf("%s,0x%x(%s)", 
					R(rt(instruction)),
					immed(instruction),
					R(rs(instruction)));
				break;
		}
	}
}
