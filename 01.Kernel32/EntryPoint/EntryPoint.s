; EntryPoint.s: 리얼모드에서 보호모드로 전환

[org 0x00]
[bits 16]

start:
	mov		ax, 0x1000
	mov 	ds, ax
	mov 	es, ax

	; A20 Gate 설정
	mov 	ax, 0x2401
	int		0x15	; BIOS 인터럽트 서비스 호출

	jc		A20GATE_ERROR	; A20 게이트 활성화 실패시 CF비트가 1로 설정된다.
	jmp		A20GATE_SUCCEED

A20GATE_ERROR:
	; 시스템 컨트롤 포트 사용 시도
	in		al, 0x02
	or		al, 0x02
	and		al, 0xFE
	out		0x92, al

A20GATE_SUCCEED:
	cli
	; GDT 설정
	lgdt 	[gdtr]

	; 보호모드 진입 - CR0 레지스터 설정
	mov 	eax, 0x4000003B
	mov 	cr0, eax

	jmp 	dword 0x18: (protected_mode - $$ + 0x10000)

%include "AsmUtils32.s"

[bits 32]
protected_mode:
	mov 	ax, 0x20	; 0x10 = 16, GDT에서 3번째 위치한 커널 데이터 디스크립터를 사용하기 위한 값(0, 8, 16..)
	; 세그먼트 셀렉터 설정
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax

	mov 	ss, ax
	mov 	esp, 0xfffe
	mov 	ebp, 0xfffe

	push 	(success_msg - $$ + 0x10000)
	push 	3
	push 	0
	call 	printmsg
	add 	esp, 12

	jmp 	dword 0x18:0x10200	; CS 셀렉터를 커널 세그먼트 디스크립터로 변경하며 0x10200 명령어 실행

; 데이터 영역
	align 	8, db 0
	dw 		0x0000
gdtr:
	; gdtr은 gdt 정보를 저장하는 자료구조 이므로 (참고)
	dw 		gdtend - gdt - 1	; gdt 테이블의 전체 크
	dd 		(gdt - $$ + 0x10000)	; gdt 테이블이 시작하는 위
gdt:
	;; null scriptor
	null_descriptor:
		dw 	0x0000
		dw 	0x0000
		db 	0x00
		db 	0x00
		db 	0x00
		db 	0x00

	ia32_code_descriptor:
		dw	0xffff
		dw	0x0000
		db	0x00
		db	0x9a
		db	0xaf
		db	0x00

	ia32_data_descriptor:
		dw	0xffff
		dw	0x0000
		db	0x00
		db	0x92
		db	0xaf
		db	0x00

	code_descriptor:
		dw 	0xffff
		dw 	0x0000
		db 	0x00
		db 	0x9a
		db 	0xcf
		db 	0x00

	data_descriptor:
		dw 	0xffff
		dw 	0x0000
		db 	0x00
		db 	0x92
		db 	0xcf
		db 	0x00
gdtend:

success_msg: db '[BootLoader] Switched to protected mode', 0

times 512 - ($ - $$) db 0x00
