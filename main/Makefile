TARGET = x86_64-elf
CC = $(TARGET)-gcc
CXX = $(TARGET)-g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fbuiltin -nostdlib \
			-mno-red-zone -mno-mmx -mno-sse -mno-sse2
AS = $(TARGET)-as

CRTBEGIN_OBJ=$(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ=$(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)

MAINOBJS = load.o arch/cpu.o interrupt/idt.o kernel.o vga/vga.o util/memory.o util/string.o interrupt/interrupt.o mm/memm.o
SHITOBJS = crti.o crtn.o $(MAINOBJS) end.o

OBJS = crti.o $(CRTBEGIN_OBJ) $(MAINOBJS) crtn.o $(CRTEND_OBJ) end.o # end.o MUST be last

default: main.bin

%.o: %.S
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.bin: $(SHITOBJS) linker.ld
	$(CXX) $(CXXFLAGS) -Wl,-Ttext,100000 -Wl,-z,max-page-size=0x1000 $(OBJS) -o $@

clean:
	rm -f $(SHITOBJS)
	rm -f main.bin