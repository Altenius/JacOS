#include "keyboard.h"
#include "../util/memory.h"
#include "../vga/vga.h"
#include "../io/io.h"
#include "../interrupt/interrupt.h"

#define KEY_LSHIFT 0x2A
	    #define KEY_RSHIFT 0x36

namespace JacOS {
	namespace Keyboard {
	    uint8_t keymap[256];

	    const char asciimap[] = {
	        0,
	        0, // esc
	        '1',
	        '2',
	        '3',
	        '4',
	        '5',
	        '6',
	        '7',
	        '8',
	        '9',
	        '0',
	        '-',
	        '=',
	        8, // backspace
	        '\t', // tab
	        'Q',
	        'W',
	        'E',
	        'R',
	        'T',
	        'Y',
	        'U',
	        'I',
	        'O',
	        'P',
	        '[',
	        ']',
	        '\n', // enter
	        0, // left ctrl
	        'A',
	        'S',
	        'D',
	        'F',
	        'G',
	        'H',
	        'J',
	        'K',
	        'L',
	        ';',
	        '\'',
	        '`',
	        0, // left shift
	        '\\',
	        'Z',
	        'X',
	        'C',
	        'V',
	        'B',
	        'N',
	        'M',
	        ',',
	        '.',
	        '/',
	        0, // right shift
	        '*', // keypad *
	        0, // left alt
	        ' ', // space
	        0, // caps lock
	        0, // F1
	        0, // F2
	        0, // F3
	        0, // F4
	        0, // F5
	        0, // F6
	        0, // F7
	        0, // F8
	        0, // F9
	        0, // F10
	        0, // numlock
	        0, // scroll lock
	        '7', // keypad 7
	        '8', // keypad 8
	        '9', // keypad 9
	        '-', // keypad -
	        '4', // keypad 4
	        '5', // keypad 5
	        '6', // keypad 6
	        '+', // keypad +
	        '1', // keypad 1
	        '2', // keypad 2
	        '3', // keypad 3
	        '0', // keypad 0
	        '.', // keypad .
	        0,
	        0,
	        0,
	        0, // F11
	    };

	    static char ToAscii(uint8_t key) {
	        if (key > sizeof(asciimap)) {
	            return 0;
	        }

	        char k = asciimap[key];
	        if (k >= 'A' && k <= 'Z') {
	            if (!(keymap[KEY_LSHIFT] || keymap[KEY_RSHIFT])) {
	                k += 'a' - 'A';
	            }
	        }
	        return k;
	    }

	    static void KeyboardHandler() {
	        uint8_t code = IO::in8(0x60);
	        uint8_t key = code & 0x7F;

	        if (code & 0x80) { // Released
	            keymap[key] = 0;
	        } else {
	            keymap[key] = 1;

	            char c = ToAscii(key);
	            if (c != 0) {
	                Vga::PrintChar(c);
	            }
	        }
	    }

		void Init() {
			memset((char*)keymap, 0, sizeof(keymap));
			IDT::RegisterHandler(1, KeyboardHandler); // Keyboard IRQ is 1
		}
	}
}