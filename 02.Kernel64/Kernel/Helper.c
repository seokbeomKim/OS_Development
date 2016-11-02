#include "Helper.h"

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
