#ifndef ARCH_I386_UTIL_H
#define ARCH_I386_UTIL_H
#include <stdint.h>

void outPortB(uint16_t port, uint8_t value);

#define CEIL_DIV(a,b) (((a+b)-1)/b)

#endif

