#ifndef	__PIC_CONTROLLER_H__
#define __PIC_CONTROLLER_H__

#include <DataType/Types64.h>

#define PIC_MASTER_PORT1		0x20
#define	PIC_MASTER_PORT2		0x21
#define	PIC_SLAVE_PORT1			0xA0
#define	PIC_SLAVE_PORT2			0xA1

// IDT 테이블에서 인터럽트 벡터가 시작되는 위치
#define	PIC_IRQ_START_VECTOR	0x20

void	kInitializePIC(void);
void	kMaskPICInterrupt(WORD wIRQ_MASK);
void	kSendEOIToPIC(int irq_number);

#endif
