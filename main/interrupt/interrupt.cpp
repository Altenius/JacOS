#include "interrupt.h"
#include <stdint.h>

extern "C" void idt_load();

void init_idt(void) {
    idt_load();
}
