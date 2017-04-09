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

// ���� �� ũ��(Mbyte ����)
static gs_qwTotalRAMMBSize = 0;

/**
 *  64Mbyte �̻��� ��ġ���� �� ũ�⸦ üũ
 *      ���� ���� �������� �ѹ��� ȣ���ؾ� ��
 */
void kCheckTotalRAMSize( void )
{
    DWORD* pdwCurrentAddress;
    DWORD dwPreviousValue;

    // 64Mbyte(0x4000000)���� 4Mbyte������ �˻� ����
    pdwCurrentAddress = ( DWORD* ) 0x4000000;
    while( 1 )
    {
        // ������ �޸𸮿� �ִ� ���� ����
        dwPreviousValue = *pdwCurrentAddress;
        // 0x12345678�� �Ἥ �о��� �� ������ ���� �������� ��ȿ�� �޸�
        // �������� ����
        *pdwCurrentAddress = 0x12345678;
        if( *pdwCurrentAddress != 0x12345678 )
        {
            break;
        }
        // ���� �޸� ������ ����
        *pdwCurrentAddress = dwPreviousValue;
        // ���� 4Mbyte ��ġ�� �̵�
        pdwCurrentAddress += ( 0x400000 / 4 );
    }
    // üũ�� ������ ��巹���� 1Mbyte�� ������ Mbyte ������ ���
    gs_qwTotalRAMMBSize = ( QWORD ) pdwCurrentAddress / 0x100000;
}

QWORD kGetTotalRAMSize( void )
{
    return gs_qwTotalRAMMBSize;
}
