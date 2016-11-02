#ifndef __PAGE_H__

#include "Types.h"

#define	PAGE_FLAGS_P	0x00000001
#define	PAGE_FLAGS_RW	0x00000002
#define	PAGE_FLAGS_US	0x00000004
#define	PAGE_FLAGS_PWT	0x00000008
#define PAGE_FLAGS_PCD	0x00000010
#define PAGE_FLAGS_A	0x00000020
#define PAGE_FLAGS_D	0x00000040
#define PAGE_FLAGS_PS	0x00000080
#define PAGE_FLAGS_G	0x00000100
#define PAGE_FLAGS_PAT	0x00001000
#define PAGE_FLAGS_EXB	0x80000000
#define PAGE_FLAGS_DEFAULT	(PAGE_FLAGS_P | PAGE_FLAGS_RW)

#define PAGE_TABLE_SIZE		0x1000
#define PAGE_ENTRY_MAXCOUNT	512
#define PAGE_DEFAULT_SIZE	0x200000

#pragma pack (push, 1)
/*
 * PML4ENTRY:	페이지 맵 레벨 4 테이블
 * PDPENTRY:	페이지 디렉터리 포인터 테이블
 * PDENTRY:		페이지 디렉터리
 * PENTRY:		페이지 엔트리
 */
typedef struct PageTableEntryStruct
{
	DWORD dwAttributeAndLowerBaseAddress;
	DWORD dwUpperBaseAddressAndEXB;
} PML4ENTRY, PDPENTRY, PDENTRY, PENTRY;
#pragma pack (pop)

void kInitializePageTables(void);
void kSetPageEntryData(PENTRY* pEntry, DWORD dwUpperBaseAddress, \
		DWORD dwLowerBaseAddress, DWORD dwLowerFlags, DWORD dwUpperFlags);


#endif
