#pragma once

#include <stdint.h>

void     time_init  (void);
void     time_clear (void);
uint64_t time_read  (void);

void     timer_interrupt (void);