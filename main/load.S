.intel_syntax noprefix

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .text
.text 0x100000
    # Multiboot header.
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .rodata
.align 16
gdt64:
    .word 0, 0
    .byte 0, 0, 0, 0 /* null descriptor */
    lgdt_code: /* 0x8 */
    .word 0, 0
    .byte 0
    .byte 0b10011010
    .byte 0b00100000
    .byte 0
    lgdt_data: /* 0x10 */
    .word 0, 0
    .byte 0
    .byte 0b10010010
    .byte 0b00000000
    .byte 0
    tss_desc:
    .word 0x67 /* sizeof TSS? */
    .word 0 /* skip base addr */
    .word 0b1000100100000000
    .word 0
    .word 0, 0, 0, 0
    gdt_ptr:
    .word gdt_ptr - gdt64 - 1 /* limit */
    .quad gdt64

.equ GDT_CODE, lgdt_code - gdt64
.equ GDT_DATA, lgdt_data - gdt64

.section .bss, "aw", @nobits
p4_table:
    .align 4096
    .skip 4096
p3_table:
    .skip 4096
p2_table:
    .skip 4096

mem3_table:
    .skip 4096
mem2_table:
    .skip 4096
mem1_table:
    .skip 4096
mem0_page:
    .skip 4096

pcache_table:
    .skip 4096

stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
.code32
_start:
    mov esp, offset stack_top

    mov [tss + 4], esp /* move to rsp0 */
    mov ecx, tss
    mov [tss_desc + 2], cx /* base 15:00 */
    shr ecx, 16
    mov [tss_desc + 4], cl /* base 23:16 */
    mov [tss_desc + 7], ch /* base 31:24 */



    pushd 0
    push eax /* multiboot magic value */
    pushd 0
    push ebx /* multiboot information */

    /* Map first P4 entry to P3 table */
    mov eax, offset p3_table
    or eax, 0b11
    mov [p4_table], eax

    /* Map first P3 entry to P2 table */
    mov eax, offset p2_table
    or eax, 0b11
    mov [p3_table], eax

    /* Map the pcache table to P2 table */
    mov eax, offset pcache_table
    or eax, 0b11
    mov [p2_table + 2 * 8], eax

    /* Map the pcache table to itself */
    mov eax, offset pcache_table
    or eax, 0b11
    mov [pcache_table + 3 * 8], eax

    /* Map each P2 entry to 2MiB page */
    mov edi, p2_table
    xor ecx, ecx
map_p2_table:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [p2_table + ecx * 8], eax

    inc ecx
    cmp ecx, 2
    jne map_p2_table

    /* Map the last address to 

    /* Map p4 to its last entry (0xFF80000000000000) */
    mov eax, offset p4_table
    or eax, 0b11
    mov [p4_table + 511 * 8], eax
    

    /* Map memory for the page stack */
    mov eax, offset mem3_table
    or eax, 0b11
    mov [p4_table + 510 * 8], eax

    mov eax, offset mem2_table
    or eax, 0b11
    mov [mem3_table + 0 * 8], eax

    mov eax, offset mem1_table
    or eax, 0b11
    mov [mem2_table + 0 * 8], eax

    mov eax, offset mem0_page
    or eax, 0b11
    mov [mem1_table + 0 * 8], eax


    /* Move p4_table address to cr3 */
    mov eax, offset p4_table
    mov cr3, eax

    /* Enable PAE */
    mov eax, cr4
    or eax, 1 << 5 /* Set PAE-bit */
    mov cr4, eax

    /* Set IA32_EFER.LME */
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    /* Set CR0.PG */
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt_ptr]
    jmp GDT_CODE:load_64



setup_sse:
    mov eax, 0x1
    cpuid
    test edx, 1 << 25
    jz no_sse

    mov eax, cr0
    and ax, 0xFFFB
    or ax, 0x2
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9
    mov cr4, eax

    xor eax, eax
    ret
no_sse:
    mov eax, 1
    ret

.code64
load_64:
    mov ax, GDT_DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    pop rsi /* multiboot information */
    pop rdi /* multiboot magic number */
    call kernel

    h:
    hlt
    jmp h
