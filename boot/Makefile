include ../Makefile.inc

INIT_ASM = init/print.asm init/gdt.asm init/a20.asm load_main.asm
PM_ASM = pm/pm.asm pm/print_vga.asm

all: boot

boot: init/boot.asm $(INIT_ASM) $(PM_ASM)
	rm -f $@
	$(ASM) -f bin -o $@ $<