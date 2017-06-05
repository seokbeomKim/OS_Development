#include <DeviceDriver/PIT_Controller.h>

void kInitializePIT(WORD wCount, BOOL bPeriodic)
{
	kWriteToPort(PIT_PORT_CONTROL, PIT_COUNTER0_ONCE);

	if (bPeriodic == TRUE) {
		kWriteToPort(PIT_PORT_CONTROL, PIT_COUNTER0_PERIODIC);
	}

	kWriteToPort(PIT_PORT_COUNTER0, wCount);
	kWriteToPort(PIT_PORT_COUNTER0, wCount >> 8);
}

WORD kReadCounter0(void) {
	BYTE bHighByte, bLowByte;
	WORD wTemp = 0;

	kWriteToPort(PIT_PORT_CONTROL, PIT_COUNTER0_LATCH);

	bLowByte = kReadFromPort(PIT_PORT_COUNTER0);
	bHighByte = kReadFromPort(PIT_PORT_COUNTER0);

	wTemp = bHighByte;
	wTemp = (wTemp << 8) | bLowByte;

	return wTemp;
}

void kWaitUsingDirectPIT(WORD wCount) {
	WORD wLastCounter0;
	WORD wCurrentCounter0;

	kInitializePIT(0, TRUE);

	wLastCounter0 = kReadCounter0();

	while (TRUE) {
		wCurrentCounter0 = kReadCounter0();
		if (((wLastCounter0 - wCurrentCounter0) & 0xFFFF) >= wCount) {
			break;
		}
	}
}
