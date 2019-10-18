#pragma once

#include <stdint.h>

typedef enum {
    SET=0,
    SET_MS=1,
    DELAY=2,
    DELAY_MS=3
} timer_mode_t;

void     time_init  (void);
void     time_clear (void);
uint64_t time_read  (void);
void     time_set   (uint64_t, timer_mode_t);

void     timer_interrupt (void);