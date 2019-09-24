#include <stdio.h>
#include <stdbool.h>
#include "interrupts.h"

void
machine_interrupt_handler(uint64_t *sp, uint64_t mcause, uint64_t mstatus, uint64_t *mepc)
{
    printf("machine interrupt: \n\tsp: 0x%p\n\tscause: 0x%lx\n\tsstatus: 0x%lx\n\tsepc: 0x%p\n", sp, mcause, mstatus, mepc);
    while (true);
    return;
}

void
supervisor_interrupt_handler(uint64_t *sp, uint64_t scause, uint64_t sstatus, uint64_t *sepc)
{
    printf("supervisor interrupt: \n\tsp: 0x%p\n\tscause: 0x%lx\n\tsstatus: 0x%lx\n\tsepc: 0x%p\n", sp, scause, sstatus, sepc);
    while (true);
    return;
}