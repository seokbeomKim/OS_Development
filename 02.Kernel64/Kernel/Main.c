#include "Types64.h"
#include "Keyboard.h"
#include "Descriptor.h"

void kPrintString(int x, int y, const char* str);

void Main(void)
{
	BYTE bTemp;
	BYTE bFlags;
	char vcTemp[2] = {0, };
	int i = 0, j;

	kPrintString(0, 10, "[KERNEL] ==== Enter to IA-32e mode ====");
	kPrintString(0, 12, "[KERNEL] Change old GDT to new one for IA-32e mode");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_START_ADDRESS);

	kPrintString(0, 13, "[KERNEL] Set TSS Segment");
	kLoadTR(GDT_TSS_SEGMENT);

	// 인터럽트 사용 위해 IDT 초기화
	kPrintString(0, 14, "[KERNEL] Initialize and set IDT");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_START_ADDRESS);

	// 키보드 초기화
	kInitializeKeyboard();
	kPrintString(0, 15, "[KERNEL] Activate Keyboard");

	// PIC 컨트롤러 초기화
	kInitializePIC();
	kPrintString(0, 16, "[KERNEL] Initialize PIC");

	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(0, 17, "[KERNEL] Enable Interrupt");

	while(1);
//	while(1) {
//		if (kIsReadBufferFull() == TRUE) {
//			bTemp = kGetKeyboardScanCode();
//
//			if (kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE) {
//				if (bFlags & KEY_FLAGS_DOWN) {
//					kPrintString (i++, 16, vcTemp);
//
//					if (vcTemp[0] == '0') {
//						bTemp = bTemp / 0;
//					}
//				}
//			}
//		}
//	}
}

void kPrintString(int x, int y, const char* str)
{
	CHARACTER* pstScreen = (CHARACTER *)0xB8000;
	int i;

	pstScreen += (y * 80);
	pstScreen += x;
	for (i = 0; str[i] != 0x00; i++) {
		pstScreen[i].bCharacter = str[i];
	}
}
