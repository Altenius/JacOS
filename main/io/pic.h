#ifndef PIC_H
#define PIC_H

#include <stdint.h>

namespace JacOS {
	namespace PIC {
		void Init(int offset1, int offset2);
		void SendEIO(uint8_t irq);
		void Disable();
	}
}

#endif