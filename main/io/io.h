#ifndef IO_H
#define IO_H

#include <stdint.h>

namespace JacOS {
	namespace IO {
		inline uint8_t in8(uint16_t port) {
			uint8_t input;
			__asm__ volatile ("inb %1, %0" : "=a"(input) : "Nd"(port));
			return input;
		}

		inline void out8(uint16_t port, uint8_t out) {
			__asm__ volatile ("outb %0, %1" : : "a"(out), "Nd"(port));
		}

		inline void Wait() {
			__asm__ volatile ("outb %%al, $0x80" : : "a"(0));
		}
	}
}

#endif