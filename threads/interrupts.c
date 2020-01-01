#include <stdio.h>
#include <stdbool.h>
#include "interrupts.h"
#include "devices/machine.h"
#include "devices/clint.h"
#include "devices/timer.h"
#include "devices/csr.h"
#include "vm/paging.h"
#include "ecall.h"

typedef enum {
    INST_ALIGN = 0,
    INST_ACCESS = 1,
    INST_ILLEGAL = 2,
    LOAD_ALIGN = 4,
    LOAD_ACCESS = 5,
    STORE_ALIGN = 6,
    STORE_ACCESS = 7,
    U_ECALL = 8,
    S_ECALL = 9,
    INST_PAGE_FAULT = 12,
    LOAD_PAGE_FAULT = 13,
    STORE_PAGE_FAULT = 15
} exception_code_t;

extern void clear_s_intr (uint64_t);

void
handle_s_mode_ecall(struct regfile* regfile) 
{
    ECALL_NUM ecall_code = REGFILE(*regfile, A0);

    switch (ecall_code)
    {
        case SBI_CLEAR_INTR: {
            uint64_t intrs =  REGFILE(*regfile, A1) & S_IRQS;
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
        case S_ECALL:
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
supervisor_interrupt_handler(uint64_t *sp, uint64_t scause, uint64_t sstatus, uint64_t *sepc, uint64_t stval, struct regfile *regfile)
{
    switch (scause) {
        case INST_PAGE_FAULT:
        {
            pte_t pte = *vm_get_pte(vm_get_current_table(), stval, PT_LEVELS - 1);
            if (pte & PTE_V_MASK) {
                vm_access_page (vm_get_current_table (), stval, false);
                return;
            }
            printf ("Page fault accessing instruction at: 0x%p (PTE: 0x%lx)\n", stval, pte);
            while (true);
            break;
        }
        case LOAD_PAGE_FAULT:
        {
            pte_t pte = *vm_get_pte(vm_get_current_table(), stval, PT_LEVELS - 1);
            if (pte & PTE_V_MASK) {
                vm_access_page (vm_get_current_table (), stval, false);
                return;
            }
            printf ("Page fault with data load at: 0x%p (PTE: 0x%lx)\n", stval, pte);
            while (true);
            break;
        }
        case STORE_PAGE_FAULT:
        {
            pte_t pte = *vm_get_pte(vm_get_current_table(), stval, PT_LEVELS - 1);
            if (pte & PTE_V_MASK) {
                vm_access_page (vm_get_current_table (), stval, true);
                return;
            }
            printf ("Page fault with data store at: 0x%p (PTE: 0x%lx)\n", stval, pte);
            while (true);
            break;
        }
        default:
        {
            printf("unknown supervisor interrupt: \n\tsp: 0x%p\n\tscause: 0x%lx\n\tsstatus: 0x%lx\n\tsepc: 0x%p\n\tstval: 0x%lx\n", sp, scause, sstatus, sepc, stval);
            while (true);
        }
    }
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