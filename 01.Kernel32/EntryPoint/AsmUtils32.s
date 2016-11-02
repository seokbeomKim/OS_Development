[bits 32]
printmsg:
	push	ebp
	mov		ebp, esp

	push	esi
	push	edi
	push	eax
	push	ecx
	push	edx

	mov		eax, dword [ebp + 12]
	mov		esi, 160
	mul 	esi
	mov		edi, eax

	; x 좌표
	mov		eax, dword [ebp+8]
	mov		esi, 2
	mul		esi
	add		edi, eax

	; 문자열
	mov		esi, dword[ebp + 16] ; 첫 번째 파라미터

messageloop:
	mov		cl, byte [esi]
	cmp		cl, 0
	je		msgend
	mov		byte [edi + 0xb8000], cl ; 0이 아니면 비디오 메모리 어드레서 0xb800:di에 문자열 출력
	add		esi, 1
	add		edi, 2

	jmp 	messageloop

msgend:
	pop 	edx
	pop 	ecx
	pop 	eax
	pop 	edi
	pop 	esi
	pop 	ebp

	ret
