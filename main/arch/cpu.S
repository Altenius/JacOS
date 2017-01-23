.intel_syntax noprefix
.section .text

.global cpuid_supported
.type cpuid_supported, @function
cpuid_supported:
    /* Test ID flag in EFLAGS */
    pushfq
    or dword ptr [rsp], 1 << 21
    popfq
    pushfq
    test dword ptr [rsp], 1 << 21
    popfq
    jz cp_unsupported
    mov rax, 1
    ret
    cp_unsupported:
    xor rax, rax
    ret


.global cpu_maxphyaddr
.type cpu_maxphyaddr, @function
cpu_maxphyaddr:
    push rbx
    push rcx
    push rdx
    mov rax, 0x80000008
    cpuid
    and eax, 0x000000FF
    pop rdx
    pop rcx
    pop rbx
    ret
