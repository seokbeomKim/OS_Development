#ifndef __INTERRUPT_HANDLER_H__
#define __INTERRUPT_HANDLER_H__

#include <DataType/Types64.h>

void	kCommonExceptionHandler(int iVectorNum, QWORD qwErrorCode);
void	kCommonInterruptHandler(int iVectorNum);
void	kKeyboardHandler(int iVectorNum);

void	kGeneralProtectionExceptionHandler(int iVectorNum, QWORD qwErrorCode);

#endif
