#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Types64.h"

// GDT 관련 매크로
#define	GDT_TYPE_CODE			0x0A
#define	GDT_TYPE_DATA			0x02
#define	GDT_TYPE_TSS			0x09
#define	GDT_FLAGS_LOWER_S		0x10
#define	GDT_FLAGS_LOWER_DPL0	0x00
#define	GDT_FLAGS_LOWER_DPL1	0x20
#define	GDT_FLAGS_LOWER_DPL2	0x40
#define	GDT_FLAGS_LOWER_DPL3	0x60
#define	GDT_FLAGS_LOWER_P		0x80
#define	GDT_FLAGS_UPPER_L		0x20
#define	GDT_FLAGS_UPPER_DB		0x40
#define	GDT_FLAGS_UPPER_G		0x80	// granularity

// GDT LOWER 비트 부분 설정 - Code/Data/TSS, DPL0, Present
#define	GDT_FLAGS_LOWER_KERNEL_CODE	(GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define	GDT_FLAGS_LOWER_KERNEL_DATA	(GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_TSS 		(GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define	GDT_FLAGS_LOWER_USER_CODE	(GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)
#define	GDT_FLAGS_LOWER_USER_DATA	(GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)

// GDT UPPER 비트 부분 설정
#define GDT_FLAGS_UPPER_CODE		(GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define	GDT_FLAGS_UPPER_DATA		(GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define	GDT_FLAGS_UPPER_TSS			(GDT_FLAGS_UPPER_G)

#define	GDT_KERNEL_CODE_SEGMENT		0x08
#define	GDT_KERNEL_DATA_SEGMENT		0x10
#define	GDT_TSS_SEGMENT				0x18

#define	GDTR_START_ADDRESS			0x142000
#define	GDT_MAX_ENTRY8_COUNT		3
#define	GDT_MAX_ENTRY16_COUNT		1
#define	GDT_TABLE_SIZE				(sizeof(GDT_ENTRY8) * GDT_MAX_ENTRY8_COUNT) + (sizeof(GDT_ENTRY16) * GDT_MAX_ENTRY16_COUNT)
#define	TSS_SEGMENT_SIZE			(sizeof(TSS_SEGMENT))

// IDT
#define	IDT_TYPE_INTERRUPT		0x0E
#define	IDT_TYPE_TRAP			0x0F
#define	IDT_FLAGS_DPL0			0x00
#define	IDT_FLAGS_DPL1			0x20
#define	IDT_FLAGS_DPL2			0x40
#define	IDT_FLAGS_DPL3			0x60
#define	IDT_FLAGS_P				0x80
#define	IDT_FLAGS_IST0			0
#define	IDT_FLAGS_IST1			1

#define	IDT_FLAGS_KERNEL		(IDT_FLAGS_DPL0 | IDT_FLAGS_P)
#define	IDT_FLAGS_USER			(IDT_FLAGS_DPL3 | IDT_FLAGS_P)

#define	IDT_MAX_ENTRY_COUNT		100
#define	IDTR_START_ADDRESS		(GDTR_START_ADDRESS + sizeof(GDTR) + GDT_TABLE_SIZE + TSS_SEGMENT_SIZE)
#define	IDT_START_ADDRESS		(IDTR_START_ADDRESS + sizeof(IDTR))
#define IDT_TABLE_SIZE			(IDT_MAX_ENTRY_COUNT * sizeof(IDT_ENTRY))
#define	IST_START_ADDRESS		0x700000
#define	IST_SIZE				0x100000

#pragma	pack (push, 1)
typedef struct kGDTRStruct {
	WORD	wLimit;
	QWORD	qwBaseAddress;
	WORD	wPading;
	DWORD	dwPading;
} GDTR, IDTR;

typedef struct kGDTREntry8Struct {
	WORD	wLowerLimit;
	WORD	wLowerBaseAddress;
	BYTE	bUpperBaseAddress1;
	BYTE	bTypeAndLowerFlag;
	BYTE	bUpperLimitAndUpperFlag;
	BYTE	bUpperBaseAddress2;
} GDT_ENTRY8;

typedef struct kGDTEntry16Struct {
	WORD	wLowerLimit;
	WORD	wLowerBaseAddress;
	BYTE	bMiddleBaseAddress1;
	BYTE	bTypeAndLowerFlag;
	BYTE	bUpperLimitAndUpperFlag;
	BYTE	bMiddleBaseAddress2;
	DWORD	dwUpperBaseAddress;
	DWORD	dwReserved;
} GDT_ENTRY16;

typedef struct kTSSDataStruct {
	DWORD	dwReserved1;
	QWORD	qwRsp[3];
	QWORD	qwReserved2;
	QWORD	qwIST[7];
	QWORD	qwReserved3;
	WORD	wReserved;
	WORD	wIOMapBaseAddress;
} TSS_SEGMENT;

typedef	struct kIDTEntryStruct {
	WORD	wLowerBaseAddress;
	WORD	wSegmentSelector;
	BYTE	bIST;
	BYTE	bTypeAndFlags;
	WORD	wMiddleBaseAddress;
	DWORD	dwUpperBaseAddress;
	DWORD	dwReserved;
} IDT_ENTRY;
#pragma pack (pop)

void	kInitializeGDTTableAndTSS(void);
void	kSetGDTEntry8(GDT_ENTRY8* pstEntry, DWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType);
void	kSetGDTEntry16(GDT_ENTRY16* pstEntry, QWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType);
void	kInitializeTSSSegment(TSS_SEGMENT* pstTSS);
void	kInitializeIDTTables(void);
void	kSetIDTEntry(IDT_ENTRY* pstEntry, void* pvHandler, WORD wSelector, BYTE bIST, BYTE bFlags, BYTE bType);
void	kDummyHandler(void);

#endif
