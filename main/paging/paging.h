#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>
#include <stddef.h>

#define BLOCK_COUNT 5
#define MAX_BLOCK_SIZE (1 << (BLOCK_COUNT - 1)) // Max block size 2^4 (16 pages)

#define block_t uint64_t

void init_paging(void);

void *alloc_pages(size_t pages);

typedef struct {
    uint64_t address;
    uint64_t length;
    block_t *blocks[BLOCK_COUNT];
    uint32_t block_sizes[BLOCK_COUNT]; // in bits
    uint32_t byte_sizes[BLOCK_COUNT]; // block size in block_t
} mem_segment_t;

#endif
