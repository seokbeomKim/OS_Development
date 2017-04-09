#include <Sysutils/ConsoleShell.h>
#include <DataType/Types.h>
#include <DeviceDriver/DeviceDriver.h>
#include <Init/Descriptor.h>
#include <Asm/AsmHelper.h>
#include <Testcase/Testcase.h>

void Main(void)
{
	kInitializeConsole(0, 8);
	kPrintf("[KERNEL] ==== Enter to IA-32e mode ====\n");
	kPrintf("[KERNEL] Change old GDT to new one for IA-32e mode\n");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_START_ADDRESS);

	kPrintf("[KERNEL] Set TSS Segment\n");
	kLoadTR(GDT_TSS_SEGMENT);

	kPrintf("[KERNEL] Initialize and set IDT\n");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_START_ADDRESS);

	kPrintf("[KERNEL] Ram Check\n");
	kCheckTotalRAMSize();
	kPrintf(" > Size = %d\n", kGetTotalRAMSize());

	kInitializeKeyboard();
	kPrintf("[KERNEL] Activate Keyboard\n");

	kInitializePIC();
	kPrintf("[KERNEL] Initialize PIC\n");

	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintf("[KERNEL] Enable Interrupt\n");

	kStartConsoleShell();
}
