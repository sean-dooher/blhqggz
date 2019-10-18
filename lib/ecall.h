#pragma once

#include <stdint.h>
#include "sbi.h"

typedef enum {
    TIME_INIT=SBI_TIME_INIT,
    TIME_CLEAR=SBI_TIME_CLEAR,
    TIME_SET=SBI_TIME_SET,
    TIME_READ=SBI_TIME_READ,
    CLEAR_INTR=SBI_CLEAR_INTR
} ECALL_NUM;

uint64_t ecall (ECALL_NUM);
uint64_t ecall1 (ECALL_NUM, uint64_t);
uint64_t ecall2 (ECALL_NUM, uint64_t, uint64_t);