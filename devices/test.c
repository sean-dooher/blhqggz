#include <stdint.h>
#include "test.h"

static uint32_t *test = (uint32_t *) TEST_CTRL_ADDR;

void
test_power_off(int status)
{
    *test = TEST_PASS;
}