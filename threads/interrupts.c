#include <stdio.h>
#include <stdbool.h>
#include "interrupts.h"
#include "devices/clint.h"
#include "ecall.h"

enum {
    S_MODE_ECALL = 9
};


enum {
    A0 = 1
};


void
handle_s_mode_ecall(struct regfile* regfile) {
    uint64_t ecall_code = regfile->reg[A0];

    switch (ecall_code)
    {
        case TIME_INIT:
            break;
        case TIME_CLEAR:
            break;
        case TIME_SET:
            break;
        default:
            printf("Unhandled Ecall: %ld\n", ecall_code);
            while (true);
            break;
    }
}

void
machine_interrupt_handler(uint64_t *sp, uint64_t mcause, uint64_t mstatus, uint64_t *mepc, struct regfile *regfile)
{

    switch (mcause) {
        case S_MODE_ECALL:
            handle_s_mode_ecall(regfile);
            break;
        default:
            printf("unhandled machine interrupt: \n\tsp: 0x%p\n\tmcause: 0x%lx\n\tmstatus: 0x%lx\n\tmepc: 0x%p\n", sp, mcause, mstatus, mepc);
            while (true);
            break;
    }
    return;
}

void
supervisor_interrupt_handler(uint64_t *sp, uint64_t scause, uint64_t sstatus, uint64_t *sepc, struct regfile *regfile)
{
    printf("supervisor interrupt: \n\tsp: 0x%p\n\tscause: 0x%lx\n\tsstatus: 0x%lx\n\tsepc: 0x%p\n", sp, scause, sstatus, sepc);
    while (true);
    return;
}