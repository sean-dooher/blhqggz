#include "devices/test.h"

void
poweroff(int exitcode)
{
    #ifdef TEST_LOG
    #endif
    test_power_off(exitcode);
}