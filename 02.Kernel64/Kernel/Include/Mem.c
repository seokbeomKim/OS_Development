#include <Include/Mem.h>

void kMemSet(void* pvDst, BYTE bData, int iSize) {
	int i;

	for (i = 0; i < iSize; i++) {
		((char*)pvDst)[i] = bData;
	}
}
int kMemCpy(void* pvDst, const void* pvSrc, int iSize) {
	int i;

	for (i = 0; i< iSize; i++) {
		((char*)pvDst)[i] = ((char*)pvSrc)[i];
	}

	return iSize;
}
int	kMemCmp(const void* pvDst, const void* pvSrc, int iSize) {
	int i;
	char cTemp;

	for (i = 0; i < iSize; i++) {
		cTemp = ((char*)pvDst)[i] - ((char*)pvSrc)[i];
		if (cTemp != 0) {
			return (int)cTemp;
		}
	}
	return 0;
}

BOOL	kSetInterruptFlag(BOOL bEnableInterrupt) {
    QWORD qwRFLAGS;

    // 이전의 RFLAGS 레지스터 값을 읽은 뒤에 인터럽트 가능/불가 처리
    qwRFLAGS = kReadRFLAGS();
    if( bEnableInterrupt == TRUE )
    {
        kEnableInterrupt();
    }
    else
    {
        kDisableInterrupt();
    }

    // 이전 RFLAGS 레지스터의 IF 비트(비트 9)를 확인하여 이전의 인터럽트 상태를 반환
    if( qwRFLAGS & 0x0200 )
    {
        return TRUE;
    }
    return FALSE;
}
