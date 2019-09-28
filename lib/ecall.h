#pragma once

#include <stdint.h>


typedef enum {
    ECALL_1=0
} ECALL_NUM;

uint64_t ecall (ECALL_NUM num);