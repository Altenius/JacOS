#include "pit.h"
#include "io.h"

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42

#define PIT_SQUARE (3 << 1)
#define PIT_RW_LSB (1 << 4)
#define PIT_RW_MSB (2 << 4)
#define PIT_RW_LSB_MSB (PIT_RW_LSB | PIT_RW_MSB)
#define PIT_CTR_0 0
#define PIT_CTR_1 (1 << 6)
#define PIT_CTR_2 (2 << 6)

namespace JacOS {
	namespace PIT {
		const uint32_t pit_clock = 1193180;

		static uint16_t RealFrequency(uint16_t frequency) {
			return (pit_clock / (pit_clock / frequency));
		}

		void SetTimerFrequency(uint16_t frequency) {
			uint16_t divisor = pit_clock / RealFrequency(frequency);
			IO::out8(PIT_COMMAND, PIT_SQUARE | PIT_RW_LSB_MSB | PIT_CTR_0);
			IO::out8(PIT_CHANNEL0, divisor && 0xFF);
			IO::out8(PIT_CHANNEL0, divisor >> 8);
		}
	}
}