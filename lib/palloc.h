#pragma once

#include <stdint.h>
#include <stddef.h>
#include "devices/machine.h"

typedef struct page {
    uint8_t data[PAGE_SIZE];
} page_t;

void palloc_init (void);
page_t *alloc_n_pages (size_t n);
void free_pages (void *addr, size_t n);

#define alloc_page() alloc_n_pages(1)
#define free_page(addr) free_pages(addr, 1)
