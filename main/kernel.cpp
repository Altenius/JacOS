#include <stddef.h>
#include <stdint.h>

#include "vga/vga.h"
#include "util/string.h"
#include "arch/cpu.h"
#include "arch/detect_mem.h"
#include "paging/paging.h"
#include "multiboot.h"
#include "mm/memm.h"
#include "mm/alloc.h"
#include "interrupt/interrupt.h"
#include "io/pic.h"
#include "io/io.h"
#include "util/memory.h"
#include "device/keyboard.h"
#include "io/apic.h"
#include "timer.h"
#include "scheduler.h"
#include "process.h"

namespace JacOS {
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

    void TestCall() {
        ta:
        Vga::PrintString("TEST CALL\n");
        goto ta;
    }

    void TestCall2() {
        tb:
        Vga::PrintString("TEST CALL2\n");
        goto tb;
    }

    void KernelThread() {
        Vga::PrintString("Runni\n");

        Process *proc = CreateProcess(TestCall);
        Process *proc2 = CreateProcess(TestCall2);

        Scheduler::Add(proc);
        Scheduler::Add(proc2);

        shat:
        Vga::PrintString("KERNEL THREAD\n");
        goto shat;

        __asm__ volatile("a: hlt\n"
                         "jmp a");
    }


    extern "C" void kernel(uint64_t magic, multiboot_info_t *bootinfo) {
        Vga::SetColor(VGA_COLOR(COLOR_WHITE, COLOR_DARK_GRAY));
        Vga::Clear();

        Vga::PrintString("Booted into kernel\n");
        Vga::SetColor(VGA_COLOR(COLOR_BROWN, COLOR_DARK_GRAY));
        Vga::PrintString("JacOS V1.0\n");
        Vga::SetColor(VGA_COLOR(COLOR_WHITE, COLOR_DARK_GRAY));
        Vga::PrintString(uqtohs(reinterpret_cast<size_t>(&end)));
        Vga::PrintString("\n");

        Memory::Init(bootinfo);
        Heap::Init();
        
        //PIC::Init(32, 40);

        PIC::Disable();
        APIC::Init();

        IDT::Init();
        Keyboard::Init();
        IDT::Enable();

        Timer::Init();

        Scheduler::Init(CreateProcess(KernelThread)); // Runs KernelThread
    }
}
