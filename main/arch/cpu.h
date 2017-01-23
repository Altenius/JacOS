#ifndef CPU_H
#define CPU_H
#include <stdint.h>

extern "C" uint8_t cpu_maxphyaddr(void);
extern "C" int cpuid_supported(void);

#endif
