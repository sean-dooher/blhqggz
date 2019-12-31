#pragma once

#include <stdint.h>
#include "thread.h"

enum {
    S_INTERRUPTS_ON = 0x222,
    S_INTERRUPTS_OFF = 0x000
} INTR_LEVEL;

void machine_interrupt_handler(uint64_t *sp, uint64_t mcause, uint64_t mstatus, uint64_t *mepc, struct regfile *regfile);
void supervisor_interrupt_handler(uint64_t *sp, uint64_t scause, uint64_t sstatus, uint64_t *sepc, uint64_t stval, struct regfile *regfile);

uint64_t init_interrupts_s (void);
uint64_t set_interrupt_level_s (uint64_t interrupt_level);

void clear_interrupt_s (uint32_t);
void clear_interrupt_m (uint32_t);

void software_interrupt_s (void);