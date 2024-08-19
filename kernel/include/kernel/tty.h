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

#endif
