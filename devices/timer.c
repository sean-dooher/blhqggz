#include <stdio.h>

#include "devices/timer.h"
#include "ecall.h"

#if !defined(QEMU_VIRT) && !defined(SIFIVE_U)
#pragma error "Timer Not Specified"
#endif

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
    ecall2 (TIME_SET, time, mode);
#endif
}

void
timer_interrupt () 
{
    return;
}