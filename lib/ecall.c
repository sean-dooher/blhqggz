#include <ecall.h>


uint64_t 
ecall (ECALL_NUM num)
{
    uint64_t ret = 0;

    asm volatile (
        "mv a0, %1;"
        "ecall;"
        "mv %0, a0"
        : "=r" (ret)
        : "r" (num)
    );

    return ret;
}

uint64_t 
ecall1 (ECALL_NUM num, uint64_t val)
{
    uint64_t ret = 0;

    asm volatile (
        "mv a0, %1;"
        "mv a1, %2;"
        "ecall;"
        "mv %0, a0"
        : "=r" (ret)
        : "r" (num), "r" (val)
    );

    return ret;
}

uint64_t 
ecall2 (ECALL_NUM num, uint64_t val1, uint64_t val2)
{
    uint64_t ret = 0;

    asm volatile (
        "mv a0, %1;"
        "mv a1, %2;"
        "mv a2, %3;"
        "ecall;"
        "mv %0, a0"
        : "=r" (ret)
        : "r" (num), "r" (val1), "r" (val2)
    );

    return ret;
}