#include "bitmap.h"

#define bitmap_entry_type uint64_t
#define bitmap_entry_size sizeof(bitmap_entry_type)
#define bitmap_entry_size_bits (bitmap_entry_size << 3)


#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (bitmap_entry_size_bits - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (bitmap_entry_size_bits - 1)))

struct bitmap {
    size_t size;
    bitmap_entry_type bits[0];
}; 

void
bitmap_init (bitmap_t *bitmap, size_t size)
{
    bitmap->size = size;
    bitmap_clear(bitmap, 0, size);
}

size_t
bitmap_size (bitmap_t *bitmap)
{
    return bitmap->size;
}

int
bitmap_get (bitmap_t *bitmap, size_t pos)
{
    bitmap_entry_type entry = bitmap->bits[pos / bitmap_entry_size_bits];
    uint8_t bit_select = pos % bitmap_entry_size_bits;
    return entry & (1UL << bit_select) ? 1 : 0;
}

void
bitmap_set (bitmap_t *bitmap, size_t pos, size_t n)
{
    size_t first_entry = pos / bitmap_entry_size_bits;
    size_t size = pos + n;
    bitmap_entry_type *map = bitmap->bits + first_entry;

    uint8_t num_to_set = bitmap_entry_size_bits - (pos % bitmap_entry_size_bits);
    bitmap_entry_type mark_to_set = BITMAP_FIRST_WORD_MASK(pos);

    while (n >= bitmap_entry_size_bits) {
        *(map++) |= mark_to_set;
        n -= num_to_set;
        num_to_set = bitmap_entry_size_bits;
        mark_to_set = ~0UL;
    }

    if (n > 0) {
        mark_to_set &= BITMAP_LAST_WORD_MASK(size);
        *map |= mark_to_set;
    }
}

void
bitmap_clear (bitmap_t *bitmap, size_t pos, size_t n)
{
    size_t first_entry = pos / bitmap_entry_size_bits;
    size_t size = pos + n;
    bitmap_entry_type *map = bitmap->bits + first_entry;

    uint8_t num_to_clear = bitmap_entry_size_bits - (pos % bitmap_entry_size_bits);
    bitmap_entry_type mask_to_clear = BITMAP_FIRST_WORD_MASK(pos);

    while (n >= bitmap_entry_size_bits) {
        *(map++) &= ~mask_to_clear;
        n -= num_to_clear;
        num_to_clear = bitmap_entry_size_bits;
        mask_to_clear = ~0UL;
    }

    if (n > 0) {
        mask_to_clear &= BITMAP_LAST_WORD_MASK(size);
        *map &= ~mask_to_clear;
    }
}

size_t
bitmap_find_and_set (bitmap_t *bitmap, size_t n)
{
    int64_t last_one = -1, curr_pos = 0;
    while (curr_pos <= bitmap->size) {
        if (curr_pos - last_one > n) {
            bitmap_set (bitmap, last_one + 1, n);
            return last_one + 1;
        }

        if (bitmap_get(bitmap, curr_pos) == 1) {
            last_one = curr_pos;
        }
        curr_pos++;
    }
    return ~0UL;
}
