    [org 0x0]
    [bits 16]

    section	.text	; text 세그먼트 정의

    jmp		0x07c0:start

	total_sectors: 			dw	0x02
	kernel32_sectors:		dw	0x02

	%include 		"AsmUtils.asm"

start:
	push	si
	push	di
	push	ax
	push	cx
	push	dx

	mov		ax, 0xb800
	mov		es, ax

	; y 좌표
	mov		ax, word [bp+4]
	mov 	ax, 0x07c0
	mov 	ds, ax
	mov 	ax, 0xb800
	mov 	es, ax		; es 세그먼트에 비디오 메모리 시작 어드레스(0xb800) 설정

	mov 	si, 0

	; 스택 설정 0x0000:0x0000 ~ 0x0000:ffff 영역에 64kb 크기로 생성
	mov 	ax, 0x0000
	mov 	ss, ax
	mov 	sp, 0xFFFE
	mov 	bp, 0xFFFE

screenclearloop:
	mov 	byte [es: si], 0
	mov 	byte [es: si+1], 0x0A

	add 	si, 2		; 문자와 속성을 설정했으므로 다음 위치로 이동

	cmp 	si, 80*25*2 ; 화면 전체 크기는 80*25 라인이므로 문자+속성 2바이트 곱

	jl 		screenclearloop

INITIAL_MESSAGES:
	push 	msg
	push 	0
	push 	0
	call 	printmsg
	add 	sp, 6

	push 	loadmsg
	push 	1
	push 	0
	call 	printmsg
	add 	sp, 6

resetdisk:
	mov 	ax, 0
	mov 	dl, 0
	int 	0x13
	jc 		disk_error_handler

	mov 	si, 0x1000	; 디스크 내용을 메모리로 복사할 어드레스(es:bx)를 0x10000으로 설정
	mov 	es, si
	mov 	bx, 0x0000

	mov 	di, word [total_sectors]

read_data:
	cmp 	di, 0
	je 		end_read
	sub 	di, 0x1

	mov 	ah, 0x02
	mov 	al, 0x1
	mov 	ch, byte [tracks]
	mov 	cl, byte [sectors]
	mov 	dh, byte [heads]
	mov 	dl, 0x00

	int		0x13
	jc		disk_error_handler

	add		si, 0x0020
	mov		es, si

	mov		al, byte [sectors]
	add		al, 0x01
	mov		byte [sectors], al
	cmp		al, 19
	jl		read_data

	xor 	byte [heads], 0x01
	mov		byte [sectors], 0x01

	cmp		byte [heads], 0x00
	jne		read_data

	add		byte [tracks], 0x01
	jmp		read_data

end_read:
	push 	complete_msg
	push 	2
	push 	0
	call 	printmsg
	add 	sp, 6

	jmp 	0x1000:0x0000


disk_error_handler:
	push 	disk_err_msg
	push 	20
	push 	1
	jmp 	$

	; Data section
	complete_msg:	db '[BootLoader] Reading the OS image complete', 0
	msg: 			db '[BootLoader] Start OS64 Boot Loader', 0
	loadmsg: 		db '[BootLoader] Load OS Image File', 0
	disk_err_msg:	db '[BootLoader] Failed to read disk', 0
	heads: 			db 0x00
	tracks: 		db 0x00
	sectors:		db 0x02

    times 			510 - ($ - $$)		db 0x00

    dw 				0xaa55
