#include "devices/serial.h"
#include "exit.h"
#include "threads/interrupts.h"
#include <stdio.h>
#include <stdbool.h>
#include "devices/timer.h"

void
main()
{
    serial_init ();
    time_init ();
    time_set (200, DELAY_MS);

    for (;;);
}
