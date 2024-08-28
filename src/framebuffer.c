#include "limine.h"
#include "util.h"

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

struct limine_framebuffer* getframebuffer() {
	// Ensure we got a framebuffer.
	if (framebuffer_request.response == 0
	        || framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}

	return framebuffer_request.response->framebuffers[0];
}

void putpixel(int fgcolor, int x, int y) {
	struct limine_framebuffer* framebuffer = getframebuffer();

	volatile uint32_t *fb_ptr = framebuffer->address;
	fb_ptr[y * (framebuffer->pitch / 4) + x] = fgcolor; // e.g. 0xFFFFFF
}
