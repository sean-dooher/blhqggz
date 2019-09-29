#include <ecall.h>


uint64_t 
ecall (ECALL_NUM num)
{
    uint64_t ret = 0;

    asm volatile (
        "mv a0, %0;"
        "ecall;"
        "mv %1, a0"
        : "=r" (num)
        : "r" (ret)
    );

    return ret;
}

uint64_t 
ecall_int (ECALL_NUM num, uint64_t val)
{
    uint64_t ret = 0;

    return ret;
}