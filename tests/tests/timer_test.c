#include <test.h>
#include <stdbool.h>
#include <stdio.h>
#include "devices/timer.h"
#include "devices/machine.h"
#include "devices/platform.h"

void
test_time_period(uint16_t max_count, uint64_t period_ms)
{
    printf("===== TIMER TEST (count: %d, period: %dms) =====\n", max_count, period_ms);

    time_init ();
    time_set (period_ms, PERIOD_MS);

    uint16_t count = 0;
    uint64_t start_time = time_read();

    while (count < max_count) {
        WFI ();
        count += 1;
        printf("TIMER TICK: %d; MAX: %d\n", count, max_count);
    }

    time_clear ();

    uint64_t curr_time = time_read();
    uint64_t time_diff = (curr_time - start_time);
    uint64_t expected_time = count * period_ms * MS_TO_TIMER;
    uint64_t expected_time_max = expected_time * 1.2;
    uint64_t expected_time_min = expected_time * 0.8;

    assert(time_diff < expected_time_max, "Expected time < %d ticks was: %d ticks", expected_time_max, time_diff);
    assert(time_diff >= expected_time_min, "Expected time >= %d ticks was: %d ticks", expected_time_min, time_diff);
    printf("===== END TIMER TEST (expected time: %d, actual: %d) =====\n", expected_time, time_diff);
}

void
run_test()
{
    test_time_period(15, 10);
    test_time_period(150, 2);
}