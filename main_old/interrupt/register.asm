; eax = vector #
; ebx = pointer
; cx = type | DPL
; dx = segment selector
register_int:
	push ebx
	push dx

	mov ebx, 8
	mul ebx
	add eax, idt_start

	pop dx
	pop ebx

	mov edi, eax
	mov ax, bx
	stosw ; store lower 16 bits of eax in [edi]

	mov ax, dx
	stosw ; store segment selector

	mov ax, cx
	stosw

	shr ebx, 16
	mov ax, bx
	stosw

	ret
