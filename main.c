#include "devices/serial.h"
#include "exit.h"
#include "threads/interrupts.h"

void
main()
{
    serial_init();
    // enable_interrupts_s();

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
