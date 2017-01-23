#ifndef INTERRUPT_H
#define INTERRUPT_H

#define IDT_ADDR 0x20000

extern "C" void init_idt(void);

#endif
