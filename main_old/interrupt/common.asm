GATE_INT_32 equ 0x8E00 ; 0b1000111000000000
GATE_TRAP_32 equ 0x8F00 ;0b1000111100000000
DPL_0 equ 0

com_ex: ; common exception
	mov eax, ex_caught
	call print_string
	jmp $
	iret

com_int: ; common interrupt
	mov eax, gp_caught
	call print_string
	iret

com_df:
	mov eax, df_caught
	call print_string
	iret

com_np:
	mov eax, np_caught
	call print_string
	pop eax
	jmp $
	iret

com_de:
	mov eax, de_caught
	call print_string
	pop eax
	jmp $
	iret

int_load_common:
	mov ecx, 21 ; 21
make_ex:
	push ecx
	lea eax, [ecx - 1]
	mov ebx, com_ex
	mov cx, GATE_INT_32
	mov dx, 0x08
	call register_int
	pop ecx
loop make_ex

	mov ecx, 256-21
make_int:
	push ecx
	lea eax, [ecx + 20]
	mov ebx, com_int
	mov cx, GATE_TRAP_32
	mov dx, 0x08
	call register_int
	pop ecx
loop make_int


	mov eax, 8
	mov ebx, com_df
	mov cx, GATE_INT_32
	mov dx, 0x08
	call register_int

	mov eax, 11
	mov ebx, com_np
	mov cx, GATE_INT_32
	mov dx, 0x08
	call register_int

	mov eax, 0
	mov ebx, com_de
	mov cx, GATE_INT_32
	mov dx, 0x08
	call register_int

	ret



gp_caught: db 'Caught interrupt', 0
ex_caught: db 'Caught exception', 0
df_caught: db 'Caught double fault', 0
np_caught: db 'Caught #NP', 0
de_caught: db 'Caught #DE', 0
