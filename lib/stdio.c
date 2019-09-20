#include "devices/serial.h"

void
print(char *msg)
{
    serial_puts(msg);
}