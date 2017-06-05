#include <Scheduler/Task.h>
#include <Init/Descriptor.h>
#include <Include/Mem.h>

void kSetUpTask(TCB* pstTCB, QWORD qwID, QWORD qwFlags, QWORD qwEntryPointAddress,
	void* pvStackAddr, QWORD qwStackSize)
{
	// Initialize context
	kMemSet(&pstTCB->stContext, 0, sizeof(pstTCB->stContext.vqRegister));

	// Initialize stack registers
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_RSP] = (QWORD)pvStackAddr + qwStackSize;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_RBP] = (QWORD)pvStackAddr + qwStackSize;

	// Initialize segment descriptors
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_CS] = GDT_KERNEL_CODE_SEGMENT;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_DS] = GDT_KERNEL_DATA_SEGMENT;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_ES] = GDT_KERNEL_DATA_SEGMENT;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_FS] = GDT_KERNEL_DATA_SEGMENT;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_GS] = GDT_KERNEL_DATA_SEGMENT;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_SS] = GDT_KERNEL_DATA_SEGMENT;

	// Initialize RIP register and interrupt flag
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_RIP] = qwEntryPointAddress;
	pstTCB->stContext.vqRegister[OFFSET_TASKREG_RFLAGS] |= 0x0200;	// Enable interrupt by setting 1 IF bits of RFLAGS

	pstTCB->qwID = qwID;
	pstTCB->pvStackAddr = pvStackAddr;
	pstTCB->qwStackSize = qwStackSize;
	pstTCB->qwFlags = qwFlags;
}
