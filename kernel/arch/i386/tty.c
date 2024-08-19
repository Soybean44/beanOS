#include "vga.h"
#include <kernel/tty.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			if (y==VGA_HEIGHT-1) {
				terminal_buffer[index] = vga_entry(' ', terminal_color);
			} else {
				const size_t up_index = (y+1) * VGA_WIDTH + x;
				terminal_buffer[index] = terminal_buffer[up_index];
			}
		}
	}
}

void terminal_clear(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_putchar(char c) {
	if (c=='\n') {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row--;
			terminal_scroll();
		}
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
				terminal_row--;
				terminal_scroll();
			}
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

// TODO: Move Printf to libc

const char possibleChars[] = "0123456789abcdef";

int * printf_number(int* argp, int length, bool sign, int radix) {
	char buffer[32] = "";
	uint32_t number;
	int number_sign = 1;
	int pos = 0;

	switch(length) {
	case PRINTF_LENGTH_SHORT_SHORT:
	case PRINTF_LENGTH_SHORT:
	case PRINTF_LENGTH_START:
		if (sign) {
			int n = *argp;
			if (n < 0) {
				n = -n;
				number_sign = -1;
			}
			number = (uint32_t) n;
		} else {
			number = *(uint32_t*) argp;
		}
		argp++;
		break;
	case PRINTF_LENGTH_LONG:
		if (sign) {
			long int n = *(long int*)argp;
			if (n < 0) {
				n = -n;
				number_sign = -1;
			}
			number = (uint32_t) n;
		} else {
			number = *(uint32_t*) argp;
		}
		argp += 2;
		break;
	case PRINTF_LENGTH_LONG_LONG:
		if (sign) {
			long long int n = *(long long int*)argp;
			if (n < 0) {
				n = -n;
				number_sign = -1;
			}
			number = (uint32_t) n;
		} else {
			number = *(uint32_t*) argp;
		}
		argp += 4;
		break;
	}

	do {
		uint32_t rem = number % radix;
		number = number / radix;

		buffer[pos++] = possibleChars[rem];
	} while (number > 0);

	if (sign && number_sign < 0) {
		buffer[pos++] = '-';
	}

	while (--pos >= 0) {
		terminal_putchar(buffer[pos]);
	}

	return argp;
}

void printf(const char* fmt, ...) {
	int* argp = (int*) &fmt;
	int state = PRINTF_STATE_START;
	int length = PRINTF_LENGTH_START;
	int radix = 10;
	bool sign = false;

	argp++;
	while (*fmt) {
		switch(state) {
		case PRINTF_STATE_START:
			if (*fmt == '%') {
				state = PRINTF_STATE_LENGTH;
			} else {
				terminal_putchar(*fmt);
			}
			break;
		case PRINTF_STATE_LENGTH:
			if (*fmt == 'h') {
				length = PRINTF_LENGTH_SHORT;
				state = PRINTF_STATE_SHORT;
			} else if (*fmt == 'l') {
				length = PRINTF_LENGTH_LONG;
				state = PRINTF_STATE_LONG;
			} else {
				goto PRINTF_STATE_SPEC_;
			}
			break;
		//hd
		case PRINTF_STATE_SHORT:
			if (*fmt == 'h') {
				length = PRINTF_LENGTH_SHORT_SHORT;
				state = PRINTF_STATE_SPEC;
			} else {
				goto PRINTF_STATE_SPEC_;
			}
			break;

		case PRINTF_STATE_LONG:
			if (*fmt == 'l') {
				length = PRINTF_LENGTH_LONG_LONG;
				state = PRINTF_STATE_SPEC;
			} else {
				goto PRINTF_STATE_SPEC_;
			}
			break;

		case PRINTF_STATE_SPEC:
PRINTF_STATE_SPEC_:
			switch(*fmt) {
			case 'c':
				terminal_putchar((char)*argp);
				argp++;
				break;
			case 's':
				if (length == PRINTF_LENGTH_LONG || length == PRINTF_LENGTH_LONG_LONG) {
					terminal_putchar(*(char *)argp);
					argp += 2;
				} else {
					terminal_putchar(*(char *)argp);
					argp++;
				}
				break;
			case '%':
				terminal_putchar('%');
				break;
			case 'd':
			case 'i':
				radix = 10;
				sign = true;
				argp = printf_number(argp, length, sign, radix);
				break;
			case 'u':
				radix = 10;
				sign = false;
				argp = printf_number(argp, length, sign, radix);
				break;
			case 'X':
			case 'x':
			case 'p':
				radix = 16;
				sign = false;
				argp = printf_number(argp, length, sign, radix);
				break;
			case 'o':
				radix = 8;
				sign = false;
				argp = printf_number(argp, length, sign, radix);
				break;
			default:
				break;

			}
			state = PRINTF_STATE_START;
			length = PRINTF_LENGTH_START;
			radix = 10;
			sign = false;
			break;
		}
		fmt++;
	}
}
