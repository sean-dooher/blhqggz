#pragma once

#include <stdint.h>

void interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc);
void supervisor_interrupt_handler(uint64_t *sp, uint32_t mcause, uint64_t *mepc);
