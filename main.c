#include "serial.h"

void main()
{
    serial_init();
    while (1) {
        serial_putc(serial_getc());
    }
}
