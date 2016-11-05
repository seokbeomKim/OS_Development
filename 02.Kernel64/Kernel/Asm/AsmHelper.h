#ifndef __ASM_HELPER_H__
#define __ASM_HELPER_H__

#include <DataType/Types64.h>

BYTE	kReadFromPort(WORD port_num);
void	kWriteToPort(WORD port_num, BYTE value);
void	kLoadGDTR(QWORD qwGDTRAdress);
void	kLoadTR(WORD wTSSSegmentOffset);
void	kLoadIDTR(QWORD qwIDTRAddress);
void	kEnableInterrupt(void);
void	kDisableInterrupt(void);
QWORD	kReadRFLAGS(void);

#endif
