#include <bitmap.h>
#include "palloc.h"
#include "vm/paging.h"

static bitmap_t *mem_bitmap;

static page_t *FREE_PAGE_BASE;

void
palloc_init (void)
{
    FREE_PAGE_BASE = (page_t *) FREE_MEM_BASE;
    
    mem_bitmap = (bitmap_t *) FREE_MEM_BASE;
    bitmap_init(mem_bitmap, N_FREE_PAGES);

    bitmap_set(mem_bitmap, 0, N_PAGES(sizeof(mem_bitmap) + (N_FREE_PAGES >> 3) + 1));
}

page_t *
alloc_n_pages (size_t n)
{
    size_t offset = bitmap_find_and_set (mem_bitmap, n);
    if (offset != ~0UL) {
        return &FREE_PAGE_BASE[offset];
    }

    return NULL;
}

void
free_pages (void *addr, size_t n)
{
    size_t page_n = N_PAGES (addr - FREE_MEM_BASE) - 1;
    bitmap_clear(mem_bitmap, page_n, n);
}