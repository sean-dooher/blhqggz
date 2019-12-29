#include <stdio.h>
#include <stdbool.h>

#include "devices/serial.h"
#include "threads/interrupts.h"
#include "devices/timer.h"
#include "devices/machine.h"

void
main()
{
    serial_init ();
    time_init ();
    time_set(1000, PERIOD_MS);

    while (true)
        WFI ();
}
