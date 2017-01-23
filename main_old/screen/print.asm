VGA_OFFSET equ 0xB8000

print_string:
	push eax
	push ebx
	push edx

	xor edx, edx
	mov dx, [vga_pos]

	mov ebx, VGA_OFFSET
	add ebx, edx
print_char_pm:
	mov byte dl, [eax]
	mov byte [ebx], dl
	inc ebx
	mov byte [ebx], 0x0F
	inc ebx
	mov dx, [vga_pos]
	add dx, 2
	mov [vga_pos], dx
	inc eax
	cmp byte [eax], 0
	jne print_char_pm

	mov ax, [vga_pos]
	mov bx, 160
	xor dx, dx
	div bx
	inc ax
	mul bx
	mov word [vga_pos], ax

	pop edx
	pop ebx
	pop eax
	ret

print_hex:
	push eax
	push ebx
	push ecx
	push dx
	mov ebx, hex_string_end
	mov ecx, 8
test_hex:
	mov dl, al
	and dl, 0x0F
	cmp dl, 0x09
	jg print_hex_alpha
	add dl, 48
	jmp hex_next_char
print_hex_alpha:
	add dl, 55
hex_next_char:
	mov [ebx], dl
	cmp ebx, hex_string
	shr eax, 4
	dec ebx
	loop test_hex
print_hex_final:
	mov eax, hex_string
	call print_string
	pop dx
	pop ecx
	pop ebx
	pop eax
	ret

clear_vga:
	push ebx
	push ecx

	mov ebx, VGA_OFFSET

	mov ecx, 1000
	clear_vga_bytes:
	mov dword [ebx], 0x0F000F00
	add ebx, 4
	loop clear_vga_bytes

	pop ecx
	pop ebx
	ret

%include "screen/cursor.asm"

vga_pos: dw 0
hex_string: db '0x00000000', 0
hex_string_end equ hex_string+9