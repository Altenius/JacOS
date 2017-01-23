#include "memory.h"

void memcpy(char *dst, const char *src, size_t size) {
    __asm__("mov %0, %%rcx\n\t"
            "mov %1, %%rsi\n\t"
            "mov %2, %%rdi\n\t"
            "rep movsb"
            :
            : "r"(size), "r"(src), "r"(dst)
            : "rcx", "rsi", "rdi");
}

void memset(char *dst, char c, size_t size) {
    __asm__("mov %0, %%al\n\t"
            "mov %1, %%rdi\n\t"
            "mov %2, %%rcx\n\t"
            "rep stosb"
            :
            : "r"(c), "r"(dst), "r"(size)
            : "al", "rdi", "rcx");
}
