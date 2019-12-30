#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct bitmap bitmap_t; 

void bitmap_init (bitmap_t *bitmap, size_t size);

void bitmap_set (bitmap_t *bitmap, size_t pos, size_t n);

void bitmap_clear (bitmap_t *bitmap, size_t pos, size_t n);

size_t bitmap_size (bitmap_t *bitmap);

int bitmap_get (bitmap_t *bitmap, size_t pos);

size_t bitmap_find_and_set (bitmap_t *bitmap, size_t n);
