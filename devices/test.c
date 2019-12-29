#include <stdint.h>
#include <stdio.h>
#include "test.h"

static volatile uint64_t *test = (uint64_t *) TEST_CTRL_ADDR;

void
test_power_off(int status)
{
    uint64_t exit_code = (status << 16);
    if (status == 0) {
        exit_code |= TEST_PASS;
    } else {
        exit_code |= TEST_FAIL;
    }
    *test = exit_code;
}