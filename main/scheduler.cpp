#include "scheduler.h"
#include "process.h"
#include "interrupt/interrupt.h"
#include "util/memory.h"
#include "io/pic.h"
#include "util/string.h"

#include "vga/vga.h"

extern "C" void resume_process(JacOS::IDT::InterruptContext *ctx, uint64_t cr3);

namespace JacOS {
	namespace Scheduler {
		static Process *current;
		static Process *first;
		static Process *last;

		void Init(Process *init_proc) {
			current = first = last = init_proc;
			current->next = current;
			resume_process(&current->ctx, current->cr3);
		}


		void Switch() {
			// Must only be called in a timer interrupt
			memcpy((char*)&current->ctx, (char*)IDT::CurrentContext(), sizeof(IDT::InterruptContext));

			current = current->next;

			PIC::SendEIO(IRQ_TIMER);
			IDT::FinalizeInterrupt();
			resume_process(&current->ctx, current->cr3);
		}

		void Add(Process *process) {
			process->next = first;
			last->next = process;
			last = process;
		}

		void Tick() {
			if (current != nullptr && current->next != current) {
				if (InUserspace()) {
					Switch();
				}
			}
		}
	}
}