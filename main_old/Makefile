include ../Makefile.inc

MISC = util.asm ia32e.asm
INT = interrupt/idt.asm interrupt/load.asm interrupt/register.asm interrupt/common.asm
ACPI = acpi/rsdp/find.asm acpi/init.asm acpi/rsdt.asm
APIC = apic/init.asm
SCREEN = screen/print.asm screen/cursor.asm
PAGING = paging/init.asm

all: main

main: main.asm $(MISC) $(ACPI) $(SCREEN) $(INT) $(APIC) $(PAGING)
	rm -f $@
	$(ASM) -f bin -o $@ $<
