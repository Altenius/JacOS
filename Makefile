include Makefile.inc

all: shitos.iso

boot/boot: force_look
	cd boot && make

main/main.bin: force_look
	cd main && make main.bin

shitos.iso: main/main.bin
	cp main/main.bin iso/boot/main.bin
	grub-mkrescue -o shitos.iso iso

trash: boot/boot main/main
	rm -f $@
	cat boot/boot main/main > $@

run: shitos.iso
	qemu-system-x86_64 $<

run-kvm: shitos.iso
	qemu-system-x86_64 -enable-kvm $<

force_look:
	true

clean:
	rm -f shitos.iso
	cd main && make clean
