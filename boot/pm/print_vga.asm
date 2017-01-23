[bits 32]

VGA_OFFSET equ 0xB8000

print_string_pm:
	push ebx
	push edx
	mov ebx, VGA_OFFSET
	mov dx, [pos]
	add bx, dx

	print_char_pm:
	mov byte dl, [eax]
	mov byte [ebx], dl
	inc ebx
	mov byte [ebx], 0x0F
	inc ebx
	mov dx, [pos]
	add dx, 2
	mov [pos], dx
	inc eax
	cmp byte [eax], 0
	jne print_char_pm

	pop edx
	pop ebx
	ret

clear_vga:
	push ebx

	mov ebx, VGA_OFFSET

	mov ecx, 1000
	clear_vga_bytes:
	mov dword [ebx], 0
	add ebx, 4
	loop clear_vga_bytes

	pop ebx
	ret

pos: dd 0