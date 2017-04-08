#include <DataType/Types.h>
#include <DeviceDriver/DeviceDriver.h>
#include <Init/Descriptor.h>
#include <Asm/AsmHelper.h>
#include <Testcase/Testcase.h>

void kPrintString(int x, int y, const char* str);

void Main(void)
{
	BYTE bTemp;
	BYTE bFlags;
	char vcTemp[2] = {0, };
	int i = 0, j;
	KEYDATA stData;

	kPrintString(0, 10, "[KERNEL] ==== Enter to IA-32e mode ====");
	kPrintString(0, 12, "[KERNEL] Change old GDT to new one for IA-32e mode");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_START_ADDRESS);

	kPrintString(0, 13, "[KERNEL] Set TSS Segment");
	kLoadTR(GDT_TSS_SEGMENT);

	// �씤�꽣�읇�듃 �궗�슜 �쐞�빐 IDT 珥덇린�솕
	kPrintString(0, 14, "[KERNEL] Initialize and set IDT");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_START_ADDRESS);

	// �궎蹂대뱶 珥덇린�솕
	kInitializeKeyboard();
	kPrintString(0, 15, "[KERNEL] Activate Keyboard");

	// PIC 而⑦듃濡ㅻ윭 珥덇린�솕
	kInitializePIC();
	kPrintString(0, 16, "[KERNEL] Initialize PIC");

	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(0, 17, "[KERNEL] Enable Interrupt");

	while(1) {
		if( kGetKeyFromKeyQueue( &stData ) == TRUE )
		{
			if( stData.bFlags & KEY_FLAGS_DOWN )
			{
				vcTemp[ 0 ] = stData.bASCIICode;
				kPrintString( i++, 20, vcTemp );

				if( vcTemp[ 0 ] == '0' )
				{
					bTemp = bTemp / 0;
				}
			}
		}
	}
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
