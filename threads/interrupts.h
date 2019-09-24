#pragma once

#include <stdint.h>

void machine_interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc);
void supervisor_interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc);

void machine_interrupt_handler(uint64_t *sp, uint64_t mcause, uint64_t mstatus, uint64_t *mepc);
void supervisor_interrupt_handler(uint64_t *sp, uint64_t scause, uint64_t sstatus, uint64_t *sepc);