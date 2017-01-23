#include <stddef.h>
#include <stdint.h>

#include "vga/vga.h"
#include "util/string.h"
#include "arch/cpu.h"
#include "arch/detect_mem.h"
#include "paging/paging.h"
#include "multiboot.h"
#include "mm/memm.h"

namespace ShitOS {
    extern size_t end;
    uint16_t *terminal_buffer;
    uint8_t maxphyaddr;


    void init_cpu(void) {
        if (cpuid_supported()) {
            Vga::PrintString("> CPUID supported\n");
            maxphyaddr = cpu_maxphyaddr();
            Vga::PrintString("> MAXPHYADDR: ");
            Vga::PrintString(uitos(maxphyaddr));
            Vga::PrintString("\n");
        } else {
            Vga::PrintString("- CPUID unsupported\n");
        }
    }


    extern "C" void kernel(uint64_t magic, multiboot_info_t *bootinfo) {
        Vga::SetColor(VGA_COLOR(COLOR_WHITE, COLOR_DARK_GRAY));
        Vga::Clear();

        Vga::PrintString("Booted into kernel\n");
        Vga::SetColor(VGA_COLOR(COLOR_BROWN, COLOR_DARK_GRAY));
        Vga::PrintString("ShitOS V1.0\n");
        Vga::SetColor(VGA_COLOR(COLOR_WHITE, COLOR_DARK_GRAY));
        Vga::PrintString(uqtohs(reinterpret_cast<size_t>(&end)));
        Vga::PrintString("\n");

        //Vga::PrintString("\nAvailable upper memory: ");
        //Vga::PrintString(uqtos((uint64_t)bootinfo->mem_upper));

        Memory::Init(bootinfo);

        //init_cpu();
        //detect_memory();
        //init_paging();

        //alloc_pages(64);

        //update_cursor();
    }
}