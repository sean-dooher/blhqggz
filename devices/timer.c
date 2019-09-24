#include "devices/timer.h"
#include "ecall.h"

uint64_t
read_time ()
{
    return ecall(TIME_READ);
}

void
init_time ()
{
    ecall(TIME_INIT);
}