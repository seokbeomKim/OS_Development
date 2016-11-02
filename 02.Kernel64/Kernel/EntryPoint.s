[bits 64]

section .text

extern Main

START:
	mov		ax, 0x10	; 64비트용세그먼트 디스크립터
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax

	mov		ss, ax
	mov		rsp, 0x6ffff8
	mov		rbp, 0x6ffff8

	call	Main		; c언어 Main함수 호출
