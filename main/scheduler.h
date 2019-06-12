#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

namespace JacOS {
	namespace Scheduler {
		void Init(Process *init_proc);
		void Tick();

		void Add(Process *process);
	}
}

#endif