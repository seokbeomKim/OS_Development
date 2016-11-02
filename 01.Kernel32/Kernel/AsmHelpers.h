#ifndef __ASMHELPERS_H__

#include "Types.h"

void kReadCPUID(DWORD dwEAX, DWORD* pdwEAX, DWORD* pdwEBX, DWORD* pdwECX, DWORD* pdwEDX);
void kEnter64bitKernel(void);

#endif
