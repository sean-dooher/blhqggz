#include <stdio.h>
#include <stdbool.h>

#include "devices/timer.h"
#include "devices/platform.h"
#include "ecall.h"

#if !defined(QEMU_VIRT) && !defined(SIFIVE_U)
#pragma error "Timer Not Specified"
#endif

static bool repeat = false;
static uint64_t period;

uint64_t
time_read ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    return ecall (TIME_READ);
#endif
}

void
time_init ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    ecall (TIME_INIT);
#endif
}

void
time_clear ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    ecall (TIME_CLEAR);
#endif
}

void
time_set (uint64_t time, timer_mode_t mode)
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    switch (mode) {
        case PERIOD_MS:
            time *= MS_TO_TIMER;
        case PERIOD: {
            repeat = true;
            period = time;
            debug("SETTING PERIODIC TIMER: %d ms\n", time / MS_TO_TIMER);
            printf("HERE");
            ecall2 (TIME_SET, period, DELAY);
        } break;
        default: {
            repeat = false;
            ecall2 (TIME_SET, time, mode);
        } break;
    }

#endif
}

void
timer_interrupt () 
{
    printf("\n======\nIN TIMER\n=====\n");
    if (repeat)
        time_set(1000, DELAY_MS);
    return;
}