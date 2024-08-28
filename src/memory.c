#include <stdint.h>
#include <stddef.h>
#include "limine.h"
#include "util.h"
#include "stdlib/stdio.h"

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	for (size_t i = 0; i < n; i++) {
		pdest[i] = psrc[i];
	}

	return dest;
}

void *memset(void *s, int c, size_t n) {
	uint8_t *p = (uint8_t *)s;

	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)c;
	}

	return s;
}

void *memmove(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	if (src > dest) {
		for (size_t i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (size_t i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *p1 = (const uint8_t *)s1;
	const uint8_t *p2 = (const uint8_t *)s2;

	for (size_t i = 0; i < n; i++) {
		if (p1[i] != p2[i]) {
			return p1[i] < p2[i] ? -1 : 1;
		}
	}

	return 0;
}

int strlen(char* s) {
	int size = 0;
	while (*s) {
		size++;
		s++;
	}
	size++;
	return size;
}

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0
};

__attribute__((used, section(".requests")))
struct limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0
};

struct limine_memmap_response* getMemmaps() {
	// Ensure we got a framebuffer.
	if (memmap_request.response == 0) {
		hcf();
	}

	return memmap_request.response;
}

struct limine_hhdm_response* getHDDM() {
	// Ensure we got a framebuffer.
	if (hhdm_request.response == 0) {
		hcf();
	}

	return hhdm_request.response;
}

void readMemmaps() {
	struct limine_memmap_response* memmaps = getMemmaps();
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

struct limine_memmap_entry* getFirstUseableMemmap() {
	struct limine_memmap_response* memmaps = getMemmaps();
	for (uint64_t i=0; i<memmaps->entry_count; i++) {
		struct limine_memmap_entry* memmap = memmaps->entries[i];
		if (memmap->type == LIMINE_MEMMAP_USABLE) {
			return memmap;
		}
	}
	return 0;
}
