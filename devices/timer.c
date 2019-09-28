#include "devices/timer.h"
#include "ecall.h"
#include "devices/clint.h"

uint64_t
time_read ()
{
    return clint_read_mtime();
}

void
time_init ()
{
    clint_init();
}

void
time_clear ()
{
    clint_clear();
}