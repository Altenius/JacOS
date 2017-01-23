SDT_LENGTH_OFFSET equ 4
RSDT_OFFSET equ 16
XSDT_OFFSET equ 24

; eax = start of RSDT
; failed if eax = 0
rsdt_verify:
	mov esi, eax
	mov eax, [eax + SDT_LENGTH_OFFSET]
	mov [RSDT_LENGTH], eax

	mov ecx, eax
	xor eax, eax
	xor ebx, ebx
rsdt_verify_byte:
	lodsb
	add ebx, eax
	loop rsdt_verify_byte
	test bl, bl
	jz rsdt_success
	xor eax, eax
rsdt_success:
	ret

; eax = sdt signature
; failed it eax = 0, else eax = pointer to SDT
find_sdt:
	push ecx
	push edi
	mov edi, [RSDT_START]
	mov ecx, [RSDT_PTR_COUNT]
search_next_sdt:
	mov ebx, [edi]
	mov ebx, [ebx]
	cmp eax, ebx
	je found_sdt
	add edi, 4
loop search_next_sdt

	xor eax, eax
	jmp end_find_sdt
found_sdt:
	mov eax, [edi]
end_find_sdt:
	pop edi
	pop ecx
	ret

RSDT_LENGTH: dd 0
RSDT_PTR: dd 0
RSDT_START: dd 0
RSDT_PTR_COUNT: dd 0