#include "interrupts.h"
#include "devices/serial.h"

void
machine_interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc)
{
    return;
}

void
supervisor_interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc)
{
    return;
}