#include "../vga/vga.h"
#include <stddef.h>
#include <stdint.h>
#include "../util/string.h"
#include "../util/memory.h"
#include "memm.h"

namespace JacOS {
	extern uint64_t end;

	namespace Page {
		size_t stack_size = 512;
		size_t stack_used = 0;

		static void ExtendStack();
	}

	namespace Memory {
		uint64_t end_page;

		static void CheckMemoryUsed(uint64_t &addr, uint64_t &length) {
			if (addr <= end_page) {
				if (length <= end_page - addr) {
					length = 0;
					return;
				}

				length -= (end_page - addr);
				addr = end_page;
			}

		}

		static bool PageUsable(uint64_t addr) {
			return addr != 0xFEE0000;
		}

		void Init(multiboot_info_t *bootinfo) {
			// end_page = Page::AlignUp((uint64_t)(&end) + 1);
			end_page = 0x0000000000408000UL;

			if (!(bootinfo->flags & MULTIBOOT_INFO_MEM_MAP)) {
				Vga::PrintString("memory map not set\n");
				return;
			}

			multiboot_memory_map_t *entry = (multiboot_memory_map_t *)bootinfo->mmap_addr;

			for (; (uint64_t)entry < bootinfo->mmap_addr + bootinfo->mmap_length;

				entry = (multiboot_memory_map_t*)((uint64_t)entry + entry->size + sizeof(entry->size))) {
				Vga::PrintString(uqtohs(entry->addr));
				Vga::PrintString(" size ");
				Vga::PrintString(uqtohs(entry->len));
				Vga::PrintString(" type ");
				Vga::PrintString(uitohs(entry->type));
				Vga::PrintString("\n");

				if (entry->type != MULTIBOOT_MEMORY_AVAILABLE) {
					continue;
				}

				uint64_t addr = Page::AlignUp(entry->addr);
				uint64_t length = Page::AlignDown(entry->len);
				CheckMemoryUsed(addr, length);

				uint64_t astop = addr + length;

				for (; addr < astop; addr += 4096) {
					if (PageUsable(addr)) {
						Page::AddPage(addr);
					}
				}
			}

			Vga::PrintString("Stack used: ");
			Vga::PrintString(uqtos(Page::stack_used));
			Vga::PrintString(" | ");

			Vga::PrintString("Stack size: ");
			Vga::PrintString(uqtos(Page::stack_size));
			Vga::PrintString("\n");
		}
	}


	namespace Page {
		uint64_t GetPage() {
			if (stack_used == 0) {
				return 0;
			}

			return stack[--stack_used];
		}

		static inline void Flush() {
			uint64_t previous;
			asm ( "mov %%cr3, %0" : "=r"(previous) );
			asm volatile ( "mov %0, %%cr3" : : "r"(previous) );
		}

		void Map(uint64_t phys_addr, uint64_t virt_addr, int prot, PML *pml) {
			const size_t MASK = (1 << 9) - 1;
			uint16_t entries[4];

			for (int i = 0; i < 4; i++) {
				entries[i] = (virt_addr >> (12 + i * 9)) & MASK;
			}

			for (int i = 3; i > 0; i--) {
				uint64_t entry = pml->entries[entries[i]];

				if (!(entry & PML_PRESENT)) {
					// Present bit is not set

					uint64_t page = GetPage();
					if (!page) { // TODO: panic
						Vga::PrintString("out of memory\n");
						return;
					}

					entry = page | PML_PRESENT | prot;
					
					pml->entries[entries[i]] = entry;
					
					pcache_table->entries[4] = entry;
					InvalidatePage((uint64_t)pcache_page);

					memset((char*)pcache_page, 0, sizeof(PML));
				}

				pcache_table->entries[5] = entry;
				InvalidatePage((uint64_t)pcache_pml);
				pml = pcache_pml;
			}

			pml->entries[entries[0]] = phys_addr | PML_PRESENT | prot;
			InvalidatePage(virt_addr);
		}

		static void ExtendStack() {
			uint64_t npage = GetPage();
			if (!npage) {
				return;
			}

			Map(npage, (uint64_t)&stack[stack_size]);
			stack_size += sizeof(PML) / sizeof(uint64_t);
		}

		void AddPage(uint64_t addr) {
			if (stack_used == stack_size) {
				ExtendStack();
			}

			stack[stack_used++] = addr;
		}
	}
}