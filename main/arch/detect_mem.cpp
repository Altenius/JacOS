#include "detect_mem.h"
#include "../vga/vga.h"
#include "../util/string.h"
#include "../util/memory.h"

#define MEM_MAP_START 0x6000
#define MEM_MAP_SIZE 0x5998

namespace JacOS {
    uint64_t total_memory, total_usable_memory;

    uint16_t mem_entries_len;
    memory_range_t mem_entries[MEM_MAX_ENTRIES];

    void detect_memory(void) {
        uint16_t size = *((uint16_t*)MEM_MAP_SIZE);
        Vga::PrintString("Memory map size: ");
        Vga::PrintString(uitos(size));
        Vga::PrintString("\n");

        memory_range_t *memory_range = (memory_range_t*)MEM_MAP_START;

        // sort the memory range from lowest address to highest
        memory_range_t temp;
        for (int i = 1; i < size; i++) {
            for (int k = i; k != 0; k--) {
                if (memory_range[k].addr < memory_range[k - 1].addr) {
                        memcpy((char*)&temp, (char*)&memory_range[k], sizeof(memory_range_t));
                        memcpy((char*)&memory_range[k], (char*)&memory_range[k - 1], sizeof(memory_range_t));
                        memcpy((char*)&memory_range[k - 1], (char*)&temp, sizeof(memory_range_t));
                } else {
                    break;
                }
            }
        }

        total_memory = 0;
        total_usable_memory = 0;

        // Fix overlapping entries and combine contiguous entries of the same type
        for (int i = 1; i < size; i++) {
            if (memory_range[i - 1].addr + memory_range[i - 1].length > memory_range[i].addr) {
                if (memory_range[i - 1].type != 1) { // The entry before this is reserved, so move our address up
                    uint64_t diff = memory_range[i - 1].addr + memory_range[i - 1].length - memory_range[i].addr; // difference between the last address of the last entry + 1 and the address of the current entry
                    if (diff >= memory_range[i].length) {
                        // erase this memory range
                        if (i != size - 1) {
                            memcpy((char*)&memory_range[i], (char*)&memory_range[i + 1], (size - i - 1) * sizeof(memory_range_t));
                        }
                        size--;
                        i--;
                        continue;
                    }
                    memory_range[i].addr += diff;
                } else {
                    memory_range[i - 1].length = memory_range[i].addr - memory_range[i - 1].addr;
                }
            }
            if (memory_range[i - 1].type == memory_range[i].type &&
                memory_range[i - 1].addr + memory_range[i - 1].length == memory_range[i].addr) { // Combine the last range with the current one (same type & contiguous)
                memory_range[i - 1].length += memory_range[i].length;

                if (i != size - 1) {
                    memcpy((char*)&memory_range[i], (char*)&memory_range[i + 1], (size - i - 1) * sizeof(memory_range_t));
                }
                size--;
                i--;
                continue;
            }
            total_memory += memory_range[i - 1].length;
            if (memory_range[i - 1].type == 1) {
                total_usable_memory += memory_range[i - 1].length;
            }
        }

        mem_entries_len = 0;

        // We only care about the usable entries
        for (int i = 0; i < size; i++) {
            if (memory_range[i].type == 1) {
                if (mem_entries_len == MEM_MAX_ENTRIES) {
                    Vga::PrintString("Too many memory entires; memory segments will be discarded\n");
                    break;
                }
                memcpy((char*)&mem_entries[mem_entries_len], (char*)&memory_range[i], sizeof(memory_range_t));
                mem_entries_len++;
            }
        }

        for (int i = 0; i < mem_entries_len; i++) {
            Vga::PrintString("Memory range ");
            Vga::PrintString(uitos(i));
            Vga::PrintString("\nAddress: 0x");
            Vga::PrintString(uqtohs(mem_entries[i].addr));
            Vga::PrintString("\nLength: 0x");
            Vga::PrintString(uqtohs(mem_entries[i].length));
            Vga::PrintString("\nType: ");
            Vga::PrintString(uitos(mem_entries[i].type));
            Vga::PrintString(mem_entries[i].type == 1 ? " (usable)" : " (reserved)");
            Vga::PrintString("\n");
        }

        Vga::PrintString("Total memory: ");
        Vga::PrintString(uqtos(total_memory));
        Vga::PrintString(" bytes\nTotal usable memory: ");
        Vga::PrintString(uqtos(total_usable_memory));
        Vga::PrintString(" bytes\n");
    }

    memory_range_t *memory_layout(uint16_t *entries) {
        *entries = mem_entries_len;
        return mem_entries;
    }

}