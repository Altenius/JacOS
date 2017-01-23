bits 32
org 0x7e00

main:
	call clear_vga

	mov eax, BOOTED
	call print_string

	call init_acpi
	test eax, eax
	jz acpi_no
	mov eax, ACPI_FOUND
	call print_string
	call update_cursor

	pushfd
	xor dword [esp], 0x200000
	popfd
	pushfd
	test dword [esp], 0x200000
	jz cpuid_no

	mov eax, CPUID_FOUND
	call print_string

	mov eax, 0x80000008
	cpuid
	test eax, eax
	jz maxpaddr_not_supported

	mov [MAXPHYADDR], al

maxpaddr_not_supported:
	mov eax, 0x01
	cpuid
	mov eax, edx
	and eax, 1000000000b ; set if APIC is present
	jnz apic_supported
	mov eax, APIC_NOT_SUPPORTED
	call print_string
apic_supported:
	and edx, 100000b ; set if MSRs are supported
	shr edx, 5
	mov [MSR_SUPPORT], dl
	test edx, edx
	jnz msr_supported
	mov eax, MSR_NOT_SUPPORTED
	call print_string
msr_supported:
	call load_idt
	call init_apic

	call update_cursor

	mov eax, TEST_MSG
	call print_string

	xor eax, eax
	xor edi, edi
	mov ecx, 25
	rep stosd

	mov edi, 8
	mov ax, 2 << 3
	stosw
	mov edi, 16
	stosw
	mov edi, 24
	stosw
	mov edi, 72
	stosw
	add edi, 2
	mov ax, 1 << 3
	stosw
	add edi, 2
	mov ax, 2 << 3
	stosw
	add edi, 2
	stosw
	add edi, 2
	stosw
	add edi, 2
	stosw

	mov ax, 5 << 3
	ltr ax

	call init_paging
	call init_ia32e

	xor edx, edx
	mov eax, 1
	xor ebx, ebx
	div ebx

	jmp $

acpi_no:
	mov eax, ACPI_NOT_FOUND
	call print_string
	call update_cursor
	jmp $
cpuid_no:
	mov eax, CPUID_NOT_FOUND
	call print_string
	call update_cursor
	jmp $

%include "paging/init.asm"
%include "ia32e.asm"
%include "util.asm"
%include "acpi/init.asm"
%include "screen/print.asm"
%include "interrupt/load.asm"
%include "apic/init.asm"

ACPI_NOT_FOUND: db 'X ACPI not loaded', 0
ACPI_FOUND: db '> ACPI loaded', 0
BOOTED: db '> Booted TrashOS', 0
CPUID_NOT_FOUND: db 'X CPUID instruction not supported', 0
CPUID_FOUND: db '> CPUID instruction supported', 0
MSR_NOT_SUPPORTED: db '> MSRs are not supported, may be unstable', 0
APIC_NOT_SUPPORTED: db '> APIC is not supported, may be unstable', 0


TEST_MSG: db 'Test message', 0

MAXPHYADDR: db 36
MSR_SUPPORT: db 0

;times 8192 - ($-$$) db 0
