#ifndef __ISR_H__
#define __ISR_H__

// ISR 정의 for Exception
void	kISR_DivideError(void);
void	kISR_Debugger(void);
void	kISR_NMI(void);
void	kISR_BreakPoint(void);
void	kISR_OverFlow(void);
void	kISR_Bounds(void);
void	kISR_InvalidOpcode(void);
void	kISR_CoprocessorNotAvailable(void);
void	kISR_DoubleFault(void);
void	kISR_CoprocessorSegmentOverrun(void);
void	kISR_InvalidTaskStateSegment(void);
void	kISR_SegmentNotPresent(void);
void	kISR_StackFault(void);
void	kISR_GeneralProtection(void);
void	kISR_PageFault(void);
void	kISR15(void);
void	kISR_MathFault(void);
void	kISR_AlignmentCheck(void);
void	kISR_MachineCheck(void);
void	kISR_SIMD(void);
void	kISR_VirtualizationException(void);
void	kISR_ControlProtectionException(void);
void    kISR_ETCException(void);

// ISR for Interrupt
void	kISR_Timer(void);
void	kISR_Keyboard(void);
void	kISR_SlavePIC(void);
void	kISR_SerialPort2(void);
void	kISR_SerialPort1(void);
void	kISR_ParallelPort2(void);
void	kISR_FloppyDiskController(void);
void	kISR_ParallelPort1(void);
void	kISR_RTC(void);
void	kISR41(void);
void	kISR42(void);
void	kISR43(void);
void	kISR_Mouse(void);
void	kISR_Coprocessor(void);
void	kISR_HardDisk1(void);
void	kISR_HardDisk2(void);
void	kISR_ETC(void);

#endif
