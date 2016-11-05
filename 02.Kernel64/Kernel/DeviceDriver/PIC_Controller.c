#include <DeviceDriver/PIC_Controller.h>

void kInitializePIC(void) {
	// MASTER 초기화
	kWriteToPort(PIC_MASTER_PORT1, 0x11);					// ICW1 0x20
	kWriteToPort(PIC_MASTER_PORT2, PIC_IRQ_START_VECTOR);	// ICW2	0x21
	kWriteToPort(PIC_MASTER_PORT2, 0x04);					// ICW3	0x21
	kWriteToPort(PIC_MASTER_PORT2, 0x01);					// ICW4	0x21

	// SLAVE 초기화
	kWriteToPort(PIC_SLAVE_PORT1, 0x11);					// ICW1
	kWriteToPort(PIC_SLAVE_PORT2, PIC_IRQ_START_VECTOR + 8);// ICW2
	kWriteToPort(PIC_SLAVE_PORT2, 0x02);					// ICW3
	kWriteToPort(PIC_SLAVE_PORT2, 0x01);					// ICW4
}

void kMaskPICInterrupt(WORD wIRQ_MASK) {
	// bit mask 이용하여 IMR 레지스터를 이용, 인터럽트 요청을 선택한다. - OCW1 커맨드 사용
	// 하위 8비트는 MASTER 를 위한 bitmask
	kWriteToPort(PIC_MASTER_PORT2, (BYTE) wIRQ_MASK);
	// 상위 8비트는 SLAVE 위한 bitmask
	kWriteToPort(PIC_SLAVE_PORT2, (BYTE)(wIRQ_MASK >> 8));
}

void kSendEOIToPIC(int irq_number) {
	kWriteToPort(PIC_MASTER_PORT1, 0x20);

	// SLAVE PIC 컨트롤러의 인터럽트인 경우 MASTER과 SLAVE 모두에게 인터럽트 번호가 전달되어야 한다.
	if (irq_number >= 8) {
		kWriteToPort(PIC_SLAVE_PORT1, 0x20);
	}
}
