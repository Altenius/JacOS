#include "paging.h"
#include "../arch/detect_mem.h"
#include "../util/string.h"
#include "../util/memory.h"
#include "../vga/vga.h"

#define MEM_BEG 0x40000

mem_segment_t *page_segments;
uint16_t segment_count;


void *alloc_pages(size_t pages) {
    /*size_t page_sizes[BLOCK_COUNT];
    memset((char*)page_sizes, 0, sizeof(page_sizes));

    page_sizes[0] = pages;
    int top;
    for (top = 0; top < BLOCK_COUNT - 1 && pages != 0; top++) {
        if (pages > 1) {
            page_sizes[top] = pages % 2;
            pages /= 2;
        } else {
            page_sizes[top] = pages % 2;
            pages = 0;
            break;
        }
    }
    if (pages != 0) {
        top++;
        page_sizes[BLOCK_COUNT - 1] = pages;
    }

    for (int i = 0; i < segment_count; i++) {
        size_t search = page_sizes[top];
        block_t mask = (1 << (search % sizeof(block_t))) - 1;
        size_t post = 0;
        if (search > sizeof(block_t)) {
            post = search / sizeof(block_t);
        }
        block_t *block = page_segments[i].blocks[top];
        block_t *block_end = block + page_segments[i].byte_sizes[top];
        while (block != block_end - )
        for (int b = 0; b < page_segments[i].byte_sizes[top]; b++) {
            block_t block = page_segments[i].blocks[top][b];

        }
    }*/

    return NULL;
}

void init_paging(void) {
    memory_range_t *range = memory_layout(&segment_count);

    page_segments = (mem_segment_t*)MEM_BEG;

    char *mem_ptr = (char*)&page_segments[segment_count];

    for (int i = 0; i < segment_count; i++) {
        page_segments[i].address = range[i].addr;
        page_segments[i].length = range[i].length;
        uint32_t pages = range[i].length / 4096;

        for (int k = 0; k < BLOCK_COUNT; k++) {
            uint32_t bytesize = pages / sizeof(block_t);
            if (pages % sizeof(block_t) != 0) {
                bytesize++;
            }
            page_segments[i].block_sizes[k] = pages;
            page_segments[i].byte_sizes[k] = bytesize;
            page_segments[i].blocks[k] = (block_t*)mem_ptr;
            memset(mem_ptr, 0, bytesize);
            mem_ptr += bytesize;
            pages /= 2;
        }
    }
}
