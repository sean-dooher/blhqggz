#pragma once

#include <stdint.h>
#include "devices/platform.h"

extern uint32_t *CLINT_SIP;
extern uint64_t *CLINT_TIMECMP;
extern uint64_t *CLINT_TIME;

void clint_init (void);

uint64_t clint_read_mtime (void);

void clint_schedule_ms (uint64_t);

void clint_schedule (uint64_t);

void clint_clear (void);