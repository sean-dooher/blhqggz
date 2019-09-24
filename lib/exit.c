#include <string.h>
#include <stdio.h>
#include "devices/test.h"


void
poweroff(int exitcode)
{
    printf("Power exit code: %d\n", exitcode);
    test_power_off(exitcode);
}