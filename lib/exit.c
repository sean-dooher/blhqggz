#include <string.h>
#include "devices/test.h"
#include "devices/serial.h"


void
poweroff(int exitcode)
{
    char exit_buffer[32];
    serial_puts("Power exit code: ");
    serial_puts(itoa(exitcode, exit_buffer, 10));
    serial_puts("\n");
    test_power_off(exitcode);
}