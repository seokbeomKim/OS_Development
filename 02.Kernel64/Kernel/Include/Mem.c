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

// 램의 총 크기(Mbyte 단위)
static gs_qwTotalRAMMBSize = 0;

/**
 *  64Mbyte 이상의 위치부터 램 크기를 체크
 *      최초 부팅 과정에서 한번만 호출해야 함
 */
void kCheckTotalRAMSize( void )
{
    DWORD* pdwCurrentAddress;
    DWORD dwPreviousValue;

    // 64Mbyte(0x4000000)부터 4Mbyte단위로 검사 시작
    pdwCurrentAddress = ( DWORD* ) 0x4000000;
    while( 1 )
    {
        // 이전에 메모리에 있던 값을 저장
        dwPreviousValue = *pdwCurrentAddress;
        // 0x12345678을 써서 읽었을 때 문제가 없는 곳까지를 유효한 메모리
        // 영역으로 인정
        *pdwCurrentAddress = 0x12345678;
        if( *pdwCurrentAddress != 0x12345678 )
        {
            break;
        }
        // 이전 메모리 값으로 복원
        *pdwCurrentAddress = dwPreviousValue;
        // 다음 4Mbyte 위치로 이동
        pdwCurrentAddress += ( 0x400000 / 4 );
    }
    // 체크가 성공한 어드레스를 1Mbyte로 나누어 Mbyte 단위로 계산
    gs_qwTotalRAMMBSize = ( QWORD ) pdwCurrentAddress / 0x100000;
}

QWORD kGetTotalRAMSize( void )
{
    return gs_qwTotalRAMMBSize;
}
