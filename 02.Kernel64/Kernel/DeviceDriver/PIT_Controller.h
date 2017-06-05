/*
 * PIT_Controller.h
 *
 * Programmable Interval Timer 디바이스 드라이버
 */

#ifndef __PIT_CONTROLLER_H__
#define __PIT_CONTROLLER_H__

#include <DataType/Types.h>

#define PIT_FREQUENCY	1193180
#define MSTOCOUNT(x)	(PIT_FREQUENCY * (x) / 1000) 	// ms 값을 카운터 값으로 변환
#define USTOCOUNT(x)	(PIT_FREQUENCY * (x) / 1000000) // us 값을 카운터 값으로 변환

// I/O PORT
#define	PIT_PORT_CONTROL	0x43
#define	PIT_PORT_COUNTER0	0x40
#define PIT_PORT_COUNTER1	0x41
#define	PIT_PORT_COUNTER2	0x42

// MODES
#define	PIT_CONTROL_COUNTER0	0x00
#define	PIT_CONTROL_LSBMSBRW	0x30
#define	PIT_CONTROL_LATCH		0x00
#define	PIT_CONTROL_MODE0		0x00
#define	PIT_CONTROL_MODE2		0x04
#define	PIT_CONTROL_BINARYCOUNTER		0x00
#define PIT_CONTROL_BCDCOUNTER			0x01

// 한번만 카운트하도록 설정하는 카운터 0용 매크로
#define	PIT_COUNTER0_ONCE		(PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | \
								PIT_CONTROL_MODE0 | PIT_CONTROL_BINARYCOUNTER)


// 계속해서 반복하도록 설정하는 카운터 0용 매크로
#define PIT_COUNTER0_PERIODIC	(PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | \
								PIT_CONTROL_MODE2 | PIT_CONTROL_BINARYCOUNTER)


#define PIT_COUNTER0_LATCH		(PIT_CONTROL_COUNTER0 | PIT_CONTROL_LATCH)

void kInitializePIT(WORD wCount, BOOL bPeriodic);
WORD kReadCounter0(void);
void kWaitUsingDirectPIT(WORD wCount);

#endif
