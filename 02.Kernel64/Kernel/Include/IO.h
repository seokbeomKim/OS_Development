#ifndef __IO_H__
#define __IO_H__

#include <DataType/Types.h>
#include <stdarg.h>

int		kSPrintf(char* pcBuffer, const char* pcFormatString, ...);
int		kVSPrintf(char* pcBuffer, const char* pcFormatString, va_list ap);

#endif
