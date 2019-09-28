#include "devices/timer.h"
#include "ecall.h"

#if defined(QEMU_VIRT) || defined(SIFIVE_U)
#include "devices/clint.h"
#else
#pragma error "Timer Not Specified"
#endif

uint64_t
time_read ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    return clint_read_mtime();
#endif
}

void
time_init ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    clint_init();
#endif
}

void
time_clear ()
{
#if defined(QEMU_VIRT) || defined(SIFIVE_U)
    clint_clear();
#endif
}