#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <stddef.h>

void memcpy(char *dst, const char *src, size_t size);
void memset(char *dst, char c, size_t size);

#endif
