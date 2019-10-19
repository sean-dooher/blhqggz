#include <stdint.h>
#include <stdio.h>
#include "devices/clint.h"

#define MAX_TIME 0x0FFFFFFFFFFFFFFF

uint32_t *CLINT_SIP     = (uint32_t *) (CLINT_BASE + SIFIVE_SIP_BASE);
uint64_t *CLINT_TIMECMP = (uint64_t *) (CLINT_BASE + SIFIVE_TIMECMP_BASE);
uint64_t *CLINT_TIME    = (uint64_t *) (CLINT_BASE + SIFIVE_TIME_BASE);

void
clint_init (void)
{

}

void
clint_schedule (uint64_t time)
{
    CLINT_TIMECMP[0] = time;
}

void
clint_delay (uint64_t delay)
{
    CLINT_TIMECMP[0] = clint_read_mtime() + delay;
}

void
clint_clear (void)
{
    CLINT_TIMECMP[0] = MAX_TIME; // todo: figure out why msb causes bug
}

uint64_t
clint_read_mtime (void)
{
    return *CLINT_TIME;
}
