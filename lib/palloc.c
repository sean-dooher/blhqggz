#include <bitmap.h>
#include <string.h>

#include "palloc.h"
#include "vm/paging.h"

static bitmap_t *mem_bitmap;

static page_t *FREE_PAGE_BASE;

uint64_t N_PALLOC_BITMAP_PAGES;

void
palloc_init (void)
{
    FREE_PAGE_BASE = (page_t *) FREE_MEM_BASE;
    
    mem_bitmap = (bitmap_t *) FREE_MEM_BASE;
    bitmap_init(mem_bitmap, N_FREE_PAGES);

    N_PALLOC_BITMAP_PAGES = N_PAGES(sizeof(mem_bitmap) + (N_FREE_PAGES >> 3) + 1);
    bitmap_set(mem_bitmap, 0, N_PALLOC_BITMAP_PAGES);

    uint8_t n_stack_pages = STACK_SIZE / PAGE_SIZE;
    bitmap_set(mem_bitmap, bitmap_size(mem_bitmap) - n_stack_pages, n_stack_pages);
}

page_t *
alloc_n_pages (size_t n, uint8_t flags)
{
    size_t offset = bitmap_find_and_set (mem_bitmap, n);
    if (offset != ~0UL) {
        page_t *pages = &FREE_PAGE_BASE[offset];
        if (flags & PALLOC_CLEAR) {
            for (int i = 0; i < n; i++) {
                memset(pages[i].data, 0, PAGE_SIZE);
            }
        }
        return pages;
    }

    return NULL;
}

void
free_pages (void *addr, size_t n)
{
    size_t page_n = N_PAGES (addr - FREE_MEM_BASE) - 1;
    bitmap_clear(mem_bitmap, page_n, n);
}