#ifndef CPU_H
#define CPU_H
#include <stdint.h>

extern "C" uint8_t cpu_maxphyaddr(void);
extern "C" int cpuid_supported(void);

inline void read_msr(uint32_t msr, uint32_t &lower, uint32_t &upper) {
	__asm__ volatile ("rdmsr" : "=a"(lower), "=d"(upper) : "c"(msr));
}

#endif
