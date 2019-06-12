#include "pic.h"
#include "io.h"

#define PIC1 0x20		/* IO base address for master PIC */
#define PIC2 0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_EIO 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01

#define ICW4_8086 0x1 /* 8086/88 mode */

namespace JacOS {
	namespace PIC {
		void Init(int offset1, int offset2) {
			IO::out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
			IO::Wait();
			IO::out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
			IO::Wait();

			IO::out8(PIC1_DATA, offset1);
			IO::Wait();
			IO::out8(PIC2_DATA, offset2);
			IO::Wait();
			IO::out8(PIC1_DATA, 4); // Tell master there's a slave PIC at IRQ2
			IO::Wait();
			IO::out8(PIC2_DATA, 2); // Tell slave its cascade identity
			IO::Wait();

			IO::out8(PIC1_DATA, ICW4_8086);
			IO::Wait();
			IO::out8(PIC2_DATA, ICW4_8086);
			IO::Wait();

			IO::out8(PIC1_DATA, 0); // Don't mask anything
			IO::out8(PIC2_DATA, 0);
		}

		void SendEIO(uint8_t irq) {
			if (irq >= 8) {
				IO::out8(PIC2_COMMAND, PIC_EIO);
			}

			IO::out8(PIC1_COMMAND, PIC_EIO);
		}

		void Disable() {
			IO::out8(PIC1_DATA, 0xFF);
			IO::out8(PIC2_DATA, 0xFF);
		}
	}
}