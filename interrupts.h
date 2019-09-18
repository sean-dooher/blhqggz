#pragma once

#include "stdint.h"

void interrupt_handler(uint32_t *sp, uint32_t mcause, uint64_t *mepc);