[bits 16]
org 0x7c00

MEM_MAP_START equ 0x6000
MEM_MAP_SIZE equ 0x5998

main:
	cli
	; zero all registers except dx (dl has the drive number we still need to copy)
	xor ax, ax
	xor bx, bx
	xor cx, cx
	xor si, si
	xor di, di
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x9000
	mov esp, ebp

	mov [drive_number], dl
	xor dx, dx

	call check_A20
	call detect_mem
	call load_main

	jmp init_pm

drive_number: db 0

%include "init/print.asm"
%include "init/gdt.asm"
%include "load_main.asm"
%include "init/a20.asm"

[bits 16]
detect_mem:
	mov word [MEM_MAP_SIZE], 0
	xor ebx, ebx
	mov di, MEM_MAP_START

	next_mem:
	mov eax, 0xE820
	mov dword [es:di + 20], 1
	mov ecx, 24
	mov edx, 0x0534D4150 ; SMAP
	int 0x15
	jc detect_mem_end
	push ax
	mov ax, [MEM_MAP_SIZE]
	add ax, 1
	mov [MEM_MAP_SIZE], ax
	pop ax
	add di, 20
	test ebx, ebx
	jz detect_mem_end
	test ecx, ecx
	jnz next_mem

	detect_mem_end:
	ret

init_pm:
	cli ;disable interrupts
	lgdt [gdt_descriptor]

	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp CODE_SEG_0:pm_start


%include "pm/pm.asm"

times (510-($-$$)) db 0
dw 0xAA55
