#include <stdarg.h>
#include "../tty.h"

#define NUM_SIZE 256
const char hex_digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8','9', 'A', 'B', 'C', 'D', 'E', 'F'};
char buf[NUM_SIZE];

int printf(char* format, ...) {
	va_list args;
	va_start(args, format);
	unsigned int size = 0;
	char glyph = *(format+size);
	while(glyph) {
		while (glyph && glyph != '%') {
			size++;
			glyph = *(format+size);
		}
		if (size) {
			write(format, size);
			format+=size;
			size=0;
		}
		if (glyph == '%') {
			format++;
			glyph = *format;
			switch (glyph) {
			case 'c': {
				int c = va_arg(args, int);
				putchar((char)c);
				glyph = *(++format);
				continue;
			}
			case 's': {
				char* s = va_arg(args, char*);
				write_string((char*)s);
				glyph = *(++format);
				continue;
			}
			case 'd': {
				int size = 0;
				int num = va_arg(args, int);
				int dig = num % 10;
				// TODO: add negative numbers
				while (num > 0) {
					size++;
					buf[NUM_SIZE-size]=dig+'0';
					num /= 10;
					dig = num % 10;
				}
				write(&buf[NUM_SIZE-size], size);
				glyph = *(++format);
				continue;
			}
			case 'x': { // Hexadecimal
				int size = 0;
				int num = va_arg(args, int);
				int dig = num % 16;
				while (num > 0) {
					size++;
					buf[NUM_SIZE-size]=hex_digits[dig];
					num /= 16;
					dig = num % 16;
				}
				write(&buf[NUM_SIZE-size], size);
				glyph = *(++format);
				continue;
			}
			default: {
				write_string("This formatting has not been implemented.\n");
				glyph = *(++format);
				break;
			}
			}
		}
	}
	va_end(args);
	return 0;
}
