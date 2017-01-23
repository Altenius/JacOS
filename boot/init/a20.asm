bits 16

; This was annoying to fix
check_A20:
	push ds
	push es
	push ax
	push di

	xor ax, ax
	mov ds, ax

	not ax ; ax = 0xFFFF
	mov es, ax

	mov di, 0x7DFE
	mov si, 0x7E0E

	mov bl, [ds:di]
	push bx
	mov bl, [es:si]
	push bx

	mov byte [ds:di], 0xFF
	mov byte [es:si], 0x00

	mov bl, [ds:di]
	test bl, bl

	pop bx
	mov [ds:di], bl
	pop bx
	mov [es:di], bl

	jz enable_A20

	pop di
	pop ax
	pop es
	pop ds
	ret

enable_A20:
	in al, 0x92
	or al, 2
	out 0x92, al

	pop di
	pop ax
	pop es
	pop ds
	ret

ps2_write_wait:
	in al, 0x64
	test al, 0x2 ; input buffer state
	jnz ps2_write_wait
	ret

ps2_read_wait:
	in al, 0x64
	test al, 1 ; output buffer state
	jz ps2_read_wait
	ret
