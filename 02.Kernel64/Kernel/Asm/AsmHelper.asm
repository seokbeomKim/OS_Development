[bits 64]

section .text

; AsmHelper.asm: 커널에서 사용되는 어셈블리 코드
global kReadFromPort, kWriteToPort, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS

kReadFromPort:
	push 	rdx

	mov		rdx, rdi	; rax 레지스터에 파라미터 1 저장
	mov		rax, 0
	in		al, dx		; dx 레지스터에 저장된 포트 어드레스에서 한 바이트를 읽음
						; al 레지스터에 저장, AL 레지스터는 함수의 반환 값으로 사용된다.

	pop		rdx
	ret

kWriteToPort:
	push	rdx
	push	rax

	mov		rdx, rdi	; rdx 레지스터에 파라미터 1을 저장
	mov		rax, rsi	; rax 레지스터에 파라미터 2를 저장
	out		dx, al		; dx 레지스터에 al에 저장된 데이터값 저장

	pop		rax
	pop		rdx
	ret

kLoadGDTR:
	lgdt	[rdi]		; GDTR 어드레스 (파라미터 1) 로드 및 GDT 테이블 생성
	ret

kLoadTR:
	ltr		di			; TSS 세그먼트 디스크립터 오프셋 이용하여 세그먼트 로드
	ret

kLoadIDTR:
	lidt	[rdi]		; IDT 테이블을 설정
	ret

kEnableInterrupt:
	sti
	ret

kDisableInterrupt:
	cli
	ret

kReadRFLAGS:
	pushfq
	pop 	rax

	ret
