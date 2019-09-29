#include <stdint.h>
#include "devices/clint.h"

uint32_t *CLINT_SIP     = (uint32_t *) (CLINT_BASE + SIFIVE_SIP_BASE);
uint64_t *CLINT_TIMECMP = (uint64_t *) (CLINT_BASE + SIFIVE_TIMECMP_BASE);
uint64_t *CLINT_TIME    = (uint64_t *) (CLINT_BASE + SIFIVE_TIME_BASE);

void
clint_init (void)
{

}

uint64_t
clint_read_mtime (void)
{
    return *CLINT_TIME;
}

void
clint_schedule_ms (uint64_t delay)
{
    clint_schedule (delay * 10000);
}

void
clint_schedule_us (uint64_t delay)
{
    clint_schedule (delay * 10);
}

void
clint_schedule (uint64_t delay)
{
    CLINT_TIMECMP[0] = clint_read_mtime() + delay;
}


void
clint_clear (void)
{
    CLINT_TIMECMP[0] = -1UL;
}