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

    // ������ RFLAGS �������� ���� ���� �ڿ� ���ͷ�Ʈ ����/�Ұ� ó��
    qwRFLAGS = kReadRFLAGS();
    if( bEnableInterrupt == TRUE )
    {
        kEnableInterrupt();
    }
    else
    {
        kDisableInterrupt();
    }

    // ���� RFLAGS ���������� IF ��Ʈ(��Ʈ 9)�� Ȯ���Ͽ� ������ ���ͷ�Ʈ ���¸� ��ȯ
    if( qwRFLAGS & 0x0200 )
    {
        return TRUE;
    }
    return FALSE;
}
