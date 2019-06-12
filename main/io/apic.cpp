#include "apic.h"
#include "../mm/memm.h"
#include "../arch/cpu.h"
#include "../vga/vga.h"
#include "../util/string.h"

#define IA32_APIC_BASE_MSR 0x1B

#define APIC_BASE 0xFFE0000
#define APIC_MAPPED 0x0000000000406000UL

#define SVR_APIC_ENABLE (1 << 8)

namespace JacOS {
	namespace APIC {
		uint32_t *SVR = (uint32_t*)0x00000000004060F0UL;

		void Init() {
			Page::Map(APIC_BASE, APIC_MAPPED, PML_READWRITE | PML_CACHEDISABLE);

			*SVR = *SVR | SVR_APIC_ENABLE;

			Vga::PrintString("Initializing apic\n");

			uint32_t lower, upper;
			read_msr(IA32_APIC_BASE_MSR, lower, upper);
			Vga::PrintString("Lower: ");
			Vga::PrintString(uitohs(lower));
			Vga::PrintString(" Upper: ");
			Vga::PrintString(uitohs(upper));
			Vga::PrintString("\n");

			uint32_t apic_base = lower & 0xFFFFF000;
		}
	}
}
