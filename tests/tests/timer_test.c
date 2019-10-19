#include <test.h>
#include <stdbool.h>
#include "devices/timer.h"
#include "devices/machine.h"
#include "devices/platform.h"

void
test_time_period(uint8_t max_count, uint64_t period_ms)
{
    time_init ();
    time_set(period_ms, PERIOD_MS);

    uint8_t count = 0;
    uint64_t start_time = time_read();

    while (count < max_count) {
        WFI ();
        count += 1;
    }

    uint64_t curr_time = time_read();
    uint64_t time_diff = (curr_time - start_time);
    uint64_t expected_time_max = (count + 1) * period_ms * MS_TO_TIMER;

    assert(time_diff < expected_time_max, "Expected time < %d ticks was: %d ticks", expected_time_max, time_diff);
}

void
run_test()
{
    test_time_period(15, 10);
}