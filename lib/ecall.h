#pragma once

#include <stdint.h>

typedef enum {
    TIME_INIT=0,
    TIME_CLEAR=1,
    TIME_SET=2
} ECALL_NUM;

uint64_t ecall (ECALL_NUM num);
uint64_t ecall_int (ECALL_NUM num, uint64_t val);