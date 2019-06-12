#include "process.h"
#include "mm/memm.h"
#include "util/memory.h"
#include "vga/vga.h"
#include "util/string.h"
#include "flags.h"

#define SEG_CODE_PL0 0x8
#define SEG_DATA_PL0 0x10

namespace JacOS {
	const uint64_t STACK_PAGE = 0xFFFFFEFFFFFFF000;
	const uint64_t STACK_START = STACK_PAGE + 0x1000 - 8; // should be aligned on 8-byte boundary and not 16-byte because the entry function expects the stack misaligned
	Page::PML * const p2 = (Page::PML*)0xFFFFFFFFC0000000;
	Page::PML * const p3 = (Page::PML*)0xFFFFFFFFFFE00000;

	Process *CreateProcess(ProcessCall call) {
		Process *process = new Process;
		uint64_t p2_page = Page::GetPage();
		uint64_t pml4_page = Page::GetPage();
		uint64_t stack_page = Page::GetPage();

		if (pml4_page == 0 || stack_page == 0 || p2_page == 0) {
			return nullptr;
		}
		
		Page::Map(p2_page, (uint64_t)Page::proc_map_pml);

		memset((char*)Page::proc_map_pml, 0, sizeof(Page::PML));
		// identity map first 2 MiB for kernel
		Page::proc_map_pml->entries[0] = p2->entries[0];
		Page::proc_map_pml->entries[1] = p2->entries[1];

		Page::proc_map_pml->entries[2] = p2->entries[2]; // pcache_table


		// map the pml4_page into proc_map_pml
		Page::Map(pml4_page, (uint64_t)Page::proc_map_pml);

		// clear pml4_page
		memset((char*)Page::proc_map_pml, 0, sizeof(Page::PML));
		// map pml4 to itself
		Page::proc_map_pml->entries[511] = pml4_page | PML_PRESENT | PML_READWRITE;
		// map memory management stack
		Page::proc_map_pml->entries[510] = Page::pml4->entries[510];

		// Map stack
		Page::Map(stack_page, STACK_PAGE, 0, Page::proc_map_pml);
		// Map p2 page
		Page::Map(p2_page, (uint64_t)p2, 0, Page::proc_map_pml);

		// Clear stack
		Page::Map(stack_page, (uint64_t)Page::proc_map_pml);
		memset((char*)Page::proc_map_pml, 0, 4096);

		// setup registers
		memset((char*)&process->ctx, 0, sizeof(process->ctx));
		process->ctx.ds = SEG_DATA_PL0;
		process->ctx.rbp = STACK_START;
		process->ctx.rsp = STACK_START;
		process->ctx.rip = (uint64_t)call;
		process->ctx.ss = SEG_DATA_PL0;
		process->ctx.cs = SEG_CODE_PL0;
		process->ctx.rflags = FLAG_DEFAULT | FLAG_IF | (0 << FLAG_IOPL_OFF);

		process->cr3 = pml4_page;

		return process;
	}
}