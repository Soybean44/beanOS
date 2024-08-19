#ifndef ARCH_I386_UTIL_H
#define ARCH_I386_UTIL_H
#include <stdint.h>
void outPortB(uint16_t port, uint8_t value);

#define CEIL_DIV(a,b) (((a+b)-1)/b)

struct InterruptRegisters {
	uint32_t cr2;
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, csm, eflags, useresp, ss;
};
#endif


