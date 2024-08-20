#include <stdint.h>
#include <stdio.h>
#include "util.h"
#include "interrupts/idt.h"
#include "timer.h"

uint64_t ticks;
const uint32_t freq = 1000;

void onIrq0(struct InterruptRegisters* regs) {
	(void)regs;
	ticks += 1;

	printf("Timer ticked!");
}

void initTimer() {
	ticks = 0;
	irq_install_handler(0,&onIrq0);

	//1.1931816666 MHz
	uint32_t divisor = 1193180/freq;

	outPortB(0x43, 0x36); // Set mode of timer to mode 3 (square wave) and output to irq0
	outPortB(0x40, (uint8_t)(divisor & 0xFF));
	outPortB(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
