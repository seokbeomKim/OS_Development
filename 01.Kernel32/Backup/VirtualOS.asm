[org 0x00]
[bits 16]

section		.text

jmp			0x1000:start	;	cs 세그먼트 레지스터에 0x1000을 복사하면서, start 레이블로 이동

sectors:	dw	0x0000
total:		equ 1024

start:
	mov		ax, cs
	mov		ds, ax
	mov		ax, 0xb800

	mov		es, ax

	%assign i	0
	%rep	total
		%assign	i	i+1
		mov		ax, 2
		mul 	word [sectors]
		mov		si, ax

		mov 	byte [es:si+ 160 * 2], '0'+(i % 10)
		add 	word[sectors], 1

		%if i == total
			jmp $

		%else
			jmp ( 0x1000 + i * 0x20 )
		%endif

		times (512 - ($ - $$) % 512) db 0x00
	%endrep
