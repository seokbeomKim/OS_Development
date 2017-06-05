#ifndef __TASK_H__
#define __TASK_H__

#include <DataType/Types.h>

// SS, RSP, RFLAGS, CS, RIP + Registers saved in ISR
#define TASK_REGISTER_COUNT (5 + 19)	
#define TASK_REGISTER_SIZE	8

// Register Offset
#define OFFSET_TASKREG_GS	0
#define OFFSET_TASKREG_FS	1
#define OFFSET_TASKREG_ES	2
#define OFFSET_TASKREG_DS	3
#define OFFSET_TASKREG_R5	4
#define OFFSET_TASKREG_R14	5
#define OFFSET_TASKREG_R13	6
#define OFFSET_TASKREG_R12	7
#define OFFSET_TASKREG_R11	8
#define OFFSET_TASKREG_R10	9
#define OFFSET_TASKREG_R9	10
#define OFFSET_TASKREG_R8	11
#define OFFSET_TASKREG_RSI	12
#define OFFSET_TASKREG_RDI	13
#define OFFSET_TASKREG_RDX	14
#define OFFSET_TASKREG_RCX	15
#define OFFSET_TASKREG_RBX	16
#define OFFSET_TASKREG_RAX	17
#define OFFSET_TASKREG_RBP	18
#define OFFSET_TASKREG_RIP	19
#define OFFSET_TASKREG_CS	20
#define OFFSET_TASKREG_RFLAGS 21
#define OFFSET_TASKREG_RSP	22
#define	OFFSET_TASKREG_SS	23

typedef struct kContextStruct
{
	QWORD vqRegister[TASK_REGISTER_COUNT];
} CONTEXT;

typedef struct kTaskControlBlock 
{
	CONTEXT	stContext;
	QWORD	qwID;
	QWORD	qwFlags;

	void*	pvStackAddr;
	QWORD	qwStackSize;
} TCB;


// Functions related to managing task
void kSetUpTask(TCB* pstTCB, QWORD qwID, QWORD qwFlags, QWORD qwEntryPointAddress,
	void* pvStackAddr, QWORD qwStackSize);


#endif