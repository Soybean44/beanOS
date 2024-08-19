#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_clear(void);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

// TODO: Move printf to libc
void printf(const char* fmt, ...);
int * printf_number(int*, int, bool, int);

#define PRINTF_STATE_START 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_SHORT 2
#define PRINTF_STATE_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_START 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4


#endif
