;ebx = string to find
;edi =  string to search
find_string:
	push ebx
	push edx
	push ecx
	cld
find_string_next:
	mov al, [ebx]
	align 4
find_loop:
	scasb
	je found_first_char
	loop find_loop
	jmp string_not_found
found_first_char:
	mov edx, ebx
find_next:
	test ecx, ecx
	jz string_not_found
	inc ebx
	dec ecx
	mov al, [ebx]
	test al, al
	jz found_string
	scasb
	je find_next
	mov ebx, edx
	jmp find_string_next
string_not_found:
	xor eax, eax
	jmp find_string_end
found_string:
	mov eax, edi

	sub ebx, edx
	sub eax, ebx
find_string_end:
	pop ecx
	pop edx
	pop ebx
	ret

;edi = string1; esi = string2; ecx = length
;eax = 0  if failed
compare_strings:
	push esi
	push edi
compare_byte:
	mov al, [esi]
	scasb
	jne compare_not_equal
	inc esi
	loop compare_byte
	mov eax, 1
	jmp compare_ret
compare_not_equal:
	xor eax, eax
compare_ret:
	pop edi
	pop esi
	ret