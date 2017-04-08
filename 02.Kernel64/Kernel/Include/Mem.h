#ifndef __MEM_H__
#define __MEM_H__

#include <DataType/Types64.h>

void	kMemSet(void* pvDst, BYTE bData, int iSize);
int		kMemCpy(void* pvDst, const void* pvSrc, int iSize);
int		kMemCmp(const void* pvDst, const void* pvSrc, int iSize);
BOOL	kSetInterruptFlag(BOOL bEnableInterrupt);

#endif
