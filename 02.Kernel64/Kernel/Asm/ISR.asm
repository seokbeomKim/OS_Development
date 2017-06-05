; ISR.asm: Interrupt Service Routine 정의 파일
;; Interrupt Handler를 ISR이라고도 한다.

[bits 64]
section .text

; 외부 정의 함수 import
extern kCommonExceptionHandler, kCommonInterruptHandler, kKeyboardHandler, kGeneralProtectionExceptionHandler

; export definitions
global kISR_DivideError, kISR_Debugger, kISR_NMI, kISR_BreakPoint, kISR_OverFlow, kISR_Bounds, kISR_InvalidOpcode, kISR_CoprocessorNotAvailable, kISR_DoubleFault, \
kISR_CoprocessorSegmentOverrun, kISR_InvalidTaskStateSegment, kISR_SegmentNotPresent, kISR_StackFault, kISR_GeneralProtection, kISR_PageFault, kISR15, kISR_MathFault, \
kISR_AlignmentCheck, kISR_MachineCheck, kISR_SIMD, kISR_VirtualizationException, kISR_ControlProtectionException, kISR_Timer, kISR_Keyboard, kISR_SlavePIC, \
kISR_SerialPort2, kISR_SerialPort1, kISR_ParallelPort2, kISR_FloppyDiskController, kISR_ParallelPort1, kISR_RTC, kISR41, kISR42, kISR43, kISR_Mouse, kISR_Coprocessor, \
kISR_HardDisk1, kISR_HardDisk2, kISR_ETC
global kISR_ETCException
global kSaveContext, kRestoreContext
global kSwitchContext

;; Context 저장과 셀렉터를 교체한다.
%macro kSaveContext		0
	push	rbp
	mov		rbp, rsp
	push	rax
	push	rbx
	push	rcx
	push	rdx
	push	rdi
	push	rsi
	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15

	mov		ax, ds	; ds segment selector
	push	rax
	mov		ax, es	; es segment selector
	push	rax
	push 	fs
	push	gs

	mov		ax, 0x10			; 커널 데이터 세그먼트로 세그먼트 셀렉터 교체
	mov		ds, ax
	mov		es, ax
	mov		gs, ax
	mov		fs, ax
%endmacro

;; 핸들러 호출 후에는 저장했던 내용을 복원한다.
%macro kRestoreContext	0
	pop		gs
	pop		fs
	pop		rax
	mov		es, ax
	pop		rax
	mov		ds, ax

	pop		r15
	pop		r14
	pop		r13
	pop		r12
	pop		r11
	pop		r10
	pop		r9
	pop		r8
	pop		rsi
	pop		rdi
	pop		rdx
	pop		rcx
	pop		rbx
	pop		rax
	pop		rbp
%endmacro

kSwitchContext:
	push	rbp
	mov		rbp, rsp

	pushfq	; pushfq: Save RFLAGS to stack

	; Current Context NULL이면 context 저장 필요 없음
	cmp		rdi, 0
	je		.LoadContext
	
	popfq	; Restore RFLAGS 

	; Save SS 
	push	rax 
	mov		ax, ss
	mov		qword [rdi + (23 * 8)], rax 

	;; RBP
	; push rbp, return address 제외하여 rsp 저장 (context switch 위해서)
	mov		rax, rbp
	add		rax, 16 
	mov		qword [rdi + (22 * 8)], rax


	;; RFLAGS 
	pushfq
	pop		rax ; RFLAGS를 스택에 push한 뒤 pop rax를 통해 rax에 값을 저장한다.
	mov		qword [rdi + (21 * 8)], rax 

	;; CS 
	mov		ax, cs 
	mov		qword [rdi + (20 * 8)], rax 

	;; RIP 레지스터를 return address로 설정한다: RIP
	mov		rax, qword [rbp + 8]
	mov		qword [rdi + (19 * 8)], rax 

	pop		rax
	pop		rbp

	add		rdi, (19 * 8)
	mov		rsp, rdi 
	sub		rdi, (19 * 8)

	kSaveContext

.LoadContext:
	mov		rsp, rsi

	kRestoreContext
	iretq


; Definition of Handlers: https://en.wikipedia.org/wiki/Interrupt_descriptor_table 참고
; 0x00, Divide Error ISR
kISR_DivideError:
	kSaveContext

	; 핸들러에 예외 번호 삽입하고 핸들러 호출
	mov		rdi, 0
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x01, Debug
kISR_Debugger:
	kSaveContext

	mov		rdi, 1
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x02, NMI
kISR_NMI:
	kSaveContext

	mov		rdi, 2
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x03, BreakPoint
kISR_BreakPoint:
	kSaveContext

	mov		rdi, 3
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x04, OverFlow
kISR_OverFlow:
	kSaveContext

	mov		rdi, 4
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x05, Bound Range Exceeded
kISR_Bounds:
	kSaveContext

	mov		rdi, 5
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x06, Invalid Opcode
kISR_InvalidOpcode:
	kSaveContext

	mov		rdi, 6
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x07, Coprocessor not available;
; 책에서는 Device not available 로 표기되어 있다.
kISR_CoprocessorNotAvailable:
	kSaveContext

	mov		rdi, 7
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x08, Double Fault
kISR_DoubleFault:
	kSaveContext

	mov		rdi, 8
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8

	iretq

