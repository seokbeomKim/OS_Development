#include <Init/Descriptor.h>
#include <Include/Mem.h>
#include <Asm/ISR.h>

void kInitializeGDTTableAndTSS(void) {
	GDTR*			pstGDTR;
	GDT_ENTRY8*		pstEntry;
	TSS_SEGMENT*	pstTSS;
	int	i;

	// GDTR
	pstGDTR		= (GDTR*) GDTR_START_ADDRESS;
	pstEntry	= (GDT_ENTRY8*) (GDTR_START_ADDRESS + sizeof(GDTR));
	pstGDTR->wLimit			= GDT_TABLE_SIZE - 1;
	pstGDTR->qwBaseAddress	= (QWORD) pstEntry;

	pstTSS		= (TSS_SEGMENT*) ((QWORD) pstEntry + GDT_TABLE_SIZE);
	kSetGDTEntry8( &(pstEntry[0]), 0, 0, 0, 0, 0);
	kSetGDTEntry8( &(pstEntry[1]), 0, 0xFFFFF,
			GDT_FLAGS_UPPER_CODE, GDT_FLAGS_LOWER_KERNEL_CODE, GDT_TYPE_CODE);
	kSetGDTEntry8( &(pstEntry[2]), 0, 0xFFFFF,
			GDT_FLAGS_UPPER_DATA, GDT_FLAGS_LOWER_KERNEL_DATA, GDT_TYPE_DATA);
	kSetGDTEntry16( (GDT_ENTRY16 *) &(pstEntry[3]), (QWORD)pstTSS, sizeof(TSS_SEGMENT) - 1,
			GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);

	kInitializeTSSSegment(pstTSS);
}

void kSetGDTEntry8(GDT_ENTRY8* pstEntry, DWORD dwBaseAddress,
		DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType) {
	pstEntry->wLowerLimit			= dwLimit & 0xFFFF;
	pstEntry->wLowerBaseAddress		= dwBaseAddress & 0xFFFF;
	pstEntry->bUpperBaseAddress1	= (dwBaseAddress >> 16) & 0xFF;
	pstEntry->bTypeAndLowerFlag		= bLowerFlags | bType;
	pstEntry->bUpperLimitAndUpperFlag	= ((dwLimit >> 16) & 0xFF) | bUpperFlags;
	pstEntry->bUpperBaseAddress2	= (dwBaseAddress >> 24) & 0xFF;
}

void kSetGDTEntry16(GDT_ENTRY16 *pstEntry, QWORD qwBaseAddress,
		DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType) {
	pstEntry->wLowerLimit			= dwLimit & 0xFFFF;
	pstEntry->wLowerBaseAddress		= qwBaseAddress & 0xFFFF;
	pstEntry->bMiddleBaseAddress1	= (qwBaseAddress >> 16) & 0xFF;
	pstEntry->bTypeAndLowerFlag		= bLowerFlags | bType;
	pstEntry->bUpperLimitAndUpperFlag = ((dwLimit >> 16) & 0xFF) | bUpperFlags;
	pstEntry->bMiddleBaseAddress2	= (qwBaseAddress >> 24) & 0xFF;
	pstEntry->dwUpperBaseAddress	= qwBaseAddress >> 32;
	pstEntry->dwReserved			= 0;
}

void kInitializeTSSSegment(TSS_SEGMENT* pstTSS) {
	kMemSet(pstTSS, 0, sizeof(TSS_SEGMENT));
	pstTSS->qwIST[0] = IST_START_ADDRESS + IST_SIZE;
	pstTSS->wIOMapBaseAddress = 0xFFFF;
}

void kInitializeIDTTables(void) {
	IDTR*		pstIDTR;
	IDT_ENTRY*	pstEntry;
	int i;

	pstIDTR		= (IDTR *) IDTR_START_ADDRESS;
	pstEntry	= (IDT_ENTRY *) (IDTR_START_ADDRESS + sizeof(IDTR));
	pstIDTR->qwBaseAddress	= (QWORD) pstEntry;
	pstIDTR->wLimit			= IDT_TABLE_SIZE - 1;

	// Register Exception ISR
	kSetIDTEntry ( &(pstEntry[0]), kISR_DivideError, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[1]), kISR_Debugger, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[2]), kISR_NMI, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[3]), kISR_BreakPoint, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[4]), kISR_OverFlow, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[5]), kISR_Bounds, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[6]), kISR_InvalidOpcode, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[7]), kISR_CoprocessorNotAvailable, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[8]), kISR_DoubleFault, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[9]), kISR_CoprocessorSegmentOverrun, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[10]), kISR_InvalidTaskStateSegment, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[11]), kISR_SegmentNotPresent, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[12]), kISR_StackFault, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[13]), kISR_GeneralProtection, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[14]), kISR_PageFault, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[15]), kISR15, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[16]), kISR_MathFault, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[17]), kISR_AlignmentCheck, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[18]), kISR_MachineCheck, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[19]), kISR_SIMD, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
//	kSetIDTEntry ( &(pstEntry[20]), kISR_VirtualizationException, 0x08, IDT_FLAGS_IST1,
//				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[20]), kISR_ETCException, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
//	kSetIDTEntry ( &(pstEntry[21]), kISR_ControlProtectionException, 0x08, IDT_FLAGS_IST1,
//				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);

	// TODO index시작 21에서 22로 추후에 변경할 것
	for (i = 21; i < 32; i++) {
		kSetIDTEntry ( &(pstEntry[i]), kISR_ETCException, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	}

	// Register ISR for Interrupt
	kSetIDTEntry ( &(pstEntry[32]), kISR_Timer, 0x08, IDT_FLAGS_IST1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[33]), kISR_Keyboard, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[34]), kISR_SlavePIC, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[35]), kISR_SerialPort2, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[36]), kISR_SerialPort1, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[37]), kISR_ParallelPort2, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[38]), kISR_FloppyDiskController, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[39]), kISR_ParallelPort1, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[40]), kISR_RTC, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[41]), kISR41, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[42]), kISR42, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[43]), kISR43, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[44]), kISR_Mouse, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[45]), kISR_Coprocessor, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[46]), kISR_HardDisk1, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry ( &(pstEntry[47]), kISR_HardDisk2, 0x08, IDT_FLAGS_IST1,
					IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);

	for (i = 48; i < IDT_MAX_ENTRY_COUNT; i++) {
		kSetIDTEntry ( &(pstEntry[i]), kISR_ETC, 0x08, IDT_FLAGS_IST1,
						IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	}
}

void kSetIDTEntry(IDT_ENTRY* pstEntry, void* pvHandler, WORD wSelector,
		BYTE bIST, BYTE bFlags, BYTE bType) {
	pstEntry->wLowerBaseAddress = (QWORD) pvHandler & 0xFFFF;
	pstEntry->wSegmentSelector = wSelector;
	pstEntry->bIST = bIST & 0x3;
	pstEntry->bTypeAndFlags = bType | bFlags;
	pstEntry->wMiddleBaseAddress = ((QWORD)pvHandler >> 16) & 0xFFFF;
	pstEntry->dwUpperBaseAddress = (QWORD) pvHandler >> 32;
	pstEntry->dwReserved = 0;
}

