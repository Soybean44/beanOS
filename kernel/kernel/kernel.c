#include <kernel/memory.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <stdint.h>
#include <stdio.h>


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
void kernel_main(uint32_t magic, struct multiboot_info* bootInfo) {
	/* Initialize terminal interface */
	initTerminal();
	printf("Welcome to beanOS\n");
	initGdt();
	initIdt();
	initTimer();
	initKeyboard();

	uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
	uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;

	initMemory(bootInfo->mem_upper * 1024, physicalAllocStart);
}

