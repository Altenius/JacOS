[bits 16]

load_main:
	clc
	mov ax, 0x1000
	mov es, ax
	mov ah, 2
	mov al, 16 ;16 sectors
	mov ch, 0 ;cylinder 0
	mov cl, 2 ;cylinder 0 and sector 2
	mov dh, 0 ;head 0
	mov bx, 0 ;put ShitOS at 0x10000
	mov dl, [drive_number]
	int 0x13
	jc load_main_failed
	xor ax, ax
	mov es, ax
	ret


load_main_failed:
	xor ax, ax
	mov es, ax
	mov ebx, FAILED
	call print_string
	jmp $

FAILED: db 'Load fail', 0
