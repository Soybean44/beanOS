#include "limine.h"
#include "util.h"
#include <stdint.h>
#include "stdlib/stdio.h"

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0
};

struct limine_memmap_response* getmemmaps() {
	// Ensure we got a framebuffer.
	if (memmap_request.response == 0) {
		hcf();
	}

	return memmap_request.response;
}

void readmemmaps() {
	struct limine_memmap_response* memmaps = getmemmaps();
	for (uint64_t i=0; i<memmaps->entry_count; i++) {
		struct limine_memmap_entry* memmap = memmaps->entries[i];
		switch (memmap->type) {
		case LIMINE_MEMMAP_USABLE: {
			printf("Type: Useable\n");
			break;
		}
		case LIMINE_MEMMAP_RESERVED: {
			printf("Type: Reserved\n");
			break;
		}
		case LIMINE_MEMMAP_ACPI_RECLAIMABLE: {
			printf("Type: ACPI Reclaimable\n");
			break;
		}
		case LIMINE_MEMMAP_ACPI_NVS: {
			printf("Type: ACPI NVS\n");
			break;
		}
		case LIMINE_MEMMAP_BAD_MEMORY: {
			printf("Type: Bad Memory\n");
			break;
		}
		case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: {
			printf("Type: Bootloader Revlaimable\n");
			break;
		}
		case LIMINE_MEMMAP_KERNEL_AND_MODULES: {
			printf("Type: Kernel and Modules\n");
			break;
		}
		case LIMINE_MEMMAP_FRAMEBUFFER: {
			printf("Type: Framebuffer\n");
			break;
		}
		default: {
			printf("Type does not exist\n");
			break;
		}
		}
		printf("Base %x, Length: %x\n", memmap->base, memmap->length);
	}
}


