IA32_APIC_BASE_MSR equ 0x1B

init_apic:
	mov ecx, IA32_APIC_BASE_MSR
	rdmsr

	shr eax, 12

	xor cx, cx
	mov cl, [MAXPHYADDR]
	push cx
	cmp cl, 20
	jl shifted_phyaddr

	mov cl, 20
shifted_phyaddr:
	mov ebx, 0x1
	shl ebx, cl
	sub ebx, 1
	and eax, ebx

	pop cx
	cmp cl, 20
	jle short_phyaddr
	mov ebx, 0x1
	shl ebx, cl
	sub ebx, 1
	and edx, ebx
	sub cl, 21
	shl eax, cl
	or eax, edx ; eax = APIC Base

short_phyaddr:
	mov [APIC_BASE_ADDR], eax
	ret

APIC_BASE_ADDR: dd 0