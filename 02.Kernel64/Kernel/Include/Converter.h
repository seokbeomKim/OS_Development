#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <DataType/Types.h>

void	kReverseString(char* pcBuffer);
long	kAToI(const char* pcBuffer, int iRadix);
QWORD	kHexStringToQword(const char* pcBuffer);
long	kDecimalStringToLong(const char* pcBuffer);
int		kIToA(long lValue, char* pcBuffer, int iRadix);
int		kHexToString(QWORD qwValue, char* pcBuffer);
int		kDecimalToString(long lValue, char* pcBuffer);

#endif
