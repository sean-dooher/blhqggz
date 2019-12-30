#include <test.h>
#include <bitmap.h>
#include <stdio.h>

#define BITMAP_SIZE 64

void
test_bitmap()
{
    uint64_t map[BITMAP_SIZE];

    bitmap_t *bitmap = (bitmap_t *) map;
    printf("BITMAP DECLARED\n");

    bitmap_init(bitmap, (BITMAP_SIZE - 2) * sizeof (uint64_t) * 8);
    printf("BITMAP_INIT (size: %ul)\n", bitmap_size(bitmap));

    for (size_t i = 0; i < bitmap_size(bitmap); i++) {
        assert(bitmap_get(bitmap, i) == 0, "Expected new bitmap to be all zeros");
    }

    size_t ret_val = bitmap_find_and_set(bitmap, 123);
    assert (ret_val != ~0UL, "Bitmap should be able to find 123 contigous free entries");

    size_t num_ones = 0;
    for (size_t i = 0; i < bitmap_size(bitmap); i++) {
        int val = bitmap_get(bitmap, i);
        num_ones += val;
    }

    assert(num_ones == 123, "Expected to find 123 used entries (found %lu)", num_ones);
}

void
run_test()
{
    test_bitmap();
}