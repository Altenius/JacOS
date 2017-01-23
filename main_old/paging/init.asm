PAGE_PML4 equ 0x1000

; with CR4.PCIDE = 0
init_paging:
    mov eax, PAGE_PML4 << 12
    mov cr3, eax

    call make_pml4

    mov ecx, 8192
    call alloc_mem

    ret


make_pml4:
    push edi
    mov ecx, 1024
    xor eax, eax
    mov edi, PAGE_PML4
    rep stosd

    pop edi
    ret

; ecx = amount of memory to allocate
alloc_mem:
    push eax
    push ebx
    push ecx
    push edx

    sub ecx, 1
    shr ecx, 12
    add ecx, 1 ; eax = # of pages

    mov eax, [loc_index]
    mov ebx, [mem_index]

    am_page:
    call map_page_4k

    add eax, 4096
    add ebx, 4096
    loop am_page
    mov [loc_index], eax
    mov [mem_index], ebx

    alloc_mem_end:
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

get_free_page: ; ecx = physical address of next free page
    
    ret


; eax = linear address
; ebx = physical address
map_page_4k: ; maps a 4 kB page
    push edx
    push ecx
    push eax
    mov edx, eax

    shr edx, 12
    mov ecx, edx
    and ecx, 0x1FF
    push ecx ; PTE

    shr edx, 9
    mov ecx, edx
    and ecx, 0x1FF
    push ecx ; PDE

    shr edx, 9
    mov ecx, edx
    and ecx, 0x1FF
    push ecx ; PDPTE

    shr edx, 9
    mov ecx, edx
    and ecx, 0x1FF
    ;push ecx ; PML4E

    mov eax, PAGE_PML4
    shl ecx, 3 ; ecx * 8
    lea eax, [eax + ecx]
    mov ecx, [eax]
    test ecx, 1
    jnz mp4k_pml4p

    mov edx, 0xE0


    mov [eax], edx
    mp4k_pml4p: ; PML4 is present

    add esp, 16

    pop eax
    pop ecx
    pop edx
    ret

loc_index: dd 0
mem_index: dd 0


PML4_NP: db 'PML4 not present', 0
