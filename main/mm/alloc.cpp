#include "alloc.h"
#include "memm.h"
#include "../vga/vga.h"
#include "../util/string.h"
#include <stdint.h>

namespace JacOS {
	namespace Heap {
		extern uint64_t end;
		uint32_t __attribute__((aligned(4))) kernel_heap[8192]; // 32 KiB. uint32_t ensures addresses are 4-byte aligned
		uint32_t *heap_loc = kernel_heap;
		uint32_t *heap_end = kernel_heap + sizeof(kernel_heap)/sizeof(uint32_t);

		void Init() {
			Vga::PrintString("Heap location: ");
			Vga::PrintString(uqtohs((uint64_t)kernel_heap));
			Vga::PrintString("\n");


		}

		// TODO: Proper heap allocation

		void *Allocate(uint64_t size) {
			size = (size + sizeof(uint32_t) - 1) / sizeof(uint32_t);
			if (heap_loc + size >= heap_end) {
				return nullptr;
			}

			void *addr = (void*)heap_loc;
			heap_loc += size;
			return addr;
		}

		void Deallocate(void *memory) {
			// Does nothing
		}
	}
}


void *operator new(unsigned long a) {
	return JacOS::Heap::Allocate(a);
}