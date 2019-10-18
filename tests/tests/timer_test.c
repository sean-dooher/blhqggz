#include <test.h>
#include <stdbool.h>
#include "devices/timer.h"
#include "devices/machine.h"
#include "devices/platform.h"

void
test_reverse()
{
    time_init ();
    time_set(10, PERIOD_MS);

    uint8_t count = 0;
    uint64_t start_time = time_read();

    while (count < 4) {
        WFI ();
        count += 1;
    }

    uint64_t curr_time = time_read();
    uint64_t time_diff = (start_time - curr_time);

    assert(time_diff < (count + 1) * 100 * MS_TO_TIMER, "Expected time between 4s and 5s was: %d ms", curr_time);
}

void
run_test()
{
    test_reverse();
}