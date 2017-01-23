[bits 32]

%include "pm/print_vga.asm"

pm_start:
	cli
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	mov ax, DATA_SEG_0
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call clear_vga

	mov eax, pm_greeting
	call print_string_pm

	jmp 0x10000 ;jump to trash


pm_greeting: db "Switched to protected mode", 0
