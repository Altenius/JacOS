#ifndef MEMM_H
#define MEMM_H

#include "../multiboot.h"

namespace ShitOS {
	namespace Memory {
		void Init(multiboot_info_t *bootinfo);
	}

	namespace Page {
		struct PML {
			uint64_t entries[512];
		};

		uint64_t GetPage();

		inline uint64_t AlignDown(uint64_t addr) {
			return addr & ~(0xFFFUL);
		}

		inline uint64_t AlignUp(uint64_t addr) {
			return AlignDown(addr + 0xFFFUL);
		}

		#define PML_PRESENT 1
		#define PML_READWRITE (1 << 1)
		#define PML_USERMODE (1 << 2)
		#define PML_CACHEDISABLE (1 << 4)
		#define PML_ACCESSED (1 << 5)

		void Map(uint64_t phys_addr, uint64_t virt_addr, int prot = 0);

		void AddPage(uint64_t addr);
	}
}

#endif