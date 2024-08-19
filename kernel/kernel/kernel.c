#include <kernel/memory.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
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

void timer_init(void);

void kernel_main(uint32_t magic, struct multiboot_info* bootInfo) {
	/* Initialize terminal interface */
	terminal_initialize();
	initMemory(bootInfo);
	printf("Welcome to beanOS!\n");
}
