#include "timer.h"
#include "io/pit.h"
#include "vga/vga.h"
#include "interrupt/interrupt.h"
#include "scheduler.h"

namespace JacOS {
	namespace Timer {
		static void OnTick() {
			Scheduler::Tick();
		}

		void Init() {
			PIT::SetTimerFrequency(100); // Hz
			IDT::RegisterHandler(0, OnTick);
		}
	}
}