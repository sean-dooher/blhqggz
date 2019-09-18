#include "serial.h"
#include "alloca.h"

void
main()
{
    serial_init();

    char *buffer = alloca(512);
    while (1)
    {
        for (int i = 0; i < 512; i++) {
            serial_putc(buffer[i] = serial_getc());

            if (buffer[i] == '\0' || buffer[i] == '\n')
                buffer[i] = '\0';
        }
    }
}
