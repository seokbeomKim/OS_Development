#include "Page.h"

void kSetPageEntryData(PENTRY* pEntry, DWORD dwUpperBaseAddress,
		DWORD dwLowerBaseAddress, DWORD dwLowerFlags, DWORD dwUpperFlags)
{
	pEntry->dwAttributeAndLowerBaseAddress = dwLowerBaseAddress | dwLowerFlags;
	pEntry->dwUpperBaseAddressAndEXB = (dwUpperBaseAddress & 0xFF) | dwUpperFlags;
}

void kInitializePageTables(void)
{
	PML4ENTRY*	pmlEntry;
	PDPENTRY*	pdpEntry;
	PDENTRY*	pdEntry;
	DWORD		dwMappingAddress;

	int i;

	/*
	 * PML4 테이블 생성 및 초기화 (한 개만 필요)
	 */
	pmlEntry = (PML4ENTRY*) 0x100000;
	kSetPageEntryData(&(pmlEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);
	for (i = 1; i < PAGE_ENTRY_MAXCOUNT; i++) {
		kSetPageEntryData(&(pmlEntry[i]), 0, 0, 0, 0);
	}

	/*
	 * 페이지 디렉터리 포인터 테이블 생성 및 초기화
	 * 64GB만 사용하므로 64개 엔트리 초기화 후에는 전부 0으로 초기화한다.
	 */
	pdpEntry = (PDPENTRY *) 0x101000;
	for (i = 0; i < 64; i++) {
		kSetPageEntryData( &(pdpEntry[i]), 0,
				0x102000 + (i * PAGE_TABLE_SIZE), PAGE_FLAGS_DEFAULT, 0);
	}
	for (i = 64; i < PAGE_ENTRY_MAXCOUNT; i++) {
		kSetPageEntryData( &(pdpEntry[i]), 0, 0, 0, 0);
	}

	/*
	 * PAGE DIRECTORY TABLE 생성 및 초기화
	 */
	pdEntry = (PDENTRY *) 0x102000;
	dwMappingAddress = 0;
	for (i = 0; i < PAGE_ENTRY_MAXCOUNT * 64; i++) {
		/*
		 * PAGE_DEFAULT_SIZE를 20비트(1MB)로 나눈 뒤 인덱스와 곱하고 그 다음에 다시 4KB로 나누어 계산한다.
		 */
		kSetPageEntryData( &(pdEntry[i]), (i * (PAGE_DEFAULT_SIZE >> 20)) >> 12,
				dwMappingAddress, PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, 0);
		dwMappingAddress += PAGE_DEFAULT_SIZE;
	}
}
