#pragma once

#include <stdint.h>
#include <stddef.h>
#include "devices/machine.h"

#define PALLOC_CLEAR 0x1
#define PALLOC_NO_FLAGS 0x0

typedef struct page {
    uint8_t data[PAGE_SIZE];
} page_t;

void palloc_init (void);
page_t *alloc_n_pages (size_t n, uint8_t flags);
void free_pages (void *addr, size_t n);


#define alloc_page(flags) alloc_n_pages(1, flags)

#define free_page(addr) free_pages(addr, 1)
