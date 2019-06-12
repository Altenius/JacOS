#ifndef VGA_H
#define VGA_H
#include <stdint.h>

#define VGA_COLOR(fg, bg) (uint8_t)((bg << 4) | fg)

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 12
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

void set_color(uint8_t col);
void check_scroll(void);
void clear_line(void);
void vga_clear(void);
void print_string(const char *string);
void print_uint32(uint32_t n);
void print_uint64(uint64_t n);
void update_cursor(void);

namespace JacOS {
	namespace Vga {
		void SetColor(uint8_t color);
		void CheckScroll();
		void ClearLine();
		void Clear();
		void PrintString(const char *string);
		void PrintChar(char c);
		void UpdateCursor();
	}
}

#endif
