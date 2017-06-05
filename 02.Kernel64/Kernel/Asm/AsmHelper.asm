[bits 64]

section .text

; AsmHelper.asm: 而ㅻ꼸�뿉�꽌 �궗�슜�릺�뒗 �뼱�뀍釉붾━ 肄붾뱶
global kReadFromPort, kWriteToPort, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC

kReadFromPort:
	push 	rdx

	mov		rdx, rdi	; rax �젅吏��뒪�꽣�뿉 �뙆�씪誘명꽣 1 ���옣
	mov		rax, 0
	in		al, dx		; dx �젅吏��뒪�꽣�뿉 ���옣�맂 �룷�듃 �뼱�뱶�젅�뒪�뿉�꽌 �븳 諛붿씠�듃瑜� �씫�쓬
						; al �젅吏��뒪�꽣�뿉 ���옣, AL �젅吏��뒪�꽣�뒗 �븿�닔�쓽 諛섑솚 媛믪쑝濡� �궗�슜�맂�떎.

	pop		rdx
	ret

kWriteToPort:
	push	rdx
	push	rax

	mov		rdx, rdi	; rdx �젅吏��뒪�꽣�뿉 �뙆�씪誘명꽣 1�쓣 ���옣
	mov		rax, rsi	; rax �젅吏��뒪�꽣�뿉 �뙆�씪誘명꽣 2瑜� ���옣
	out		dx, al		; dx �젅吏��뒪�꽣�뿉 al�뿉 ���옣�맂 �뜲�씠�꽣媛� ���옣

	pop		rax
	pop		rdx
	ret

kLoadGDTR:
	lgdt	[rdi]		; GDTR �뼱�뱶�젅�뒪 (�뙆�씪誘명꽣 1) 濡쒕뱶 諛� GDT �뀒�씠釉� �깮�꽦
	ret

kLoadTR:
	ltr		di			; TSS �꽭洹몃㉫�듃 �뵒�뒪�겕由쏀꽣 �삤�봽�뀑 �씠�슜�븯�뿬 �꽭洹몃㉫�듃 濡쒕뱶
	ret

kLoadIDTR:
	lidt	[rdi]		; IDT �뀒�씠釉붿쓣 �꽕�젙
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

kReadTSC:
	push	rdx
	rdtsc				; 타임 스탬프 카운터 읽어서 RDX:RAX에 저장
	shl		rdx, 32		; RDX 레지스터에 있는 상위 32비트 TSC 값과 RAX 레지스터에 있는
	or		rax, rdx	; 하위 32비트 TSC 값을 OR하여 RAX 레지스터에 64bit TSC 값 저장

	pop rdx
	ret
