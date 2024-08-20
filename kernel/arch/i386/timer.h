#ifndef ARCH_I386_TIMER_H
#define ARCH_I386_TIMER_H
#include "util.h"
void initTimer();
void onIrq0(struct InterruptRegisters* regs);
#endif
