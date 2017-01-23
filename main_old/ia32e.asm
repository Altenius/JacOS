init_ia32e:
    mov eax, cr4
    or eax, 1 << 5 ; CR4.PAE
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8 ; I32_EFFER.LME
    wrmsr

    ret
