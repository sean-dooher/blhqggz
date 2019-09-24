#include <stdarg.h>
#include "devices/serial.h"


void
print(const char *msg)
{
    serial_puts(msg);
}