#ifndef DETECT_MEM_H
#define DETECT_MEM_H
#include <stdint.h>

#define MEM_MAX_ENTRIES 30

namespace ShitOS {
	typedef struct {
	    uint64_t addr;
	    uint64_t length;
	    uint32_t type;
	} __attribute__((packed)) memory_range_t;

	memory_range_t *memory_layout(uint16_t *entries);
	void detect_memory(void);
}

#endif
