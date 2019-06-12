#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>

namespace JacOS {
	namespace Heap {
		void *Allocate(uint64_t size);
		void Deallocate(void *memory);

		void Init();
	}
}

#endif