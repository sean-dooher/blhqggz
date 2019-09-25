#include "devices/timer.h"
#include "ecall.h"
#include "devices/clint.h"

uint64_t
read_time ()
{
    return clint_read_mtime();
}

void
init_time ()
{
    clint_init();
}