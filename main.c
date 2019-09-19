#include "devices/serial.h"
#include "exit.h"

void
main()
{
    serial_init();

    while (1) 
    {
        char buffer[512];
        for (int i = 0; i < 512; i++) {
            serial_putc(buffer[i] = serial_getc());

            if (buffer[i] == '\0' || buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }
        poweroff(0);
    }
}
