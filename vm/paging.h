#pragma once

#include <stdint.h>
#include "devices/machine.h"

extern const void *KERNEL_TEXT_BASE;
extern const void *KERNEL_TEXT_END;
extern const void *FREE_MEM_BASE;
extern const void *FREE_MEM_END;

#define N_PAGES(SIZE) ((SIZE + (PAGE_SIZE - 1)) / PAGE_SIZE)

void vm_init_early (void);