#include "vga.h"
#include "../util/memory.h"
#include <stdint.h>



namespace JacOS {
    namespace Vga {
        uint16_t *vga_base = (uint16_t*)0xB8000;
        uint8_t col = 0;
        uint8_t row = 0;
        uint8_t color = 0x0F;



        void SetColor(uint8_t col) {
            color = col;
        }

        void CheckScroll() {
            if (row > 24) {
                uint8_t amount = row - 24;
                for (int i = 1; i != 25; i++) {
                    memcpy((char*)vga_base + ((i - amount) * 80 * 2), (char*)vga_base + (i * 80 * 2), 160);
                }
                row = 24;
                ClearLine();
            }
        }

        void ClearLine() {
            if (row < 25) {
                for (int i = 0; i < 80; i++) {
                    vga_base[i + row * 80] = (color << 8);
                }
                // memset((char*)vga_base + row * 80 * 2, 0, 160);
            }
        }

        void Clear() {
            for (int i = 0; i < 80 * 25; i++) {
                vga_base[i] = (color << 8);
            }
        }

        void PrintString(const char *string) {
            while (*string != '\0') {
                if (*string != '\n') {
                    vga_base[col + row * 80] = (color << 8) | *string;
                    col++;
                } else {
                    col = 0;
                    row++;
                    CheckScroll();
                }
                string++;
            }
        }

        void PrintChar(char c) {
            if (c == '\n') {
                col = 0;
                row++;
                CheckScroll();
                return;
            }

            vga_base[col + row * 80] = (color << 8) | c;
            col++;
        }

        void UpdateCursor() {
            uint16_t cursor_pos = row * 80 + col;
            __asm__("movw %0, %%bx\n\t"
                    "movw $0x3D4, %%dx\n\t"
                    "movb $0x0F, %%al\n\t"
                    "out %%al, %%dx\n\t"
                    "movw $0x3D5, %%dx\n\t"
                    "movb %%bl, %%al\n\t"
                    "out %%al, %%dx\n\t"
                    "movw $0x3D4, %%dx\n\t"
                    "movb $0x0E, %%al\n\t"
                    "out %%al, %%dx\n\t"
                    "movw $0x3D5, %%dx\n\t"
                    "movb %%bh, %%al\n\t"
                    "out %%al, %%dx\n\t"
                    :
                    : "r" (cursor_pos));
        }
    }
}