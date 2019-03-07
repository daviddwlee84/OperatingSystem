/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#include "copyright.h"


/* normal opcodes */

#define I_SPECIAL	000
#define I_BCOND		001
#define I_J		002
#define I_JAL		003
#define I_BEQ		004
#define I_BNE		005
#define I_BLEZ		006
#define I_BGTZ		007
#define I_ADDI		010
#define I_ADDIU		011
#define I_SLTI		012
#define I_SLTIU		013
#define I_ANDI		014
#define I_ORI		015
#define I_XORI		016
#define I_LUI		017
#define I_COP0		020
#define I_COP1		021
#define I_COP2		022
#define I_COP3		023

#define I_LB		040
#define I_LH		041
#define I_LWL		042
#define I_LW		043
#define I_LBU		044
#define I_LHU		045
#define I_LWR		046

#define I_SB		050
#define I_SH		051
#define I_SWL		052
#define I_SW		053

#define I_SWR		056

#define I_LWC0		060
#define I_LWC1		061
#define I_LWC2		062
#define I_LWC3		063

#define I_SWC0		070
#define I_SWC1		071
#define I_SWC2		072
#define I_SWC3		073

/* special opcodes */

#define I_SLL		000

#define I_SRL		002
#define I_SRA		003
#define I_SLLV		004

#define I_SRLV		006
#define I_SRAV		007
#define I_JR		010
#define I_JALR		011

#define I_SYSCALL	014
#define I_BREAK		015

#define I_MFHI		020
#define I_MTHI		021
#define I_MFLO		022
#define I_MTLO		023

#define I_MULT		030
#define I_MULTU		031
#define I_DIV		032
#define I_DIVU		033

#define I_ADD		040
#define I_ADDU		041
#define I_SUB		042
#define I_SUBU		043
#define I_AND		044
#define I_OR		045
#define I_XOR		046
#define I_NOR		047

#define I_SLT		052
#define I_SLTU		053

/* bcond opcodes */


#define I_BLTZ		000
#define I_BGEZ		001

#define I_BLTZAL	020
#define I_BGEZAL	021

/* whole instructions */

#define I_NOP           000
