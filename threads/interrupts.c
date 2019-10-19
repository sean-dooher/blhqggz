#include <stdio.h>
#include <stdbool.h>
#include "interrupts.h"
#include "devices/machine.h"
#include "devices/clint.h"
#include "devices/timer.h"
#include "devices/csr.h"
#include "ecall.h"

enum {
    S_MODE_ECALL = 9
};

extern void clear_s_intr (uint64_t);

void
handle_s_mode_ecall(struct regfile* regfile) {
    ECALL_NUM ecall_code = regfile->reg[A0];

    switch (ecall_code)
    {
        case SBI_CLEAR_INTR: {
            uint64_t intrs = regfile->reg[A1] & S_IRQS;
            uint64_t old_val;
            CSRRC(old_val, mip, intrs);
        } break;
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

void
clear_interrupt_m (uint32_t intrs)
{
    intrs &= (M_IRQS | S_IRQS | U_IRQS);
    uint64_t old_val;
    CSRRC(old_val, mip, intrs);
}

void
clear_interrupt_s (uint32_t intrs)
{
    sbi_clear_interrupts (intrs);
}