.section .text

.global idt_load
.type idt_load, @function
idt_load:
	lidt (idt_descriptor)
	ret


.global isr0
.type isr0, @function
isr0:
	pushq $0 /* error code */
	pushq $0 /* vector */
	jmp interrupt_generic


.global isr1
.type isr1, @function
isr1:
	pushq $0 /* error code */
	pushq $1 /* vector */
	jmp interrupt_generic


.global isr2
.type isr2, @function
isr2:
	pushq $0 /* error code */
	pushq $2 /* vector */
	jmp interrupt_generic


.global isr3
.type isr3, @function
isr3:
	pushq $0 /* error code */
	pushq $3 /* vector */
	jmp interrupt_generic


.global isr4
.type isr4, @function
isr4:
	pushq $0 /* error code */
	pushq $4 /* vector */
	jmp interrupt_generic


.global isr5
.type isr5, @function
isr5:
	pushq $0 /* error code */
	pushq $5 /* vector */
	jmp interrupt_generic


.global isr6
.type isr6, @function
isr6:
	pushq $0 /* error code */
	pushq $6 /* vector */
	jmp interrupt_generic


.global isr7
.type isr7, @function
isr7:
	pushq $0 /* error code */
	pushq $7 /* vector */
	jmp interrupt_generic


.global isr8
.type isr8, @function
isr8:
	/* error code is already on stack */
	pushq $8 /* vector */
	jmp interrupt_generic


.global isr9
.type isr9, @function
isr9:
	pushq $0 /* error code */
	pushq $9 /* vector */
	jmp interrupt_generic


.global isr10
.type isr10, @function
isr10:
	/* error code is already on stack */
	pushq $10 /* vector */
	jmp interrupt_generic


.global isr11
.type isr11, @function
isr11:
	/* error code is already on stack */
	pushq $11 /* vector */
	jmp interrupt_generic


.global isr12
.type isr12, @function
isr12:
	pushq $0 /* error code is already on stack */
	pushq $12 /* vector */
	jmp interrupt_generic


.global isr13
.type isr13, @function
isr13:
	/* error code is already on stack */
	pushq $13 /* vector */
	jmp interrupt_generic


.global isr14
.type isr14, @function
isr14:
	/* error code is already on stack */
	pushq $14 /* vector */
	jmp interrupt_generic


.global isr15
.type isr15, @function
isr15:
	pushq $0 /* error code */
	pushq $15 /* vector */
	jmp interrupt_generic


.global isr16
.type isr16, @function
isr16:
	pushq $0 /* error code */
	pushq $16 /* vector */
	jmp interrupt_generic


.global isr17
.type isr17, @function
isr17:
	/* error code is already on stack */
	pushq $17 /* vector */
	jmp interrupt_generic


.global isr18
.type isr18, @function
isr18:
	pushq $0 /* error code */
	pushq $18 /* vector */
	jmp interrupt_generic


.global isr19
.type isr19, @function
isr19:
	pushq $0 /* error code */
	pushq $19 /* vector */
	jmp interrupt_generic


.global isr20
.type isr20, @function
isr20:
	pushq $0 /* error code */
	pushq $20 /* vector */
	jmp interrupt_generic

.global irq0
.type isr0, @function
irq0:
	pushq $0 /* error code */
	pushq $32 /* vector */
	jmp interrupt_generic

.global irq1
.type isr1, @function
irq1:
	pushq $0 /* error code */
	pushq $33 /* vector */
	jmp interrupt_generic

.global resume_process
.type resume_process, @function
resume_process:
	mov %rsi, %cr3
	mov %rdi, %rsp
	jmp int_resume

interrupt_generic:
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp

	movq %ds, %rax
	pushq %rax

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es

	mov %rsp, %rdi /* interrupt context */
	call interrupt_handler

int_resume:

	popq %rax
	movw %ax, %ds
	movw %ax, %es

	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8

	add $16, %rsp /* get rid of error code and vector */


	iretq


.section .data
.align 8
idt_descriptor:
	.short (256 * 16) - 1
	.quad idt