#include <stdarg.h>
#include "../tty.h"

#define NUM_SIZE 32

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
			case 'd': {
				char buf[NUM_SIZE];
				int size = 0;
				int num = va_arg(args, int);
				int dig = num % 10;
				while (num > 0) {
					size++;
					buf[NUM_SIZE-size]=dig+'0';
					num /= 10;
					dig = num % 10;
				}
				write(&buf[NUM_SIZE-size], size);
				break;
			}
			default: {
				write_string("This formatting has not been implemented.\n");
				break;
			}
			}
			format++;
		}
	}
	va_end(args);
	return 0;
}
