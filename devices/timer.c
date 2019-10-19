#include <stdio.h>
#include <stdbool.h>

#include "devices/platform.h"
#include "devices/timer.h"
#include "devices/clint.h"
#include "ecall.h"

#if !defined(SIFIVE_CLINT)
#pragma error "Timer Not Specified"
#endif

static bool repeat = false;
static uint64_t period;

uint64_t
time_read ()
{
#if defined(SIFIVE_CLINT)
    return clint_read_mtime();
#endif
}

void
time_init ()
{
#if defined(SIFIVE_CLINT)
    clint_init();
#endif
}

void
time_clear ()
{
#if defined(SIFIVE_CLINT)
    clint_clear();
#endif
}

void
time_set (uint64_t time, timer_mode_t mode)
{
#if defined(SIFIVE_CLINT)
    switch (mode) {
        case SET_MS:
            time *= MS_TO_TIMER; // FALLS THROUGH
        case SET: {
            repeat = false;
            clint_schedule (time);
        } break;
        case PERIOD_MS:
            time *= MS_TO_TIMER; // FALLS THROUGH
        case PERIOD: {
            repeat = true;
            period = time;
            clint_delay (time);
        } break;
        case DELAY_MS:
            time *= MS_TO_TIMER; // FALLS THROUGH
        case DELAY: {
            repeat = false;
            clint_delay (time);
        } break;
    }
#endif
}

static void
time_restart ()
{
#if defined(SIFIVE_CLINT)
    if (repeat)
        clint_delay (period);
#endif
}

void
timer_interrupt () 
{
    debug("\n======\nIN TIMER\n=====\n");

        
    time_restart();
}