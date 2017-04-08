#include <Init/InterruptHandler.h>
#include <DeviceDriver/PIC_Controller.h>

// 怨듯넻�쑝濡� �궗�슜�릺�뒗 Exception Handler
void kCommonExceptionHandler(int iVectorNum, QWORD qwErrorCode) {
	char vcBuffer[2] = {0, };

	vcBuffer[0] = '0' + iVectorNum / 10;
	vcBuffer[1] = '0' + iVectorNum % 10;

	kPrintString(0, 20, "[EXCEPTION] An exception has been occured: ");
	kPrintString(42, 20, vcBuffer);
}

// 怨듯넻�쑝濡� �궗�슜�릺�뒗 Interrupt Handler
void kCommonInterruptHandler(int iVectorNum) {
	char vcBuffer[] = "[INT:  ,  ]";
	static int g_iCommonInterruptCount = 0;

	vcBuffer[5] = '0' + (int)(iVectorNum / 10);
	vcBuffer[6] = '0' + (int)(iVectorNum % 10);
	vcBuffer[8] = '0' + g_iCommonInterruptCount;

    g_iCommonInterruptCount = ( g_iCommonInterruptCount + 1 ) % 10;
    kPrintString(60, 0, vcBuffer);

	kSendEOIToPIC(iVectorNum - PIC_IRQ_START_VECTOR);
}

void kKeyboardHandler(int iVectorNum) {
	char vcBuffer[] = "[INT:  ,  ]";
	static int g_iKeyboardInterruptCount = 0x00;
//
//	vcBuffer[5] = '0' + (int)(iVectorNum / 10);
//	vcBuffer[6] = '0' + (int)(iVectorNum % 10);
//	vcBuffer[8] = '0' + g_iKeyboardInterruptCount;
//	g_iKeyboardInterruptCount = (++g_iKeyboardInterruptCount) % 10;
//	kPrintString(0, 30, vcBuffer);
//
	BYTE bTemp;

	if (kIsReadBufferFull() == TRUE) {
		bTemp = kGetKeyboardScanCode();
		kConvertScanCodeAndEnqueue(bTemp);
	}

	kSendEOIToPIC(iVectorNum - PIC_IRQ_START_VECTOR);

}

void kGeneralProtectionExceptionHandler(int iVectorNum, QWORD qwErrorCode) {
	kPrintString(0, 0, "[EXCEPTION] GeneralProtection Exception!!!");

	char vcBuffer[5] = {0, };
	QWORD temp = qwErrorCode;

	vcBuffer[0] = '0' + temp / 10000;
	temp = temp % 10000;
	vcBuffer[1] = '0' + temp / 1000;
	temp = temp % 1000;
	vcBuffer[2] = '0' + temp / 100;
	temp = temp % 100;
	vcBuffer[3] = '0' + temp / 10;
	temp = temp % 10;
	vcBuffer[4] = '0' + temp;

	kPrintString(0, 1, vcBuffer);

	while (1);
}
