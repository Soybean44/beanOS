#include <stdint.h>
#include "util.h"

void outPortB(uint16_t Port, uint8_t Value) {
	asm volatile ("outb %1, %0" : : "dN" (Port), "a" (Value));
}
