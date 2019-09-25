#pragma once
#include <stdint.h>

void clint_init (void);

uint64_t clint_read_mtime (void);

void clint_schedule_ms (uint64_t);

void clint_schedule (uint64_t);