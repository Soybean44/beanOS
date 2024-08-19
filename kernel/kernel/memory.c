#include <kernel/memory.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>

void initMemory(struct multiboot_info* bootInfo) {
	for (uint32_t i=0; i < bootInfo->mmap_length; i += sizeof(struct multiboot_mmap_entry)) {
		struct multiboot_mmap_entry *mmmt = (struct multiboot_mmap_entry*)bootInfo->mmap_addr+i;
		printf("Low addr: %x | High addr: %x | Low length %x | High length %x | size %x | type %d\n", mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high, mmmt->size, mmmt->type);
	}
}
