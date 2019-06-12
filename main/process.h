#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "interrupt/interrupt.h"

namespace JacOS {
	typedef void (*ProcessCall)();

	struct Process {
		IDT::InterruptContext ctx;
		uint64_t cr3;

		Process *next;
	};

	Process *CreateProcess(ProcessCall call);
}

#endif