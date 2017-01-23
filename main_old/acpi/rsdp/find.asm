bits 32
find_rsdp:
	mov edi, [0x40E] ;EBDA address
	mov ecx, 0x400 ;1 KiB
	mov ebx, rsdp_string
	call find_string ; the rsdp signature is always on a 16 byte boundary; this function could be patched.
	test eax, eax
	jz not_in_ebda
	ret
no_rsdp:
	xor eax, eax
	ret
not_in_ebda:
	mov eax, NOT_IN_EBDA
	call print_string

	mov edi, 0xE0000
	mov ecx, 0x1FFFF  ;read to 0xFFFFF
	mov ebx, rsdp_string
	call find_string
	test eax, eax
	jz no_rsdp
	ret

rsdp_string: db 'RSD PTR ', 0
NOT_IN_EBDA: db '- RSDP not in EBDA, searching memory', 0