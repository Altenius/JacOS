.intel_syntax noprefix
.section .text

.equ idt_addr, 0x20000

.global idt_load
.type idt_load, @function
idt_load:
    lidt [idt_descriptor]
    ret


.section .data
.align 8
idt_descriptor:
    .short (256 * 8) - 1
    .int idt_addr
