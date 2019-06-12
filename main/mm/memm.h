#ifndef MEMM_H
#define MEMM_H

#include "../multiboot.h"

namespace JacOS {
	namespace Memory {
		void Init(multiboot_info_t *bootinfo);
	}

	namespace Page {
		struct PML {
			uint64_t entries[512];
		};

		PML * const pml4 = (PML*) 0xFFFFFFFFFFFFF000UL;
		PML * const pcache_table = (PML*) 0x0000000000403000UL;
		PML * const pcache_page = (PML*) 0x0000000000404000UL;
		PML * const pcache_pml = (PML*) 0x0000000000405000UL;
		PML * const proc_map_pml = (PML*) 0x0000000000406000UL;
		uint64_t * const stack = (uint64_t*) 0xFFFFFF0000000000UL;

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

		void Map(uint64_t phys_addr, uint64_t virt_addr, int prot = 0, PML *pml = pml4);

		void AddPage(uint64_t addr);

		inline void InvalidatePage(uint64_t addr) {
			__asm__ volatile ("invlpg (%0)" : : "b"(addr) : "memory");
		};
	}
}

#endif