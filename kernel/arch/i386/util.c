#include <stdint.h>
#include <kernel/util.h>

void outPortB(uint16_t port, uint8_t value) {
	asm volatile ("outb %1 %0" : : "dN" (port), "a" (value));
}
