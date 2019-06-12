#include "interrupt.h"
#include <stdint.h>
#include "../util/memory.h"
#include "../vga/vga.h"
#include "../util/string.h"
#include "../io/pic.h"
#include "../io/io.h"

extern "C" void idt_load();

struct idt_entry {
	uint32_t data[4];
};

#define NUM_INTERRUPTS 256
typedef void (*RawHandler)(void);

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();

extern "C" void irq0();
extern "C" void irq1();

idt_entry __attribute__((aligned(4096))) idt[NUM_INTERRUPTS];

#define IDTD_PRESENT 0x8000

#define IDTD_TYPE_INT (0xE << 8)
#define IDTD_TYPE_TRAP (0xF << 8)

bool in_kernel = false;
bool in_interrupt = false;

namespace JacOS {
	namespace IDT {
		IrqHandler irq_handlers[15];
		InterruptContext *current_context;

		static void RegisterRaw(uint8_t vector, uint16_t segment_selector, RawHandler handler, uint16_t dpl, uint16_t type) {
			uint64_t offset = (uint64_t)handler;

			idt[vector].data[0] = (segment_selector << 16) | (offset & 0xFFFF);
			offset >>= 16;
			idt[vector].data[1] = ((offset & 0xFFFF) << 16) | IDTD_PRESENT | (dpl << 13) | type;
			offset >>= 16;
			idt[vector].data[2] = offset;
		}

		void Init() {
			memset((char*)idt, 0, sizeof(idt));
			memset((char*)irq_handlers, 0, sizeof(irq_handlers));

			RegisterRaw(32, 0x8, irq0, 0, IDTD_TYPE_INT);
			RegisterRaw(33, 0x8, irq1, 0, IDTD_TYPE_INT);

			idt_load();
		}

		void RegisterHandler(uint8_t irq, IrqHandler handler) {
			irq_handlers[irq] = handler;
		}

		extern "C" void interrupt_handler(JacOS::IDT::InterruptContext *ctx) {
			in_kernel = in_interrupt;
			in_interrupt = true;
			current_context = ctx;
			if (ctx->vector >= 32 && ctx->vector <= 47) { // PIC
				uint8_t irq = ctx->vector - 32;

				IrqHandler handler = irq_handlers[irq];
				if (handler != nullptr) {
					handler();
				}

				PIC::SendEIO(irq);
			}
			in_interrupt = in_kernel;
		}

		void FinalizeInterrupt() {
			in_interrupt = in_kernel;
		}

		InterruptContext *CurrentContext() {
			return current_context;
		}
	}

	bool InUserspace() {
		return  !in_kernel;
	}
}