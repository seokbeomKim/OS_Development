[bits 32]

global kReadCPUID, kEnter64bitKernel

SECTION .text

; CPU ID 반환
; PARAM: DWORD dwEAX, DWORD* pdwEAX, DWORD* pdwEBX, DWORD* pdwECX, DWORD* pdwEDX
kReadCPUID:
	push	ebp
	mov		ebp, esp
	push	eax
	push	ebx
	push	ecx
	push	edx
	push	esi

	mov		eax, dword [ebp + 8]
	cpuid

	; cpuid 반환값 저장
	mov		esi, dword [ebp + 12]
	mov		dword [esi], eax

	mov		esi, dword [ebp + 16]
	mov		dword [esi], ebx

	mov		esi, dword [ebp + 20]
	mov		dword [esi], ecx

	mov		esi, dword [ebp + 24]
	mov		dword [esi], edx

	pop		esi
	pop		edx
	pop		ecx
	pop		ebx
	pop		eax
	pop		ebp

	ret

kEnter64bitKernel:
	; cr4 control register
	mov		eax, cr4
	or		eax, 0x20
	mov		cr4, eax

	; cr3 control register: 캐시 기능 활성화해야하므로 PCD, PWT 비트는 모두 0으로 설정한다.
	mov		eax, 0x100000
	mov		cr3, eax

	mov		ecx, 0xc0000080
	rdmsr

	or		eax, 0x0100
	wrmsr

	mov		eax, cr0
	or		eax, 0xE0000000
	xor		eax, 0x60000000
	mov		cr0, eax
	jmp		0x08:0x200000

