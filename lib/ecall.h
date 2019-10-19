#pragma once

#define SBI_TIME_INIT  1
#define SBI_TIME_CLEAR 2
#define SBI_TIME_SET   3
#define SBI_TIME_READ  4
#define SBI_CLEAR_INTR 5

#ifndef __ASSEMBLER__
#include <stdint.h>
typedef enum {
    CLEAR_INTR=SBI_CLEAR_INTR
} ECALL_NUM;

uint64_t ecall (ECALL_NUM);
uint64_t ecall1 (ECALL_NUM, uint64_t);
uint64_t ecall2 (ECALL_NUM, uint64_t, uint64_t);

#define sbi_clear_interrupts(intrs) ecall1 (SBI_CLEAR_INTR, intrs)
#endif