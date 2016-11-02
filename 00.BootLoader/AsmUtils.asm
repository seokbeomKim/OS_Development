[bits 16]
; Functions: printmsg (msg, x, y)
printmsg:
	push	bp
	mov		bp, sp

	push	es
	push	si
	push	di
	push	ax
	push	cx
	push	dx

	mov		ax, 0xb800
	mov		es, ax

	; y 좌표
	mov		ax, word [bp+6]
	mov		si, 160
	mul		si
	mov		di, ax

	; x 좌표
	mov		ax, word [bp+4]
	mov		si, 2
	mul		si
	add		di, ax

	; 문자열
	mov		si, word[bp+8] ; 첫 번째 파라미터

messageloop:
	mov		cl, byte [si]
	cmp		cl, 0
	je		msgend
	mov		byte [es:di], cl ; 0이 아니면 비디오 메모리 어드레서 0xb800:di에 문자열 출력
	add		si, 1
	add		di, 2

	jmp 	messageloop

msgend:
	pop 	dx
	pop 	cx
	pop 	ax
	pop 	di
	pop 	si
	pop 	es
	pop 	bp

	ret