; 0x09, Coprocessor Segment Overrun
kISR_CoprocessorSegmentOverrun:
	kSaveContext

	mov		rdi, 9
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x0A, Invalid Task State Segment
kISR_InvalidTaskStateSegment:
	kSaveContext

	mov		rdi, 10
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8
	iretq

; 0x0B, Segment Not Present
kISR_SegmentNotPresent:
	kSaveContext

	mov		rdi, 11
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8
	iretq

; 0x0C, Stack Fault
kISR_StackFault:
	kSaveContext

	mov		rdi, 12
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8
	iretq

; 0x0D, General Protection
kISR_GeneralProtection:
	kSaveContext

	mov		rdi, 13
	mov		rsi, qword [rbp + 8]
	call	kGeneralProtectionExceptionHandler

	kRestoreContext
	add		rsp, 8

	iretq

; 0x0E, Page Fault
kISR_PageFault:
	kSaveContext

	mov		rdi, 14
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8

	iretq

; 0x0F, reserved
kISR15:
	kSaveContext

	mov		rdi, 15
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x10, Math Fault: 책에는 FPU Error라고 표시되어 있다.
kISR_MathFault:
	kSaveContext

	mov		rdi, 16
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x11, Alignment Check
kISR_AlignmentCheck:
	kSaveContext

	mov		rdi, 17
	mov		rsi, qword [rbp + 8]
	call	kCommonExceptionHandler

	kRestoreContext
	add		rsp, 8

	iretq

; 0x12, Machine Check
kISR_MachineCheck:
	kSaveContext

	mov		rdi, 18
	call	kCommonExceptionHandler

	kRestoreContext
	iretq

; 0x13, SIMD Floating-Point Exception
kISR_SIMD:
	kSaveContext

	mov		rdi, 19
	call	kCommonExceptionHandler

	kRestoreContext
	iretq

; 0x14, Virtualization Exception
kISR_VirtualizationException:
	kSaveContext

	mov		rdi, 20
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x15, Control Protection Exception
kISR_ControlProtectionException:
	kSaveContext

	mov		rdi, 21
	call	kCommonExceptionHandler

	kRestoreContext

	iretq

; 0x16 ~ 0x1F까지 Reserved ISR
kISR_ETCException:
	kSaveContext

	mov		rdi, 20
	call	kCommonExceptionHandler

	kRestoreContext

	iretq


;; Interrupt Handlers
; 0x20, Timer ISR
kISR_Timer:
	kSaveContext

	mov		rdi, 32
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x21, Keyboard ISR
kISR_Keyboard:
	kSaveContext

	mov		rdi, 33
	call	kKeyboardHandler

	kRestoreContext

	iretq

; 0x22, SlavePIC
kISR_SlavePIC:
	kSaveContext

	mov		rdi, 34
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x23, Serial Port 2
kISR_SerialPort2:
	kSaveContext

	mov		rdi, 35
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x24, SerialPort1:
kISR_SerialPort1:
	kSaveContext

	mov		rdi, 36
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x25, Parallel Port 2
kISR_ParallelPort2:
	kSaveContext

	mov		rdi, 37
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x26, Floppy Disk Controller
kISR_FloppyDiskController:
	kSaveContext

	mov		rdi, 38
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x27, Parallel Port 1
kISR_ParallelPort1:
	kSaveContext

	mov		rdi, 39
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x28, RTC
kISR_RTC:
	kSaveContext

	mov		rdi, 40
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x29, Reserved
kISR41:
	kSaveContext

	mov		rdi, 41
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2A, Unused
kISR42:
	kSaveContext

	mov		rdi, 42
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2B, Unused
kISR43:
	kSaveContext

	mov		rdi, 43
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2C, Mouse
kISR_Mouse:
	kSaveContext

	mov		rdi, 44
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2D, CoProcessor
kISR_Coprocessor:
	kSaveContext

	mov		rdi, 45
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2E, Hard Disk 1
kISR_HardDisk1:
	kSaveContext

	mov		rdi, 46
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x2F, Hard Disk 2
kISR_HardDisk2:
	kSaveContext

	mov		rdi, 47
	call	kCommonInterruptHandler

	kRestoreContext

	iretq

; 0x30, ISR for Other Intterupts
kISR_ETC:
	kSaveContext

	mov		rdi, 48
	call	kCommonInterruptHandler

	kRestoreContext

	iretq
