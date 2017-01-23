load_idt:
	; clear idt memory
	mov ecx, 512
	mov edi, idt_start
	xor eax, eax
	rep stosd

	call int_load_common

	lidt [idt_descriptor]

	sti
	ret

%include "interrupt/idt.asm"
%include "interrupt/register.asm"
%include "interrupt/common.asm"
