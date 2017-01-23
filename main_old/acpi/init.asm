REVISION_OFFSET equ 15

ACPI_V1_SIZE equ 20
ACPI_V2_SIZE_EX equ 16

SDT_HDR_SIZE equ 36

init_acpi:
	call find_rsdp
	test eax, eax
	jz acpi_not_successful
	mov [RSDP_TABLE_PTR], eax
	push eax

	mov bl, [eax + REVISION_OFFSET]
	mov byte [ACPI_VERSION], bl
acpi_verify:
acpi_v1_verify:
	mov ecx, ACPI_V1_SIZE
	xor ebx, ebx
acpi_v1_verify_byte:
	add ebx, [eax]
	inc eax
	loop acpi_v1_verify_byte
	test bl, bl
	jnz acpi_not_verified

	mov bl, [ACPI_VERSION]
	test bl, bl
	jnz acpi_v2_verify

	pop eax
	mov eax, [eax + RSDT_OFFSET]
	mov [RSDT_PTR], eax

	jmp acpi_verified
	ret
acpi_v2_verify:
	push eax
	mov eax, ACPI_VERSION_2
	call print_string
	pop eax
	mov ecx, ACPI_V2_SIZE_EX
	xor ebx, ebx
acpi_v2_verify_byte:
	add ebx, [eax]
	inc eax
	loop acpi_v2_verify_byte
	test bl, bl
	jz acpi_v2_verified
acpi_not_verified:
	mov eax, ACPI_NOT_VERIFIED
	call print_string
acpi_not_successful:
	xor eax, eax
	ret
acpi_v2_verified:
	pop eax
	mov eax, [eax + RSDT_OFFSET]
	mov [RSDT_PTR], eax
acpi_verified:
	push eax
	mov eax, ACPI_VERIFIED
	call print_string
	pop eax

	call rsdt_verify
	test eax, eax
	jz acpi_not_successful

	mov eax, RSDT_FOUND
	call print_string

	mov eax, [RSDT_PTR]
	add eax, SDT_HDR_SIZE
	mov [RSDT_START], eax

	mov eax, [RSDT_LENGTH]
	sub eax, SDT_HDR_SIZE
	mov ebx, 4
	xor edx, edx
	div ebx
	mov [RSDT_PTR_COUNT], eax

	mov eax, FADT_SIG ; find FADT table
	call find_sdt
	test eax, eax
	jz acpi_not_successful

	mov [FADT_PTR], eax
	; FADT should be valid, we won't validate it

acpi_already_enabled:
	ret


RSDP_TABLE_PTR: dd 0
ACPI_VERSION: db 0

ACPI_VERIFIED: db '- ACPI checksum verified', 0
ACPI_NOT_VERIFIED: db '- ACPI not verified', 0
RSDT_FOUND: db '- RSDT found', 0
ACPI_VERSION_2: db '- ACPI V2', 0

%include "acpi/rsdp/find.asm"
%include "acpi/rsdt.asm"
%include "acpi/fadt.asm"