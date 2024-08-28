#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "limine.h"
struct limine_framebuffer* getframebuffer();
void putpixel(int fgcolor, int x, int y);
#endif
