#include <stdio.h>
#include <stdbool.h>

#include "devices/platform.h"
#include "devices/timer.h"
#include "devices/clint.h"
#include "ecall.h"

#if !defined(SIFIVE_CLINT)
#pragma error "Timer Not Specified"
#endif

static bool repeat;
static uint64_t period, next_time, curr_time;

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
    curr_time = clint_read_mtime();
    next_time = curr_time;
    period = 0;
    repeat = false;
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
            curr_time = clint_read_mtime();
            next_time = curr_time + period;
            debug ("TIMER PERIOD: 0x%lx\n", period);
            clint_schedule (next_time);
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
    if (repeat) {
        next_time = period + next_time;
        clint_schedule (next_time);
    }
#endif
}

void
timer_interrupt () 
{
    curr_time = clint_read_mtime();
    debug("\n======\nIN TIMER\n=====\n");        
    time_restart();
}