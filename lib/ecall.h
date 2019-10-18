#pragma once

#include <stdint.h>

typedef enum {
    TIME_INIT=0,
    TIME_CLEAR=1,
    TIME_SET=2,
    TIME_READ=3,
    S_TIMER_CLEAR=4
} ECALL_NUM;

uint64_t ecall (ECALL_NUM);
uint64_t ecall1 (ECALL_NUM, uint64_t);
uint64_t ecall2 (ECALL_NUM, uint64_t, uint64_t);