align 8

idt_start equ 0x10000
idt_end equ idt_start + (256 * 8)

idt_descriptor:
	dw idt_end - idt_start - 1 ; limit
	dd idt_start ; base
