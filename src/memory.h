#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>
#include "limine.h"
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strlen(char* s);
struct limine_memmap_response* getMemmaps();
struct limine_hhdm_response* getHDDM();
void readMemmaps();
struct limine_memmap_entry* getFirstUseableMemmap();
#endif

