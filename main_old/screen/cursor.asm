update_cursor:
	push dx
	push eax
	push ebx

	mov eax, [vga_pos]
	mov ebx, 2
	xor edx, edx
	div ebx
	mov ebx, eax

	mov dx, 0x3D4
	mov al, 0x0F
	out dx, al

	mov dx, 0x3D5
	mov al, bl
	out dx, al

	mov dx, 0x3D4
	mov al, 0x0E
	out dx, al

	mov dx, 0x3D5
	mov al, bh
	out dx, al

	pop ebx
	pop eax
	pop dx
	ret