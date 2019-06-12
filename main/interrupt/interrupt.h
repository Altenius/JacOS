#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define IDT_ADDR 0x20000
#define IRQ_TIMER 0

namespace JacOS {
	namespace IDT {
		struct InterruptContext {
			uint64_t ds;
			uint64_t rbp;
			uint64_t rdi;
			uint64_t rsi;
			uint64_t rdx;
			uint64_t rcx;
			uint64_t rbx;
			uint64_t rax;
			uint64_t r15;
			uint64_t r14;
			uint64_t r13;
			uint64_t r12;
			uint64_t r11;
			uint64_t r10;
			uint64_t r9;
			uint64_t r8;

			uint64_t vector;

			uint64_t error;
			uint64_t rip;
			uint64_t cs;
			uint64_t rflags;
			uint64_t rsp;
			uint64_t ss;
		};

		typedef void (*IrqHandler)();

		void Init();

		inline void Enable() {
			__asm__ volatile ("sti");
		};
		inline void Disable() {
			__asm__ volatile ("cli");
		};

		void RegisterHandler(uint8_t irq, IrqHandler handler);

		InterruptContext *CurrentContext();

		void FinalizeInterrupt();
	}

	bool InUserspace();
}



#endif
