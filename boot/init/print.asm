print_string:
	push ax
	push bx
	mov ah, 0x0E

	mov byte al, [bx]
	print_char:
	int 0x10
	inc bx
	mov al, [ds:bx]
	cmp al, 0
	jne print_char

	pop bx
	pop ax
	ret