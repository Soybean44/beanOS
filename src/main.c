#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "util.h"
#include "memory.h"
#include "stdlib/stdio.h"

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;



// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		hcf();
	}


	// Note: we assume the framebuffer model is RGB with 32-bit pixels.
	//for (size_t i = 0; i < 100; i++) {
	//	putpixel(0xffffff,i,i);
	//}

	char* msg = "Hello World!\n";
	printf(msg);
	printf("My name is bean!\n");
	printf("I am %d %s%c\n\n", 18, "years ol", 'd');

	struct limine_memmap_entry* memmap = getFirstUseableMemmap();
	if (memmap == 0) {
		printf("No useable memmap entry.\n");
		hcf();
	}

	// The higher half direct map gives us the offset of the virtual addresses, allowing us to
	// convert the virtual address given by the memmap to a physical address we can use in the
	// code to actually assign data in the memory address, later on we can use this to create a crude allocator for our opperating system
	struct limine_hhdm_response* hhdm = getHDDM();

	char* mem_ptr = (char*)(memmap->base + hhdm->offset);

	char* memory_msg = "Hello from memory!\n";
	memcpy(mem_ptr, memory_msg, strlen(memory_msg));
	printf(mem_ptr);

	// We're done, just hang...
	hcf();
}
