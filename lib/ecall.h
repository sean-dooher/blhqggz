#pragma once

#include <stdint.h>


typedef enum {
    TIME_INIT = 1,
    TIME_READ = 2
} ECALL_NUM;

uint64_t ecall (ECALL_NUM num);