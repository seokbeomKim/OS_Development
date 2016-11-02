#include "Types.h"
#include "Page.h"
#include "AsmHelpers.h"

void kPrintString(int x, int y, const char* str);
BOOL kInitializeKernel64Area(void);
BOOL kCheckMinimumMemoryCapacity(void);
BOOL kCheckCPUCompatibility(void);
void kCopyKernel64ImageTo2Mbyte(void);

/*
 * Main 함수를 앞 쪽에 배치하여 컴파일 시에 코드 섹션의 가장 앞쪽에 위치하게 하였다.
 */
int main(void)
{
	kPrintString(0, 4,
			"[KERNEL] C source code has been added to binary file");
	if (kCheckMinimumMemoryCapacity() == TRUE) {
		kPrintString(0, 5,
				"[KERNEL] Memory capacity is OKAY.");
	}
	else {
		kPrintString(0, 5, "[KERNEL] System needs more memory capacity");
		while(1);
	}
	if (kInitializeKernel64Area() == TRUE) {
		kPrintString(0, 6, "[KERNEL] Memory initialization is finished successfully");
	}
	else {
		kPrintString(0, 6, "[KERNEL] Failed to initialize memory");
		while(1);
	}

	kInitializePageTables();
	kPrintString(0, 7, "[KERNEL] IA-32e Page Table initialized");

	kCheckCPUCompatibility();

	kCopyKernel64ImageTo2Mbyte();
	kEnter64bitKernel();
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

BOOL kCheckMinimumMemoryCapacity(void)
{
	DWORD* pCursor;

	pCursor = (DWORD*) 0x100000;

	while ((DWORD)pCursor < 0x4000000) {
		*pCursor = 0x12345678;

		if (*pCursor != 0x12345678) {
			return FALSE;
		}
		pCursor += (0x100000 / 4);
	}
	return TRUE;
}

/*
 * 0x100000 (1MB) ~ 0x600000(6MB) 영역을 0으로 초기화
 */
BOOL kInitializeKernel64Area(void)
{
	DWORD* pCursor;

	pCursor = (DWORD*) 0x100000;
	while ( (DWORD) pCursor < 0x600000 ) {
		*pCursor = 0x00;
		if ( *pCursor != 0x00 ) {
			return FALSE;
		}
		pCursor++;
	}

	return TRUE;
}

BOOL kCheckCPUCompatibility(void)
{
	char vendor[32] = {0, };
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	kReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	*((DWORD *)vendor) = dwEBX;
	*((DWORD *)vendor + 1) = dwEDX;
	*((DWORD *)vendor + 2) = dwECX;

	if (dwEDX & (1 << 29)) {
		kPrintString(0, 8, "[KERNEL] 64-bits Compatibility check: ");
	}
	else {
		kPrintString(0, 8, "[KERNEL] 64-bits Compatibility check: FAIL");
	}
	kPrintString(38, 8, vendor);
}

void kCopyKernel64ImageTo2Mbyte(void)
{
    WORD wKernel32SectorCount, wTotalKernelSectorCount;
    DWORD* pdwSourceAddress,* pdwDestinationAddress;
    int i;

    wTotalKernelSectorCount = *( ( WORD* ) 0x7C05 );
    wKernel32SectorCount = *( ( WORD* ) 0x7C07 );

    pdwSourceAddress = ( DWORD* ) ( 0x10000 + ( wKernel32SectorCount * 512 ) );
    pdwDestinationAddress = ( DWORD* ) 0x200000;

    for( i = 0 ; i < 512 * ( wTotalKernelSectorCount - wKernel32SectorCount ) / 4;
        i++ )
    {
        *pdwDestinationAddress = *pdwSourceAddress;
        pdwDestinationAddress++;
        pdwSourceAddress++;
    }
}
